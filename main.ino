#include <Servo.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Ultrasonic.h>

#define PIN_SERVO_SX_AVANTI 2
#define PIN_SERVO_SX_INDIETRO 3
#define PIN_SERVO_DX_AVANTI 4
#define PIN_SERVO_DX_INDIETRO 5
#define PIN_TRACCIAMENTO_SINISTRA A2
#define PIN_TRACCIAMENTO_CENTRO A3
#define PIN_TRACCIAMENTO_DESTRA A4
#define PIN_IR_RECEIVER 6
#define PIN_LED_VERDE 7
#define PIN_LED_ROSSO 8
#define PIN_ULTRASONIC_TRIG 9
#define PIN_ULTRASONIC_ECHO 10
#define PIN_LCD_RS 11
#define PIN_LCD_EN 12
#define PIN_LCD_D4 13
#define PIN_LCD_D5 A0
#define PIN_LCD_D6 A1
#define PIN_LCD_D7 A2

#define VELOCITA_AVANTI 255
#define VELOCITA_FERMO 0
#define VELOCITA_GIRA_LV1 100
#define VELOCITA_GIRA_LV2 150
#define VELOCITA_GIRA_LV3 200
#define VELOCITA_GIRA_LV4 255
#define DISTANZA_OSTACOLO 10

Servo servoSxAvanti;
Servo servoSxIndietro;
Servo servoDxAvanti;
Servo servoDxIndietro;

IRrecv irrecv(PIN_IR_RECEIVER);
decode_results results;

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
Ultrasonic ultrasonic(PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO);

void setup() {
  configurazionePin();
  irrecv.enableIRIn();
  lcd.begin(16, 2);
  lcd.print("Pronto");
}

void loop() {
  u8 valoreSensoreTracciamento = 0;
  valoreSensoreTracciamento = ottieniValoreSensoreTracciamento(); //ottenere il valore del sensore

  int distanza = ultrasonic.Ranging(CM);
  if (distanza <= DISTANZA_OSTACOLO) {
    digitalWrite(PIN_LED_ROSSO, HIGH);
    eseguiMotore(VELOCITA_FERMO, VELOCITA_FERMO); //carrello si ferma
    lcd.clear();
    lcd.print("Ostacolo rilevato");
    delay(2000);
    lcd.clear();
    lcd.print("Pronto");
    digitalWrite(PIN_LED_ROSSO, LOW);
    return;
  }

 if (irrecv.decode(&results)) {
    switch (results.value) {
      case 0xFFA25D: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_AVANTI, VELOCITA_AVANTI); //carrello muove avanti
        break;
      case 0xFF629D: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_GIRA_LV4, VELOCITA_GIRA_LV1); //carrello gira
        break;
      case 0xFFE21D: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_GIRA_LV3, VELOCITA_GIRA_LV2); //carrello gira a destra
        break;
      case 0xFF22DD: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_AVANTI, VELOCITA_AVANTI); //carrello muove avanti
        break;
      case 0xFF02FD: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_GIRA_LV2, VELOCITA_GIRA_LV3); //carrello gira a sinistra
        break;
      case 0xFFC23D: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_GIRA_LV1, VELOCITA_GIRA_LV4); //carrello gira a destra
        break;
      case 0xFFE01F: // Codice del pulsante del telecomando IR
        eseguiMotore(VELOCITA_FERMO, VELOCITA_FERMO); //carrello si ferma
        break;
      default:
        break;
    }
    irrecv.resume();
  } else {
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
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_ROSSO, OUTPUT);
  digitalWrite(PIN_LED_VERDE, HIGH);
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

  if (velocitaSx == VELOCITA_FERMO && velocitaDx == VELOCITA_FERMO) {
    lcd.clear();
    lcd.print("Carrello fermo");
  } else {
    lcd.clear();
    lcd.print("Carrello in movimento");
  }
}


void resetAzioneCarrello() {
  eseguiMotore(0, 0);
  lcd.clear();
  lcd.print("Reset azione carrello");
}
