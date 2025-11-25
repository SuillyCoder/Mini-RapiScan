#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int leftLED = 2;
const int rightLED = 3;
const int button = 5;
const int buzzer = 8;

const long buzzerInterval = 100;  // Oscillate every 100ms
const long ledInterval = 100;      // LED blink interval
int currentFreq = 800;
bool freqToggle = false;
bool ledToggle = false;

// Asynchronous timing
unsigned long previousLEDMillis = 0;
unsigned long previousDisplayMillis = 0;
unsigned long previousBuzzerMillis = 0;

// Track previous button state to avoid constant LCD updates
int previousButtonStatus = LOW;

void triggerAlarm(int buttonStatus) {
  if (buttonStatus == HIGH) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousBuzzerMillis >= buzzerInterval) {
      previousBuzzerMillis = currentMillis;
      
      // Alternate between two frequencies
      if (freqToggle) {
        currentFreq = 1200; // High pitch
      } else {
        currentFreq = 600;  // Low pitch
      }
      freqToggle = !freqToggle;
      
      tone(buzzer, currentFreq);
    }
  } else {
    noTone(buzzer);
  }
}

void displayMessage(int buttonStatus) {
  // Only update LCD when button state CHANGES (not every loop!)
  if (buttonStatus != previousButtonStatus) {
    lcd.clear();
    
    if (buttonStatus == HIGH) {
      lcd.setCursor(0, 0);
      lcd.print("Threat Detected!");
      lcd.setCursor(0, 1);
      lcd.print("Youre in Trouble");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("X-Ray Machine");
      lcd.setCursor(0, 1);
      lcd.print("Test: Start");
    }
    
    previousButtonStatus = buttonStatus;
  }
}

void blinkLEDs(int buttonStatus) {
  if (buttonStatus == HIGH) {
    unsigned long currentMillis = millis();
    
    // Blink LEDs alternately
    if (currentMillis - previousLEDMillis >= ledInterval) {
      previousLEDMillis = currentMillis;
      
      ledToggle = !ledToggle;
      
      if (ledToggle) {
        digitalWrite(leftLED, HIGH);
        digitalWrite(rightLED, LOW);
      } else {
        digitalWrite(leftLED, LOW);
        digitalWrite(rightLED, HIGH);
      }
    }
  } else {
    // Button not pressed - turn off both LEDs
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, LOW);
  }
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println("X-Ray Machine Test Starting...");
  
  // Initialize the LCD (only ONCE!)
  lcd.init();
  lcd.backlight();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X-Ray Machine");
  lcd.setCursor(0, 1);
  lcd.print("Test: Start");
  
  // Initialize the Pin Modes
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);  // Changed to INPUT_PULLUP!
  
  Serial.println("Setup complete!");
}

void loop() {
  int buttonStatus = digitalRead(button);
  
  // Debug: Print button status
  static int lastPrintedStatus = HIGH;
  if (buttonStatus != lastPrintedStatus) {
    Serial.print("Button: ");
    Serial.println(buttonStatus == LOW ? "PRESSED" : "RELEASED");
    lastPrintedStatus = buttonStatus;
  }
  
  triggerAlarm(buttonStatus);
  displayMessage(buttonStatus);
  blinkLEDs(buttonStatus);
}