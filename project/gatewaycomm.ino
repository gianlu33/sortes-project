//TODO functions

String onReceive(int packetSize) {
  if (packetSize == 0) return "error";  // if there's no packet, return
  String header = "";
  Serial.println("in");
  // read packet header bytes:
  //char a = LoRa.read();
  //char b = LoRa.read();
  //char c = LoRa.read();
  //char d = LoRa.read();
  //for (int i = 0; i<4; i++){
    //header += (char)LoRa.read();
  //}

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  String timeToNextMsg = incoming;

  //Serial.print(a);
  //Serial.print(b);
  //Serial.print(c);
  //Serial.print(d);
  //Serial.print(" ");
  Serial.println(timeToNextMsg);

  return incoming;
}
