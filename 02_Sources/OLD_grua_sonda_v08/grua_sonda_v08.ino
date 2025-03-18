#include <Preferences.h>

#define   IENC_A             32 //GPIO de interrupción del encoder
#define   ENC_B              39
#define   FIN_CARRERA        22 //Fin de carrera
#define   G_BUTTON           13 //Azul físico
#define   R_BUTTON           12 //Amarillo Físico
#define   B_BUTTON           14 //Rojo físico
#define   TENSION            21 //Switch sensor de tensión
#define   CORRIENTE          36 //Resistencia sensora de corriente
#define   PWM_OUT            17 //Salida de PWM. GPIO cortocircuitado
#define   MOT_A              16 //LPWM
#define   MOT_B              4  //RPWM

#define   PARAR              0
#define   BAJAR              1
#define   SUBIR              2

// setting PWM properties
const int freq = 20000;
const int ledChannel = 0;
const int resolution = 8;

//Variables de tiempo
unsigned long ts = 100 * 1000;
uint64_t last_ts = 0.00;
unsigned long B_time = 3000 * 1000;
uint64_t last_timeB = 0;
uint64_t tiempoEnCasa;
unsigned long tSubida = 5000 * 1000;
uint64_t tiempoModo4;

//Variables del encoder
int lastStateENC_A = LOW;
int stateENC_A = LOW;
volatile long encoderPosition;
int Position = 0;
long diametter = 0.036;
const double kDistance = (0.036 * 3.1416) / 400; //Diámetro*pi/pasos = (0.036*3)/400 = 0.15/400 = 0.000375***

//Variables del control
long lastPosition = 0;
int PWM;
float valorCorriente[5];
long dif = 0;
float distancia = 0;
float VEL = 0.05;   //setpoint
float error = 0;
int contRepeticiones;
float pwmF;

enum {
  CASA,
  AUTOMATICO,
  DETENIDO,
  BAJANDO,
  SUBIENDO
} modo;

enum {
  INICIO_A,
  ESPERANDO_A,
  BAJANDO_A,
  SUBIENDO_A,
  FIN_A,
  RESET_A
} fOperacionAuto;

bool valoresBT = false;
bool probeStatus = false;
unsigned long contCiclo;
bool funcionAuto;           //Error por falla en la energía.
bool intDesactivada;
bool sobreCorriente;

//variables para botones
bool B_Status;
bool lastG, lastR, lastB;
bool pulsoBotones;

//Variables de estado del motor
int motorMovement = 0;                //0 = stop; 1 = down; 2 = up
bool tope = LOW;

//Variables modificadas por el usuario, distancias en m, velocidad en m/s, tiempo en us.
float repeticiones;                          //Si es -1 que se detenga en la distancia máxima.
float dMax;
float velDeseada;
float tMuestreo;

struct Valores {
  int repeticiones_;
  float dMax_;
  float velDeseada_;
  float tMuestreo_;
};
Valores valoresUsuario;

Preferences datosGuardados;

//Interrupción del encoder
void IRAM_ATTR ai0() {
  if (digitalRead(ENC_B) == LOW) {
    encoderPosition++;
  } else {
    encoderPosition--;
  }
}

//Interrupción fin de carrera, Detecta cuando llega la sonda
void IRAM_ATTR int1() {
  if (digitalRead(FIN_CARRERA) == HIGH) {     //Switch NC = 0; al abrir da 1, interrupción flanco de subida
    probeStatus = true;          //True = sonda guardada; False = sonda está en el pozo
  }
}

void setup() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWM_OUT, ledChannel);

  //Inicialización de variables
  B_Status = false;
  lastG = false; lastR = false; lastB = false;
  pulsoBotones = false;
  contCiclo = 0;
  encoderPosition = 0;
  lastPosition = 0;
  if (digitalRead(FIN_CARRERA) == HIGH)modo = CASA;
  else modo = DETENIDO;
  PWM = 0;
  intDesactivada = false;     //Si es false, la interrupción del fin de carrera está activada
  contRepeticiones = 0;

  Serial.begin(115200);
  Serial.setTimeout(100);

  pinMode(IENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(PWM_OUT, OUTPUT);
  pinMode(MOT_A, OUTPUT);
  pinMode(MOT_B, OUTPUT);
  pinMode(G_BUTTON, INPUT_PULLUP);
  pinMode(R_BUTTON, INPUT_PULLUP);
  pinMode(B_BUTTON, INPUT_PULLUP);
  pinMode(FIN_CARRERA, INPUT_PULLUP);
  pinMode(TENSION, INPUT_PULLUP);

  attachInterrupt(IENC_A, ai0, RISING);
  attachInterrupt(FIN_CARRERA, int1, RISING);

  datosGuardados.begin("grua_sonda", false);
  
  if (digitalRead(FIN_CARRERA)) modo = CASA;
  if (datosGuardados.getInt("modo", DETENIDO) == AUTOMATICO) {
    repeticiones = datosGuardados.getInt("repeticiones", 0);
    dMax = datosGuardados.getFloat("dmax", 0);
    velDeseada = datosGuardados.getFloat("veldeseada", 0);
    tMuestreo = datosGuardados.getFloat("tmuestreo", 0);
    contRepeticiones = datosGuardados.getInt("contador", -1);
    modo = AUTOMATICO;
    fOperacionAuto = RESET_A;
    valoresBT = true;
  }

}

void loop() {
  if (Serial.available() > 0 && motorMovement == PARAR) {
    valoresUsuario = comunicacion_serial();
    repeticiones = valoresUsuario.repeticiones_;
    dMax = valoresUsuario.dMax_;
    velDeseada = valoresUsuario.velDeseada_;
    tMuestreo = valoresUsuario.tMuestreo_;
    valoresBT = true;
    datosGuardados.putInt("repeticiones", repeticiones);
    datosGuardados.putFloat("dmax", dMax);
    datosGuardados.putFloat("veldeseada", velDeseada);
    datosGuardados.putFloat("tmuestreo", tMuestreo);
  }
  if ((esp_timer_get_time() - last_ts) >= ts) {
    contCiclo = 0;
    funcion_sistema();
    last_ts = esp_timer_get_time();
  }
  contCiclo++;
}
