Valores comunicacion_serial() {
  //Variables para la comunicación serial
  bool request;          //Datos en el buffer
  int dataInit;
  int positionComma;
  String stringRead;        //Cadena leída
  String info[4];

  Valores setValues;

  request = Serial.readStringUntil('#');
  if (request == 1) {
    stringRead = Serial.readStringUntil ('\n');
    dataInit = 0;
    for (int i = 0; i < 4; i++) {
      positionComma = stringRead.indexOf(',', dataInit);
      info[i] = stringRead.substring(dataInit, positionComma);
      dataInit = positionComma + 1;
    }
    setValues = {info[0].toInt(), info[1].toFloat(), info[2].toFloat(), info[3].toFloat()};
  }
  return setValues;
}
