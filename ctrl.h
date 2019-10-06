
class Variables {
  public:
    uint8_t BUTTON_PIN = 5;
    uint8_t BUTTON_PIN_1 = 18;
    uint8_t BUTTON_PIN_2 = 19;
    uint8_t LED_PIN_0 = 21;
    uint8_t S_PIN_OUT = 32;

    uint8_t MIDI_Channel = 1;
    uint8_t SS_Ctrl;
    uint8_t POT_L_CTRL;
    uint8_t POT_R_CTRL;
    uint8_t SUPER_SONIC;
    uint8_t BUTTONS;
    uint8_t POTENTIOMETRES;
    uint16_t DUR_MAX;
    uint16_t DUR_MIN;
    uint8_t SMOOTHING;
    uint8_t LATENCY;
    uint8_t NOTE_BUT_1;
    uint8_t NOTE_BUT_2;
    uint8_t NOTE_BUT_3;
    uint8_t LED_PROG;
    float LED_SPEED;
    uint8_t LED_BRIGHT;

    bool buttState[3] = {LOW, LOW, LOW};
    bool lastButtState[3] = {HIGH, HIGH, HIGH};
    uint8_t anVal[3] = {0, 0, 0};
    uint8_t lastAnVal[3] = {127, 127, 127};
    uint8_t soft[7];
    String dataArray[17];
    uint32_t timer_SS;
    uint8_t Value[2];
    uint8_t softRes, lastSoftRes;
    uint8_t *softResult = &softRes;
    uint8_t *lastSoftResult = &lastSoftRes;
    uint16_t x[4];
    uint16_t y[3];
    uint32_t timer;
    float sin0, sin1, sin2;
    uint16_t duration = 0;
    uint8_t cm;
    char c;
    uint8_t siZe = 0, EE_siZe;
    uint8_t *size_ = &siZe;
    uint8_t *EE_size = &EE_siZe;
    float color[3];
    bool wifi_is_connected;

    String input_string = "";
    const String VarArr[29] = {
      "__Get_IP__",    // 0
      "__P1_b1_on__",  // 1
      "__P2_b2_on__",  // 2
      "__P3_b3_on__",  // 3
      "__MIDI_chan__", // 4
      "__Pass__",      // 5
      "__Log__",       // 6
      "__Latency__",   // 7
      "__Refresh__",   // 8
      "__SSB_on__",    // 9
      "__SSB_off__",   // 10
      "__Pot_on__",    // 11
      "__Pot_off__",   // 12
      "__But_on__",    // 13
      "__But_off__",   // 14
      "__Dist_min__",  // 15
      "__Dist_max__",  // 16
      "__Smooth__",    // 17
      "__Pot_L__",     // 18
      "__Pot_R__",     // 19
      "__S_S_S__",     // 20
      "__L_P__",       // 21
      "__L_S__",       // 22
      "__L_B__",       // 23
      "__SEND_D__",    // 24
      "__RECIVE_D__",  // 25
      "__CHECK_DEV__", // 26
      "__SAVE__",      // 27
      "__2__"          // 28
    };


};

class Function: Variables {

  public:
    byte parse(String input_string, const String *VarArr) {
      if (input_string.equals(VarArr[0]) == true) {
        return 0;
      }
      else if (input_string.equals(VarArr[1]) == true) {
        return 1;
      }
      else if (input_string.equals(VarArr[2]) == true) {
        return 2;
      }
      else if (input_string.equals(VarArr[3]) == true) {
        return 3;
      }
      else if (input_string.equals(VarArr[4]) == true) {
        return 4;
      }
      else if (input_string.equals(VarArr[5]) == true) {
        return 5;
      }
      else if (input_string.equals(VarArr[6]) == true) {
        return 6;
      }
      else if (input_string.equals(VarArr[7]) == true) {
        return 7;
      }
      else if (input_string.equals(VarArr[8]) == true) {
        return 8;
      }
      else if (input_string.equals(VarArr[9]) == true) {
        return 9;
      }
      else if (input_string.equals(VarArr[10]) == true) {
        return 10;
      }
      else if (input_string.equals(VarArr[11]) == true) {
        return 11;
      }
      else if (input_string.equals(VarArr[12]) == true) {
        return 12;
      }
      else if (input_string.equals(VarArr[13]) == true) {
        return 13;
      }
      else if (input_string.equals(VarArr[14]) == true) {
        return 14;
      }
      else if (input_string.equals(VarArr[15]) == true) {
        return 15;
      }
      else if (input_string.equals(VarArr[16]) == true) {
        return 16;
      }
      else if (input_string.equals(VarArr[17]) == true) {
        return 17;
      }
      else if (input_string.equals(VarArr[18]) == true) {
        return 18;
      }
      else if (input_string.equals(VarArr[19]) == true) {
        return 19;
      }
      else if (input_string.equals(VarArr[20]) == true) {
        return 20;
      }
      else if (input_string.equals(VarArr[21]) == true) {
        return 21;
      }
      else if (input_string.equals(VarArr[22]) == true) {
        return 22;
      }
      else if (input_string.equals(VarArr[23]) == true) {
        return 23;
      }
      else if (input_string.equals(VarArr[24]) == true) {
        return 24;
      }
      else if (input_string.equals(VarArr[25]) == true) {
        return 25;
      }
      else if (input_string.equals(VarArr[26]) == true) {
        return 26;
      }
      else if (input_string.equals(VarArr[27]) == true) {
        return 27;
      }
      else if (input_string.equals(VarArr[28]) == true) {
        return 28;
      }
      else return 250;

    }

    float myMap(long x, long in_min, long in_max, float out_min, float out_max)
    {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


    uint16_t Slider() {
      uint8_t ser_av; char serial_read; String readed_String;
      while (true)  {
        ser_av = Serial.available();
        if (ser_av > 0) {
          for (uint8_t i = 0; i < ser_av; i++ ) {
            serial_read = Serial.read();
            readed_String += serial_read;
          }
          break;
        }
      }
      return readed_String.toInt();
    }
  private:
    long timer_circle = millis();
    long _iter;
  public:
    void colorProg(float *color,
                   uint8_t *Value,
                   uint16_t *x,
                   byte LED_PROG,
                   float LED_SPEED,
                   byte LED_BRIGHT,
                   uint32_t timer,
                   CRGB *leds,
                   uint8_t LED_COUNT) {
      float prog[3];
      float Bright_Conv;
      float Bright_Conv1;
      if (LED_PROG == 1) {
        prog[0] = 1;
        prog[1] = 0.314;
        prog[2] = 2.71;
      }
      else if (LED_PROG == 2) {
        if (millis() - timer > LED_SPEED) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          leds[abs(_iter % 8)] = CHSV(0, 255, Bright_Conv1);
          leds[(abs(1 + _iter) % 8)] = CHSV(32, 255, Bright_Conv1);
          leds[(abs(2 + _iter) % 8)] = CHSV(64, 255, Bright_Conv1);
          FastLED.show();
          _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 7, 0.001);
          timer = millis();
          fadeall(leds, LED_COUNT);
        }
      }
      else if (LED_PROG == 3) {

        if (millis() - timer > LED_SPEED) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          leds[abs(_iter % 8)] = CRGB::Black;
          leds[(abs(1 + _iter) % 8)] = CRGB::White;
          FastLED.show();
          _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 7, 0.001);
          timer = millis();
          fadeall(leds, LED_COUNT);
        }
      }
      else if (LED_PROG == 4) {

        if (millis() - timer > 1) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          if ((_iter % 127) < -64) {
            for (uint8_t i = 0; i < 8; i++) {
              leds[i] = CHSV(0, 0, 255);
            }
            FastLED.show();
            _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 0.001, 16);
            timer = millis();
            fadeall(leds, LED_COUNT);
          }
          else {
            for (uint8_t i = 0; i < 8; i++) {
              leds[i] = CHSV(0, 0, 0);
            }
            FastLED.show();
            _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 0.001, 16);
            timer = millis();
            fadeall(leds, LED_COUNT);
          }
        }

      }
      else if (LED_PROG == 5 ) {

        if (millis() - timer > 1) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          leds[0] = CHSV(0 + (_iter % 256), Value[1], Bright_Conv1);
          leds[1] = CHSV(32 + (_iter % 256), Value[1], Bright_Conv1);
          leds[2] = CHSV(64 + (_iter % 256), Value[1], Bright_Conv1);
          leds[3] = CHSV(96 + (_iter % 256), Value[1], Bright_Conv1);
          leds[4] = CHSV(128 + (_iter % 256), Value[1], Bright_Conv1);
          leds[5] = CHSV(192 + (_iter % 256), Value[1], Bright_Conv1);
          leds[6] = CHSV(224 + (_iter % 256), Value[1], Bright_Conv1);
          leds[7] = CHSV(256 + (_iter % 256), Value[1], Bright_Conv1);
          FastLED.show();
          _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 0.001, 16);
          timer = millis();
          fadeall(leds, LED_COUNT);
        }
      }
      else if (LED_PROG == 6 ) {
        if (millis() - timer > LED_SPEED) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          leds[abs(_iter % 8)] = CRGB::Blue;
          leds[(abs(1 + _iter) % 8)] = CRGB::Green;
          FastLED.show();
          _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 7, 0.001);
          timer = millis();
          fadeall(leds, LED_COUNT);
        }

      }
      else if (LED_PROG == 7 ) {
        if (millis() - timer > LED_SPEED) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);
          leds[abs(_iter % 8)] = CRGB::Yellow;
          leds[(abs(1 + _iter) % 8)] = CRGB::Red;
          leds[(abs(2 + _iter) % 8)] = CRGB::Black;
          FastLED.show();
          _iter = _iter - LED_SPEED - myMap(Value[0], 0, 127, 7, 0.001);
          timer = millis();
          fadeall(leds, LED_COUNT);
        }
      }
      else if (LED_PROG == 8 ) {
        prog[0] = 1.0;
        prog[1] = 1.5;
        prog[2] = 1.75;
      }

      if ((LED_PROG < 2) || (LED_PROG > 7)) {
        if (millis() - timer > 10 ) { // put your main code here, to run repeatedly:
          Bright_Conv = float(LED_BRIGHT);
          Bright_Conv1 = myMap(Bright_Conv, 0.0, 127.0, 0.00, 255);
          Value[1] = myMap(Value[0], 255, 0, 0, 255);


          color[0] = (sin(sqrt(x[0] * prog[0] * LED_SPEED * 0.1))) + 1;
          color[1] = (sin(sqrt(x[0] * prog[1] * LED_SPEED * 0.1))) + 1;
          color[2] = (sin(sqrt(x[0] * prog[2] * LED_SPEED * 0.1))) + 1;
          x[1] = color[0] * 127;
          x[2] = color[1] * 127;
          x[3] = color[2] * 127;
          leds[0] = CHSV(x[1], Value[1], Bright_Conv1);
          leds[1] = CHSV(x[2], Value[1], Bright_Conv1);
          leds[2] = CHSV(x[3], Value[1], Bright_Conv1);
          leds[3] = CHSV(x[1] + 15, Value[1], Bright_Conv1);
          leds[4] = CHSV(x[2] + 10, Value[1], Bright_Conv1);
          leds[5] = CHSV(x[3] + 15, Value[1], Bright_Conv1);
          leds[6] = CHSV(x[1] + 30, Value[1], Bright_Conv1);
          leds[7] = CHSV(x[2] + 35, Value[1], Bright_Conv1);
          FastLED.show();
          x[0]++;
          timer = millis();
          fadeall(leds, LED_COUNT);
        }
      }
    }

    void eeWrite(String Data, uint16_t address, uint8_t size_) {
      delay(20);
      size_ = Data.length();
      for (uint8_t i = 0; i < size_; i++) {
        EEPROM.write(address + i, Data[i]);
        if (Data[i] == '\n')
          break;
      }
      EEPROM.write(address + size_, '\0');
      EEPROM.commit();
      delay(100);
    }

    String eeRead(uint8_t address) {
      String out = EEPROM.readString(address);
      return out;
    }


    void _Send_Dump_(String * dataArray) {
      uint8_t iter = 0;
      Serial.print(" \r\n");
      for (uint8_t i = 40; i < 121; i += 5) {
        dataArray[iter] = eeRead(i);
        Serial.println(dataArray[iter]);
        iter++;
      }
    }

    void EEwriteParam(String *dataArray) {
      uint8_t iter = 0;
      String temp;
      for (uint8_t i = 40; i < 121; i += 5) {
        temp = dataArray[iter];
        eeWrite(temp, i, 5);
        iter++;
      }
      EEPROM.commit();
    }

    void EEreadParam(String *dataArray) {
      uint8_t iter = 0;
      for (uint8_t i = 40; i < 121; i += 5) {
        dataArray[iter] = eeRead(i);
        iter++;
      }
    }
private:

      void fadeall(CRGB * leds, uint8_t LED_COUNT) {
        for (int i = 0; i < LED_COUNT; i++) {
          leds[i].nscale8(250);
        }
      }
      void circleLED(CRGB * leds, uint8_t LED_COUNT) {

        leds[0] = CHSV(0 + (_iter % 256), 255, 255);
        leds[1] = CHSV(32 + (_iter % 256), 255, 255);
        leds[2] = CHSV(64 + (_iter % 256), 255, 255);
        leds[3] = CHSV(96 + (_iter % 256), 255, 255);
        leds[4] = CHSV(128 + (_iter % 256), 255, 255);
        leds[5] = CHSV(160 + (_iter % 256), 255, 255);
        leds[6] = CHSV(192 + (_iter % 256), 255, 255);
        leds[7] = CHSV(224 + (_iter % 256), 255, 255);
        _iter++;
      }

    };
