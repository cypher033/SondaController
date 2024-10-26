void funcion_sistema() {
  float velocidad = 0.05;
  float disMax = 20;
  buttons();
  switch (modo) {
    case CASA:
      PWM = 0;
      motorControl(0, 0);          //Motor detenido. Cero velocidad.
      encoderPosition = 0;         //Reinicio valor de la posición.
      lastPosition = 0;
      break;
    case AUTOMATICO:
      switch (fOperacionAuto) {
        case INICIO_A:
          contRepeticiones = 0;
          datosGuardados.putInt("contador", contRepeticiones);
          fOperacionAuto = BAJANDO_A;
          break;
        case ESPERANDO_A:
          PWM = 0;
          motorControl(PARAR, PWM);          //Motor detenido. Cero velocidad.
          encoderPosition = 0;               //Reinicio valor de la posición.
          lastPosition = 0;
          if ((esp_timer_get_time() - tiempoEnCasa) >= tMuestreo) fOperacionAuto = BAJANDO_A;
          break;
        case BAJANDO_A:
          rutina_bajada_automatica(dMax, velDeseada);
          break;
        case SUBIENDO_A:
          rutina_subida_automatica(velDeseada);
          break;
        case FIN_A:
          modo = CASA;
          funcionAuto = false;
          break;
        case RESET_A:
          repeticiones = datosGuardados.getInt("repeticiones", 0);
          dMax = datosGuardados.getFloat("dmax", 0);
          velDeseada = datosGuardados.getFloat("veldeseada", 0);
          tMuestreo = datosGuardados.getFloat("tmuestreo", 0);
          contRepeticiones = datosGuardados.getInt("contador", -1);
          if (!probeStatus)rutina_subida_automatica(velocidad);
          else fOperacionAuto = ESPERANDO_A;
          break;
        default:
          modo = CASA;
          break;
      }
      break;
    case DETENIDO:
      PWM = 0;
      motorControl(PARAR, PWM);
      break;
    case BAJANDO:
      funcionAuto = false;
      rutina_bajada(disMax, velocidad);
      break;
    case SUBIENDO:
      funcionAuto = false;
      rutina_subida(velocidad);
      break;
    default:
      modo = DETENIDO;
      break;
  }
}
