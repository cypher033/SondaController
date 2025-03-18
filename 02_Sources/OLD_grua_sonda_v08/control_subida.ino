int control_subida(float S_VEL) {
  double diferenciaM;

  dif = encoderPosition - lastPosition;
  lastPosition = encoderPosition;
  diferenciaM = (dif * kDistance);
  VEL = diferenciaM / 0.1;
  if (VEL < 0)VEL = -VEL;
  error = VEL - S_VEL;                //Error+ si VEL > U_VEL -> BAJAR VEL
                                      //Error- si VEL < U_VEL -> SUBIR VEL
  if (!digitalRead(TENSION) || (esp_timer_get_time() - tiempoModo4) < tSubida) {
    if (error > 0) {
      if (error > 0.005) {
        PWM--;
        if (error > 0.02) {
          PWM--;
        }
        if (error > 0.06) {
          PWM--;
        }
        if (PWM < 0)PWM = 0;
      }
    } else {
      if (PWM < 80)PWM = 80;
      if (VEL == 0.00) {
        if (digitalRead(TENSION)) PWM = 175;
        else PWM = PWM + 5;
      }

      if (error < -0.005) {
        PWM++;
        if (error < -0.02) {
          PWM++;
        }
        if (error < -0.06) {
          PWM++;
        }
        if (PWM > 255)PWM = 255;
      }
    }
  } else {
    PWM = PWM - 10;
    if (PWM < 0)PWM = 0;
  }
  return PWM;
}
