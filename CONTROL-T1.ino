#include <EEPROM.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#include <string>
#include <iostream>
#include "AppleMidi.h"
#include <FastLED.h>
#include "ctrl.h"
#include "def.h"
#if LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#endif


APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h
bool AppleMIDIConnected = false;

CRGB leds[LED_COUNT];

#if LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
#endif

Variables Var;
Function Func;


void setup()
{
  Serial.begin(115200);
  EEPROM.begin(256); // Addresses 0 - 40, - Wi-Fi Log-Pass;
  pinMode(Var.BUTTON_PIN, INPUT);
  pinMode(Var.BUTTON_PIN_1, INPUT);
  pinMode(Var.BUTTON_PIN_2, INPUT);
  pinMode(S_PIN_IN, INPUT_PULLUP);
  pinMode(S_PIN_OUT, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(A_C, INPUT_PULLUP);
  pinMode(A_C_1, INPUT_PULLUP);
  pinMode(LED_PIN_0, INPUT_PULLUP);
  delay(20);

#if LCD
  lcd.init();                      // initialize the lcd
  lcd.backlight();
#endif

  LEDS.addLeds<WS2812, LED_PIN_0, RGB>(leds, LED_COUNT);
  LEDS.setBrightness(255);

  String Log = Func.eeRead(0);
  uint8_t Log_length = Log.length() + 1;
  String Pass = Func.eeRead(20);
  uint8_t Pass_length = Pass.length() + 1;
  //char SSID[] = "SM";
  //char PASS[] = "1231231231230";
  //Func.eeWrite(SSID, 0, 3);
  //Func.eeWrite(PASS, 20, 14);
  char SSID[Log_length];
  char PASS[Pass_length];
  Log.toCharArray(SSID, Log_length);
  Pass.toCharArray(PASS, Pass_length);
  //Serial.println(SSID);
  //Serial.println(PASS);
  wifiMulti.addAP(SSID, PASS);

  uint8_t iter = 0;
  for (uint8_t i = 40; i < 121; i += 5) {
    Var.dataArray [iter] = Func.eeRead(i);
    iter++;
  }

  Var.SUPER_SONIC = Var.dataArray[0].toInt();
  Var.POTENTIOMETRES = Var.dataArray[1].toInt();
  Var.BUTTONS = Var.dataArray[2].toInt();
  Var.LATENCY = Var.dataArray[3].toInt();
  Var.NOTE_BUT_1 = Var.dataArray[4].toInt();
  Var.NOTE_BUT_2 = Var.dataArray[5].toInt();
  Var.NOTE_BUT_3 = Var.dataArray[6].toInt();
  Var.DUR_MIN = Var.dataArray[7].toInt();
  Var.DUR_MAX = Var.dataArray[8].toInt();
  Var.SMOOTHING = Var.dataArray[9].toInt();
  Var.POT_L_CTRL = Var.dataArray[10].toInt();
  Var.POT_R_CTRL = Var.dataArray[11].toInt();
  Var.SS_Ctrl = Var.dataArray[12].toInt();
  Var.LED_PROG = Var.dataArray[13].toInt();
  Var.LED_SPEED = Var.dataArray[14].toInt();
  Var.LED_BRIGHT =  Var.dataArray[15].toInt();
  Var.MIDI_Channel = Var.dataArray[16].toInt();


  if ((Var.MIDI_Channel < 1) || (Var.MIDI_Channel > 16)) {
    Var.MIDI_Channel = 1;
  }
  if ((Var.LED_PROG < 1) || (Var.LED_PROG > 16)) {
    Var.LED_PROG = 1;
  }
  if ((Var.LED_SPEED < 1) || (Var.LED_SPEED > 50)) {
    Var.LED_SPEED = 0.0001;
  }
  if ((Var.LED_BRIGHT < 1) || (Var.LED_BRIGHT > 127)) {
    Var.LED_BRIGHT = 127;
  }

}

void loop() {
#if LCD
  lcd.clear();
#endif

  static bool connected = false;
  if (wifiMulti.run() == WL_CONNECTED) {
    if (!connected) {
      //Serial.print(F("WiFi connected!\r\n"));
      AppleMIDI_setup();
      connected = true;
      Var.wifi_is_connected = true;
    }
  }
  else {
    if (connected) {
      connected = false;
      Var.wifi_is_connected = false;
    }
    delay(50);
  }

  // Listen to incoming notes
  AppleMIDI.run();
  if (Var.wifi_is_connected == true) {
    if (Var.SUPER_SONIC == 1) {
      Super_Sonic_to_Midi(Var.soft,
                          Var.anVal,
                          Var.lastAnVal,
                          0,
                          Var.MIDI_Channel,
                          Var.LATENCY,
                          Var.timer_SS,
                          Var.duration,
                          Var.DUR_MIN,
                          Var.DUR_MAX,
                          Var.SS_Ctrl,
                          Var.Value,
                          Var.softResult,
                          Var.lastSoftResult);
    }

    if (Var.POTENTIOMETRES == 1) {
      PotentiomertoMidi(Var.anVal, Var.lastAnVal, 1, A_C, Var.POT_L_CTRL, Var.MIDI_Channel,  Var.Value);
      PotentiomertoMidi(Var.anVal, Var.lastAnVal, 2, A_C_1,  Var.POT_R_CTRL, Var.MIDI_Channel, Var.Value);
    }

    Func.colorProg(Var.color,
                   Var.Value,
                   Var.x,
                   Var.LED_PROG,
                   Var.LED_SPEED,
                   Var.LED_BRIGHT,
                   Var.timer,
                   leds,
                   LED_COUNT);

    if (Var.BUTTONS == 1) {
      Buttons(Var.NOTE_BUT_2,
              Var.BUTTON_PIN,
              0,
              Var.buttState,
              Var.lastButtState,
              Var.MIDI_Channel,
              Var.Value,
              leds);
      Buttons(Var.NOTE_BUT_3,
              Var.BUTTON_PIN_1,
              1,
              Var.buttState,
              Var.lastButtState,
              Var.MIDI_Channel,
              Var.Value,
              leds);
      Buttons(Var.NOTE_BUT_1,
              Var.BUTTON_PIN_2,
              2,
              Var.buttState,
              Var.lastButtState,
              Var.MIDI_Channel,
              Var.Value,
              leds);
    }
  }


  while (Serial.available() > 0) {
    Var.c = Serial.read();
    if (Var.c == '\n') {


      switch ( Func.parse(Var.input_string, Var.VarArr) ) {
        case 0:
          Serial.println(WiFi.localIP());

          break;
        case 1: {
            Var.NOTE_BUT_1 = Func.Slider();
            Var.dataArray[4] = String(Var.NOTE_BUT_1);


          }
          break;
        case 2: {
            Var.NOTE_BUT_2 = Func.Slider();
            Var.dataArray[5] = String(Var.NOTE_BUT_2);

          }
          break;
        case 3: {
            Var.NOTE_BUT_3 = Func.Slider();
            Var.dataArray[6] = String(Var.NOTE_BUT_3);
          }
          break;
        case 4: {
            Var.MIDI_Channel = Func.Slider();
            Var.dataArray[16] = String(Var.MIDI_Channel);

          } break;
        case 5: {
            String Ex_ = Log(Var.size_);
            Func.eeWrite(Ex_, 20, 20);
#if LCD
            lcd.setCursor(0, 0);
            lcd.print(Ex_);
            lcd.setCursor(0, 1);
            lcd.print("Pass => ");
#endif
            break;
          }
        case 6: {
            String Ex_ = Log(Var.size_);
            Func.eeWrite(Ex_, 0, 20);
#if LCD
            lcd.setCursor(0, 0);
            lcd.print(Ex_);
            lcd.setCursor(0, 1);
            lcd.print("Log");
#endif

          }
          break;
        case 7: {
            Var.LATENCY = Func.Slider();
            Var.dataArray[3] = String(Var.LATENCY);

          }
          break;
        case 8:
          Wi_Fi_connect();
          break;
        case 9: {
            Var.SUPER_SONIC = 1;
            Var.dataArray[0] = String(Var.SUPER_SONIC);

          }
          break;
        case 10: {
            Var.SUPER_SONIC = 0;
            Var.dataArray[0] = String(Var.SUPER_SONIC);

          }
          break;
        case 11: {
            Var.POTENTIOMETRES = 1;
            Var.dataArray[1] = String(Var.POTENTIOMETRES);

          }
          break;
        case 12: {
            Var.POTENTIOMETRES = 0;
            Var.dataArray[1] = String(Var.POTENTIOMETRES);

          }
          break;
        case 13: {
            Var.BUTTONS = 1;
            Var.dataArray[2] = String(Var.BUTTONS);

          }
          break;
        case 14: {
            Var.BUTTONS = 0;
            Var.dataArray[2] = String(Var.BUTTONS);

          }
          break;
        case 15:
          Var.DUR_MIN = Func.Slider(); {
            Var.dataArray[7] = String(Var.DUR_MIN);

          }
          break;
        case 16:
          Var.DUR_MAX = Func.Slider(); {
            Var.dataArray[8] = String(Var.DUR_MAX);

          }
          break;
        case 17: {
            Var.SMOOTHING = Func.Slider();
            Var.dataArray[9] = String(Var.SMOOTHING);

          } break;
        case 18: {
            Var.POT_L_CTRL = Func.Slider();
            Var.dataArray[10] = String(Var.POT_L_CTRL);

          }
          break;
        case 19: {
            Var.POT_R_CTRL = Func.Slider();
            Var.dataArray[11] = String(Var.POT_R_CTRL);

          } break;
        case 20: {
            Var.SS_Ctrl = Func.Slider();
            Var.dataArray[12] = String(Var.SS_Ctrl);

          }
          break;
        case 21: {
            Var.LED_PROG = Func.Slider();
            Var.dataArray[13] = String(Var.LED_PROG);

          }
          break;
        case 22: {
            Var.LED_SPEED = Func.Slider();
            Var.dataArray[14] = String(Var.LED_SPEED);

          }
          break;
        case 23: {
            Var.LED_BRIGHT = Func.Slider();
            Var.dataArray[15] = String(Var.LED_BRIGHT);

          }
          break;
        case 24:
          Func._Send_Dump_(Var.dataArray);
#if LCD
          lcd.setCursor(0, 0);
          lcd.print("SEND D => ");
          for (int i = 0; i < 18; i++) {
            lcd.print(Var.dataArray[i]);
          }
#endif
          break;
        case 25: {
            WiFi_check();
          }
          break;
        case 26: {
            Serial.print("CONTROL_T1\r\n");
          }
          break;
        case 27: {
            Func.EEwriteParam(Var.dataArray);
          }
          break;
        case 28: {
            String Log = Func.eeRead(0);
            String Pass = Func.eeRead(20);
            uint8_t Ll = Log.length();
            uint8_t Pl = Pass.length();
            char Login[Ll + 1];
            char Password[Pl + 1];
            Log.toCharArray(Login, Ll + 1);
            Pass.toCharArray(Password, Pl + 1);
#if LCD
            lcd.setCursor(0, 0);
            lcd.print("SSID => ");
            lcd.print(Login);
            lcd.setCursor(0, 1);
            lcd.print("PASS => " );
            lcd.print(Password);
#endif
#if !LCD

            Serial.print("SSID => ");
            Serial.println(Login);
            Serial.print("PASS => " );
            Serial.println(Password);
#endif
          }
          break;
        case 250: {
#if LCD
            lcd.setCursor(0, 0);
            lcd.print(Var.input_string);
            lcd.setCursor(0, 1);
            lcd.print("Invald String");
#endif
          }

          break;
      }
      Var.input_string = "";
    }
    else {
      Var.input_string += Var.c;
    }
  }
  delay(Var.LATENCY);
}




void AppleMIDI_setup()
{
  // Create a session and wait for a remote host to connect to us
  AppleMIDI.stop();
  AppleMIDI.begin("Control T1");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

}

inline void controlChange(byte channel, byte control, byte value, uint8_t *Value) {
  AppleMIDI.controlChange(0x0B | control, 0x00 | value, channel | 0x00);
  Value[0] = value;
}

inline void Super_Sonic_to_Midi(uint8_t *soft,
                         uint8_t *anVal,
                         uint8_t *lastAnVal,
                         uint8_t ctrlNum,
                         byte MIDI_Channel,
                         uint8_t LATENCY,
                         uint32_t timer_SS,
                         uint16_t duration,
                         uint8_t DUR_MIN,
                         uint16_t DUR_MAX,
                         uint8_t SS_Ctrl,
                         uint8_t *Value,
                         uint8_t *softResult,
                         uint8_t *lastSoftResult) {

  if (millis() - timer_SS > (10 + LATENCY)) {
    digitalWrite(S_PIN_OUT, LOW);
    delayMicroseconds(2);
    digitalWrite(S_PIN_OUT, HIGH);
    delayMicroseconds(20);
    digitalWrite(S_PIN_OUT, LOW);
    duration = pulseIn(S_PIN_IN, HIGH);
    if ((duration >= DUR_MAX) || (duration <= DUR_MIN))
    {
      anVal[ctrlNum] = lastAnVal[ctrlNum];

      if ((duration > DUR_MAX) && (duration < DUR_MAX + 200))
      {
        anVal[ctrlNum] = 127;
        controlChange(MIDI_Channel, SS_Ctrl, anVal[ctrlNum], Value);
      }
    }
    else {
      anVal[ctrlNum] = map(duration, DUR_MIN, DUR_MAX, 0, 127);
      if ((anVal[ctrlNum] - lastAnVal[ctrlNum]) > 1 || (anVal[ctrlNum] - lastAnVal[ctrlNum]) < -1) {
        if (anVal[ctrlNum] != lastAnVal[ctrlNum]) {
          controlChange(MIDI_Channel, SS_Ctrl, *softResult, Value);
        }
      }


      lastAnVal[ctrlNum] = anVal[ctrlNum];
      soft[0] = lastAnVal[ctrlNum];
      soft[1] = soft[0];
      soft[2] = soft[1];
      soft[3] = soft[2];
      soft[4] = soft[3];
      soft[5] = soft[4];
      *softResult = (anVal[ctrlNum] + lastAnVal[ctrlNum] + soft[0] + soft[1] + soft[2] + soft[3] + soft[4] + soft[5]) / 8;
      if (*softResult - anVal[ctrlNum] > 20) {
        *softResult = *lastSoftResult;
      }

      timer_SS = millis();
      *lastSoftResult = *softResult;

    }
  }
}

inline void PotentiomertoMidi(uint8_t *anVal, uint8_t *lastAnVal, uint8_t ctrlNum, char anPIN, uint8_t POT_CTRL, byte MIDI_Channel, uint8_t *Value) {

  uint16_t input = analogRead(anPIN) / 4;
  if ((input >= POT_RANGE_HI) || (input <= POT_RANGE_LOW))
  {
    anVal[ctrlNum] = lastAnVal[ctrlNum];
  }
  else {
    anVal[ctrlNum] = map(input, POT_RANGE_LOW, POT_RANGE_HI, 0, 127);
    if ((anVal[ctrlNum] - lastAnVal[ctrlNum]) > 1 || (anVal[ctrlNum] - lastAnVal[ctrlNum]) < -1) {
      if (anVal[ctrlNum] != lastAnVal[ctrlNum]) {
        controlChange(MIDI_Channel, POT_CTRL, anVal[ctrlNum], Value);
      }
    }
    lastAnVal[ctrlNum] = anVal[ctrlNum];
  }
}

inline void Buttons(uint8_t NOTE_BUT, uint8_t BUTTON_PIN, uint8_t ctrlNum, bool *buttState, bool *lastButtState, byte MIDI_Channel, uint8_t *Value, CRGB *leds) {
  static uint8_t hue = 0;
  buttState[ctrlNum] = digitalRead(BUTTON_PIN);
  if (buttState[ctrlNum] != lastButtState[ctrlNum]) {
    if (buttState[ctrlNum] == HIGH) {
      AppleMIDI.noteOn(NOTE_BUT, 0x64, MIDI_Channel);
      for (uint8_t i = 0; i < 8; i++) {
        leds[i] = CHSV(Value[1], 0, 255);
      }
    }
    else {
      AppleMIDI.noteOff(NOTE_BUT, 0x00, MIDI_Channel);
    }
  }
  FastLED.show();
  lastButtState[ctrlNum] = buttState[ctrlNum];

}


void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  AppleMIDIConnected  = true;
}

void OnAppleMidiDisconnected(uint32_t ssrc) {
  AppleMIDIConnected  = false;
}

String Log(uint8_t *size_) {
  int i, ser_av; char serial_read; String readed_String_0;
  while (true)  {
    ser_av = Serial.available();
    if (ser_av > 0) {
      for (i = 0; i < ser_av; i++ ) {
        serial_read = Serial.read();
        if (serial_read == '\n') {
          break;
        }
        else if (serial_read == ' ') {
          break;
        }
        readed_String_0 += serial_read;
      }
      break;
    }
  }
  *size_ = i;
  return readed_String_0;
}

void Wi_Fi_connect() {
  //WiFi.disconnect();
 // delay(100);
  String Log = Func.eeRead(0);
  String Pass = Func.eeRead(20);
  uint8_t Ll = Log.length();
  uint8_t Pl = Pass.length();
  char Login[Ll];
  char Password[Pl];
  Log.toCharArray(Login, Ll);
  Pass.toCharArray(Password, Pl);
  wifiMulti.addAP(Login, Password);

  for (uint8_t i = 0; (wifiMulti.run() != WL_CONNECTED) && (i < 15) ; i++) {
#if LCD
    lcd.print(".");
#endif
    delay(500);
  }
  if (wifiMulti.run() == WL_CONNECTED) {
    Var.wifi_is_connected = true;
#if LCD
    lcd.setCursor(0, 2);
    lcd.print("WL_CONNECTED");
#endif
  }
  else {
    Var.wifi_is_connected = false;
#if LCD
    lcd.setCursor(0, 2);
    lcd.print("!!!! WL_CONN");

#endif
  }
}


void WiFi_check() {

  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.print("WiFi\r\n");
    Var.wifi_is_connected = true;

  }
  else {
    Serial.print("!WiFi\r\n");
    Var.wifi_is_connected = false;
  }
}
