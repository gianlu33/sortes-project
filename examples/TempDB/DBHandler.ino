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

EDB_Status addRecord(int msg, float data)
{
  Log logTempLoc;
  logTempLoc.msg = msg;
  logTempLoc.data = data;
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
  EDB_Status st = db.readRec(id, EDB_REC logTemp);
  if (st == EDB_OK)
  {
    Serial.print("Message: ");
    Serial.print(logTemp.msg);
    Serial.print(" Temperature: ");
    Serial.println(logTemp.data);
  }
  return st;
}
