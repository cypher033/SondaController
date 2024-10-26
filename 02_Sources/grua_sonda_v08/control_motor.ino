void motorControl(int motorMovement, int PWM) { //0 stop; 1 abajo; 2 arriba
  switch (motorMovement) {
    case 0:
      digitalWrite(MOT_A, LOW);
      digitalWrite(MOT_B, LOW);
      ledcWrite(ledChannel, 0);
      break;
    case 1:
      digitalWrite(MOT_A, HIGH);
      digitalWrite(MOT_B, LOW);
      ledcWrite(ledChannel, PWM);
      break;
    case 2:
      if (!digitalRead(FIN_CARRERA)) {
        digitalWrite(MOT_A, LOW);
        digitalWrite(MOT_B, HIGH);
        ledcWrite(ledChannel, PWM);
      }
      break;
    default:
      digitalWrite(MOT_A, LOW);
      digitalWrite(MOT_B, LOW);
      ledcWrite(ledChannel, 0);
      break;
  }
}
