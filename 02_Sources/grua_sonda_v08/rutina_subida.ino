void rutina_subida(float velocidad) {
  if (probeStatus) {
    modo = CASA;
    PWM = control_subida(0.05);
    motorControl(SUBIR, PWM);
    delay(1900);
  }
  PWM = control_subida(velocidad);
  motorControl(SUBIR, PWM);
  sensar_corriente();
}

void rutina_subida_automatica(float velocidad) {
  if (probeStatus) {
    contRepeticiones = datosGuardados.getInt("contador", 0);
    contRepeticiones++;
    datosGuardados.putInt("contador", contRepeticiones);
    if (contRepeticiones >= repeticiones) {
      fOperacionAuto = FIN_A;
    } else {
      fOperacionAuto = ESPERANDO_A;
      tiempoEnCasa = esp_timer_get_time();
    }
    PWM = control_subida(0.05);
    motorControl(SUBIR, PWM);
    delay(1900);
  }
  PWM = control_subida(velocidad);
  motorControl(SUBIR, PWM);
  sensar_corriente();
}
