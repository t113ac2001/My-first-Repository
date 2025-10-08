/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

// the setup function runs once when you press reset or power the board
const int rLEDPin = 3;
const int gLEDPin = 4;
const int bLEDPin = 5;
const int buttonPin = 2;

// 固定顏色模式
int mode = 0; // 0~4：紅、綠、藍、白、關
bool breathingMode = false;

// 按鈕偵測
bool lastButtonState = HIGH;
unsigned long buttonPressedTime = 0;
unsigned long buttonReleasedTime = 0;

// 呼吸燈參數
int r = 255, g = 0, b = 0;
int step = 1;
int brightness = 0;
int brightnessStep = 5;
unsigned long lastBreathUpdate = 0;
unsigned long breathDelay = 30;

void setup() {
  pinMode(rLEDPin, OUTPUT);
  pinMode(gLEDPin, OUTPUT);
  pinMode(bLEDPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  // 按下偵測
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    buttonPressedTime = millis();
  }

  // 放開偵測
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    buttonReleasedTime = millis();
    long pressDuration = buttonReleasedTime - buttonPressedTime;

    if (pressDuration < 1000) {
      // 短按：切換模式
      if (breathingMode) {
        breathingMode = false;
        mode = 0;
        turnOffLED();
      } else {
        mode = (mode + 1) % 5;
      }
    } else {
      // 長按：進入呼吸模式
      breathingMode = true;
      // 初始化呼吸燈參數
      r = 255; g = 0; b = 0;
      step = 1;
      brightness = 0;
      brightnessStep = 5;
      lastBreathUpdate = millis();
    }
  }

  lastButtonState = currentButtonState;

  if (breathingMode) {
    nonBlockingBreathing();
  } else {
    showStaticColor(mode);
  }
}

// 顯示固定顏色
void showStaticColor(int mode) {
  switch (mode) {
    case 0: setColor(255, 0, 0); break;     // 紅
    case 1: setColor(0, 255, 0); break;     // 綠
    case 2: setColor(0, 0, 255); break;     // 藍
    case 3: setColor(255, 255, 255); break; // 白
    case 4: turnOffLED(); break;            // 關
  }
}

// 非阻塞呼吸燈
void nonBlockingBreathing() {
  if (millis() - lastBreathUpdate < breathDelay) return;
  lastBreathUpdate = millis();

  // RGB 漸變
  switch (step) {
    case 1: g++; if (g >= 255) step = 2; break;
    case 2: r--; if (r <= 0) step = 3; break;
    case 3: b++; if (b >= 255) step = 4; break;
    case 4: g--; if (g <= 0) step = 5; break;
    case 5: r++; if (r >= 255) step = 6; break;
    case 6: b--; if (b <= 0) step = 1; break;
  }

  // 呼吸亮度
  brightness += brightnessStep;
  if (brightness >= 255 || brightness <= 0) brightnessStep = -brightnessStep;

  int rOut = (r * brightness) / 255;
  int gOut = (g * brightness) / 255;
  int bOut = (b * brightness) / 255;
  setColor(rOut, gOut, bOut);
}

// 設定 LED 顏色
void setColor(int r, int g, int b) {
  analogWrite(rLEDPin, r);
  analogWrite(gLEDPin, g);
  analogWrite(bLEDPin, b);
}

// 關閉 LED
void turnOffLED() {
  setColor(0, 0, 0);
}