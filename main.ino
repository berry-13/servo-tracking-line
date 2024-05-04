#include <Servo.h>

#define PIN_SERVO_SX_AVANTI 2
#define PIN_SERVO_SX_INDIETRO 3
#define PIN_SERVO_DX_AVANTI 4
#define PIN_SERVO_DX_INDIETRO 5
#define PIN_TRACCIAMENTO_SINISTRA A2
#define PIN_TRACCIAMENTO_CENTRO A3
#define PIN_TRACCIAMENTO_DESTRA A4

#define VELOCITA_AVANTI 255
#define VELOCITA_FERMO 0
#define VELOCITA_GIRA_LV1 100
#define VELOCITA_GIRA_LV2 150
#define VELOCITA_GIRA_LV3 200
#define VELOCITA_GIRA_LV4 255

Servo servoSxAvanti;
Servo servoSxIndietro;
Servo servoDxAvanti;
Servo servoDxIndietro;

void setup() {
  configurazionePin();
}

void loop() {
  u8 valoreSensoreTracciamento = 0;
  valoreSensoreTracciamento = ottieniValoreSensoreTracciamento(); //ottenere il valore del sensore

  switch (valoreSensoreTracciamento)
  {
    case 0:   //000
      eseguiMotore(VELOCITA_AVANTI, VELOCITA_AVANTI); //carrello muove avanti
      break;
    case 7:   //111
      eseguiMotore(VELOCITA_FERMO, VELOCITA_FERMO); //carrello si ferma
      break;
    case 1:   //001
      eseguiMotore(VELOCITA_GIRA_LV4, VELOCITA_GIRA_LV1); //carrello gira
      break;
    case 3:   //011
      eseguiMotore(VELOCITA_GIRA_LV3, VELOCITA_GIRA_LV2); //carrello gira a destra
      break;
    case 2:   //010
    case 5:   //101
      eseguiMotore(VELOCITA_AVANTI, VELOCITA_AVANTI);  //carrello muove avanti
      break;
    case 6:   //110
      eseguiMotore(VELOCITA_GIRA_LV2, VELOCITA_GIRA_LV3); //carrello gira a sinistra
      break;
    case 4:   //100
      eseguiMotore(VELOCITA_GIRA_LV1, VELOCITA_GIRA_LV4); //carrello gira a destra
      break;
    default:
      break;
  }
}

u8 ottieniValoreSensoreTracciamento() {
  u8 valoreSensoreTracciamento = 0;
  valoreSensoreTracciamento = (digitalRead(PIN_TRACCIAMENTO_SINISTRA) == 1 ? 1 : 0) << 2 | (digitalRead(PIN_TRACCIAMENTO_CENTRO) == 1 ? 1 : 0) << 1 | (digitalRead(PIN_TRACCIAMENTO_DESTRA) == 1 ? 1 : 0);
  return valoreSensoreTracciamento;
}

void configurazionePin() {
  servoSxAvanti.attach(PIN_SERVO_SX_AVANTI);
  servoSxIndietro.attach(PIN_SERVO_SX_INDIETRO);
  servoDxAvanti.attach(PIN_SERVO_DX_AVANTI);
  servoDxIndietro.attach(PIN_SERVO_DX_INDIETRO);
  pinMode(PIN_TRACCIAMENTO_SINISTRA, INPUT);
  pinMode(PIN_TRACCIAMENTO_DESTRA, INPUT);
  pinMode(PIN_TRACCIAMENTO_CENTRO, INPUT);
}

void eseguiMotore(int velocitaSx, int velocitaDx) {
  int angoloSx = map(velocitaSx, -255, 255, 0, 180);
  int angoloDx = map(velocitaDx, -255, 255, 0, 180);

  if (velocitaSx > 0) {
    servoSxAvanti.write(angoloSx);
    servoSxIndietro.write(0); // Fermo
  } else if (velocitaSx < 0) {
    servoSxAvanti.write(0); // Fermo
    servoSxIndietro.write(-angoloSx);
  } else {
    servoSxAvanti.write(0); // Fermo
    servoSxIndietro.write(0); // Fermo
  }

  if (velocitaDx > 0) {
    servoDxAvanti.write(angoloDx);
    servoDxIndietro.write(0); // Fermo
  } else if (velocitaDx < 0) {
    servoDxAvanti.write(0); // Fermo
    servoDxIndietro.write(-angoloDx);
  } else {
    servoDxAvanti.write(0); // Fermo
    servoDxIndietro.write(0); // Fermo
  }
}

void resetAzioneCarrello() {
  eseguiMotore(0, 0);
}
