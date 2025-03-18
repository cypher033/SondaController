void buttons() {
  bool GB, RB, BB;
  GB = !digitalRead(G_BUTTON);
  RB = !digitalRead(R_BUTTON);
  BB = !digitalRead(B_BUTTON);
  if ((GB && !lastG) || (RB && !lastR) || (BB && !lastB)) {
    if (modo != CASA) {
      modo = DETENIDO;
      datosGuardados.putInt("modo", modo); //Guardar valor en la EEPROM
    }
    last_timeB = esp_timer_get_time();
  }
  if (GB && lastG && valoresBT) {
    if (esp_timer_get_time() - last_timeB >= B_time) {
      modo = AUTOMATICO;
      fOperacionAuto = INICIO_A;
      datosGuardados.putInt("modo", modo); //Guardar valor en la EEPROM
      datosGuardados.putBool("funcionAuto", funcionAuto);
    }
  }
  if (RB && lastR) {
    if ((esp_timer_get_time() - last_timeB >= B_time) && modo != 0) {
      modo = SUBIENDO;
      tiempoModo4 = esp_timer_get_time();
      datosGuardados.putInt("modo", modo); //Guardar valor en la EEPROM
    }
  }
  if (BB && lastB) {
    if (esp_timer_get_time() - last_timeB >= B_time) {
      modo = BAJANDO;
      datosGuardados.putInt("modo", modo); //Guardar valor en la EEPROM
    }
  }

  lastG = GB;
  lastR = RB;
  lastB = BB;
}
