void rutina_bajada(float disMax, float velocidad) {
  if (digitalRead(FIN_CARRERA) == HIGH)probeStatus = true;
  if (probeStatus) {
    detachInterrupt(FIN_CARRERA);
    probeStatus = false;
    intDesactivada = true;
  }
  if (encoderPosition > 50 && intDesactivada) {
    attachInterrupt(FIN_CARRERA, int1, RISING);
    intDesactivada = false;
  }
  distancia = encoderPosition * kDistance;
  if (distancia < disMax) {
    PWM = control_bajada(velocidad);
    motorControl(BAJAR, PWM);
  } else modo = DETENIDO;
}

void rutina_bajada_automatica(float disMax, float velocidad) {
  if (digitalRead(FIN_CARRERA) == HIGH)probeStatus = true;
  if (probeStatus) {
    detachInterrupt(FIN_CARRERA);
    probeStatus = false;
    intDesactivada = true;
  }
  if (encoderPosition > 50 && intDesactivada) {
    attachInterrupt(FIN_CARRERA, int1, RISING);
    intDesactivada = false;
  }
  distancia = encoderPosition * kDistance;
  if (distancia < disMax) {
    PWM = control_bajada(velocidad);
    motorControl(BAJAR, PWM);
  } else {
    PWM = 0;
    motorControl(PARAR, PWM);
    fOperacionAuto = SUBIENDO_A;
  }
}
