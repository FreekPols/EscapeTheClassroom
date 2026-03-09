#include <Arduino.h>
#include "main.h"
#include "codes.h"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);

// Configuration
static const unsigned long SAMPLE_INTERVAL_MS = 5;     // how often we sample analog input
static const unsigned long DEBOUNCE_TIME_MS  = 30;     // debounce stability time
static const unsigned long DEFAULT_TIMEOUT_MS = 300;  // rising edge timeout

int poll_buttons() {
  int v = analogRead(14);
  int hint = analogRead(15);

  if (v < 50) {
    return 0; // no button press
  } else if (v > 200 && v < 300) {
    return 3;
  } else if (v > 450 && v < 550) {
    return 2;
  } else if (v > 700 && v < 800) {
    return 1;
  } else if (v > 950) {
    return 4;
  } else if (hint > 512) {
    return 5;
  }
  return 0;
}

// Internal state for debouncing / edge detection
static unsigned long lastSampleTime = 0;
static int stableButton = 0;       // last confirmed stable state (0 = none)
static int candidateButton = 0;    // last sampled candidate value
static unsigned long candidateSince = 0;
static unsigned long waitStart = 0; // timestamp when user started waiting for an edge

int get_debounced_button_ms(unsigned long timeoutMs) {
  unsigned long now = millis();

  // Start the waiting window if needed
  if (waitStart == 0) {
    waitStart = now;
  }

  // Check timeout first so we can abort quickly
  if (timeoutMs > 0 && (now - waitStart) >= timeoutMs) {
    // Timeout: reset wait state and return -1 to indicate timeout
    waitStart = 0;
    // Also reset transient candidate to avoid spuriously picking up old values
    candidateButton = stableButton;
    candidateSince = now;
    return -1;
  }

  // Throttle sampling to SAMPLE_INTERVAL_MS
  if ((now - lastSampleTime) < SAMPLE_INTERVAL_MS) {
    return 0; // not time to sample yet, still waiting
  }
  lastSampleTime = now;

  // Read the current raw button mapping
  int reading = poll_buttons();

  // If reading changed from last candidate, start a new candidate timer
  if (reading != candidateButton) {
    candidateButton = reading;
    candidateSince = now;
    return 0; // wait for candidate to be stable for debounce time
  }

  if (candidateButton != stableButton) {
    if ((now - candidateSince) >= DEBOUNCE_TIME_MS) {
      int previousStable = stableButton;
      stableButton = candidateButton;

      // Detect edge: we treat a press as transition from 0 -> N (no button -> some button)
      if (previousStable == 0 && stableButton != 0) {
        // Debounced press edge detected: reset the wait timer and return the detected button
        waitStart = 0; // reset waiting window for next call
        return stableButton;
      }
    }
  }

  return 0;
}

// Convenience wrapper using default timeout
int get_debounced_button() {
  return get_debounced_button_ms(DEFAULT_TIMEOUT_MS);
}

void setupHintButton() {
  pinMode(15, INPUT);
}

void setupLEDs() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void setLevelLED(int level) {
  // level is 0-4, we light up LEDs 10-13 accordingly
  digitalWrite(10, level == 1 ? HIGH : LOW);
  digitalWrite(11, level == 2 ? HIGH : LOW);
  digitalWrite(12, level == 3 ? HIGH : LOW);
  digitalWrite(13, level == 4 ? HIGH : LOW);
}

void setupBuzzer() {
  // piezo buzzer on pin 3
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
}

void buzz(int frequency = 440, int durationMs = 100) {
  if (frequency <= 0 || durationMs <= 0) {
    digitalWrite(3, LOW);
    return;
  }
  digitalWrite(2, HIGH);
  tone(3, frequency, durationMs);
  digitalWrite(2, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(14, INPUT);
  setupLEDs();
  setupBuzzer();
  setupHintButton();
  // initialize internal state explicitly
  lastSampleTime = 0;
  stableButton = poll_buttons();
  candidateButton = stableButton;
  candidateSince = millis();
  waitStart = 0;

  Wire.begin();
  Wire.beginTransmission(0x27);
  int error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2);  // initialize the lcd

  } else {
    Serial.println(": LCD not found.");
  }

    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");

    for (int i = 0; i < 5; i++) {
      setLevelLED(i);
      delay(500);
    }

    buzz();

    setLevelLED(0);
}

int n = 0;

String correctCode = "1234";
String input;

void loop() {
  int res = get_debounced_button();

  if (res > 0) {
    input += String(res);
    lcd.clear();
    lcd.print(input);
  }

  if (input.length() > 3) {
    if (input == correctCode) {
      setLevelLED(1);
      lcd.clear();
      lcd.print("Correct!");
      while (true) {} // halt
    } else {
      setLevelLED(0);
      buzz(440, 200);
      lcd.clear();
      lcd.print("Wrong code");
      input = "";
      delay(1000);
    }
  }
}