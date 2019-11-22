#include <Arduino_FreeRTOS.h>
#include <EDB.h>
#include <EEPROM.h>

#define TABLE_SIZE 512

EDB db(&writer, &reader);

struct Log {
  //int id;
  int msg;
  float data;
}logTemp;

void setup() {
  Serial.begin(9600);
  db.create(0, TABLE_SIZE, (unsigned int)sizeof(logTemp));
}

void loop() {
  EDB_Status dbst = EDB_OK;

  dbst=addRecord(10, GetTemp());
  dbst=addRecord(33, 47);
  for (int i = 0; i < db.count(); i++){
    Serial.println(db.count());
    dbst=readRecord(i);
  }
  
  delay(1000);
  printError(dbst);
}
