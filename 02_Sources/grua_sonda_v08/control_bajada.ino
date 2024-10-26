int control_bajada(float S_VEL) {
  double diferenciaM;
  double velAnterior = 0;
  bool velProm = false;

  dif = encoderPosition - lastPosition;
  lastPosition = encoderPosition;
  diferenciaM = (dif * kDistance);
  VEL = diferenciaM / 0.1;
  velAnterior = VEL;
  if (VEL < 0)VEL = -VEL;
  error = VEL - S_VEL;                //Error+ si VEL > U_VEL -> BAJAR VEL
                                      //Error- si VEL < U_VEL -> SUBIR VEL
  if (!digitalRead(TENSION)) {
    if (error > 0) {
      if (error > 0.005) {
        pwmF = pwmF - 0.05;
        if (error > 0.02) {
          pwmF = pwmF - 0.1;
        }
        if (error > 0.06) {
          pwmF = pwmF - 0.5;
        }
        if (pwmF < 0)pwmF = 0;
      }
    } else {
      if (pwmF < 80)pwmF = 80;
      if (VEL == 0.00) pwmF = pwmF + 5;
      if (error < -0.005) {
        pwmF = pwmF + 0.05;
        if (error < -0.02) {
          pwmF = pwmF + 0.1;
        }
        if (error < -0.06) {
          pwmF = pwmF + 0.5;
        }
        if (pwmF > 255)pwmF = 255;
      }
    }
  } else {
    pwmF = pwmF - 10;
    if (pwmF < 0)pwmF = 0;
  }
  PWM = round(pwmF);
  return PWM;
}
