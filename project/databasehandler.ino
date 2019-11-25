// EEPROM Database handling functions

void printError(EDB_Status err)
{
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      Serial.println("Error: Record index out of range");
      break;
    case EDB_TABLE_FULL:
      Serial.println("Error: Table full");
      break;
    case EDB_OK:
      break;
    default:
      break;
  }
}

void writer(unsigned long address, byte data)
{
  EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
  return EEPROM.read(address);
}

EDB_Status addRecord(struct Log logTempLoc)
{
  /*
  Log logTempLoc;
  logTempLoc.msg = msg;
  logTempLoc.data = data;
  */
  EDB_Status st = db.appendRec(EDB_REC logTempLoc);
  return st;
}

EDB_Status deleteRecord(int id)
{
  EDB_Status st = db.deleteRec(id);
  return st;
}

EDB_Status readRecord(int id)
{
  Log logTempLoc;
  EDB_Status st = db.readRec(id, EDB_REC logTempLoc);
  if (st == EDB_OK)
  {
    Serial.print("Message: ");
    Serial.print(logTempLoc.msg);
    Serial.print(" Temperature: ");
    Serial.println(logTempLoc.data);
  }
  return st;
}

EDB_Status readAllRec(){
  EDB_Status st = EDB_OK;
  for(int i = 1; i <= db.count(); i++){
    Serial.print("Data ");
    Serial.print(i+1);
    Serial.print(".: ");
    st = readRecord(i);
    if(st != EDB_OK)
      return st;
  }
  return st;
}

void deleteAllRecords(){
  db.clear();
}

float getTemp()
{
  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  // Delay task, wait for voltages to become stable.
  vTaskDelay( 20 / portTICK_PERIOD_MS);            

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  unsigned int wADC = ADCW;

  // Calculate temperature based on calibration data.
  float temperature = (wADC / 2.1) - 190;

  // Convert from K to C°
  return (temperature-273);
}
