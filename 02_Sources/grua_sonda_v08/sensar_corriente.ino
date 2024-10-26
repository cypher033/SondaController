void sensar_corriente() {
  float valorADC;
  float corriente;
  valorADC = 0;
  valorADC = analogRead(CORRIENTE);
  corriente = 0;
  for (int i = 4; i > 0; i--) {
    valorCorriente[i] = valorCorriente[i-1];
    corriente = corriente + valorCorriente[i];
  }
  corriente = corriente +valorADC;
  valorCorriente[0] = corriente/5;
  corriente = 2.63 + (0.011*valorCorriente[0]);
  if(corriente > 9){modo = DETENIDO; sobreCorriente = true; }
}
