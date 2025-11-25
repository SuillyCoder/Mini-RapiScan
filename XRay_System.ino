#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int leftLED = 2;
const int rightLED = 3;
const int buzzer = 8;

const long buzzerInterval = 100;
const long ledInterval = 100;
int currentFreq = 800;
bool freqToggle = false;
bool ledToggle = false;

unsigned long previousLEDMillis = 0;
unsigned long previousBuzzerMillis = 0;

// Threat state
bool threatActive = false;
String threatType = "None";
int threatConfidence = 0;

// For non-blocking LCD updates
bool lcdNeedsUpdate = false;
unsigned long lastLCDUpdate = 0;
const long lcdUpdateInterval = 500; // Update LCD every 500ms when threat active

void setup() {
  // Initialize Serial (CRITICAL for Python communication!)
  Serial.begin(9600);
  Serial.println("Arduino Ready - Waiting for commands...");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X-Ray Scanner");
  lcd.setCursor(0, 1);
  lcd.print("Status: Ready");
  
  // Initialize pins
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Start with everything off
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  noTone(buzzer);
}

void loop() {
  // Check for incoming serial commands from Python
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove whitespace
    
    // Parse command
    if (command == "SAFE") {
      threatActive = false;
      threatType = "None";
      threatConfidence = 0;
      lcdNeedsUpdate = true;
      
      Serial.println("ACK:SAFE"); // Acknowledge
      
    } else if (command.startsWith("THREAT:")) {
      // Format: THREAT:Gun:69
      threatActive = true;
      
      // Parse threat type and confidence
      int firstColon = command.indexOf(':');
      int secondColon = command.indexOf(':', firstColon + 1);
      
      if (secondColon > 0) {
        threatType = command.substring(firstColon + 1, secondColon);
        threatConfidence = command.substring(secondColon + 1).toInt();
      } else {
        threatType = command.substring(firstColon + 1);
        threatConfidence = 0;
      }
      
      lcdNeedsUpdate = true;
      
      Serial.print("ACK:THREAT:");
      Serial.println(threatType);
    }
  }
  
  // Update outputs based on threat status
  if (threatActive) {
    triggerAlarm();
    blinkLEDs();
    updateLCDThreat();
  } else {
    stopAlarm();
    turnOffLEDs();
    updateLCDSafe();
  }
}

void triggerAlarm() {
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
}

void stopAlarm() {
  noTone(buzzer);
}

void blinkLEDs() {
  unsigned long currentMillis = millis();
  
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
}

void turnOffLEDs() {
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
}

void updateLCDThreat() {
  unsigned long currentMillis = millis();
  
  // Only update LCD periodically to avoid flickering
  if (lcdNeedsUpdate || (currentMillis - lastLCDUpdate >= lcdUpdateInterval)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("! THREAT ALERT !");
    
    lcd.setCursor(0, 1);
    // Truncate threat type if too long
    String displayText = threatType;
    if (displayText.length() > 11) {
      displayText = displayText.substring(0, 11);
    }
    lcd.print(displayText);
    
    // Show confidence if available
    if (threatConfidence > 0) {
      lcd.print(" ");
      lcd.print(threatConfidence);
      lcd.print("%");
    }
    
    lcdNeedsUpdate = false;
    lastLCDUpdate = currentMillis;
  }
}

void updateLCDSafe() {
  if (lcdNeedsUpdate) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X-Ray Scanner");
    lcd.setCursor(0, 1);
    lcd.print("Status: CLEAR");
    
    lcdNeedsUpdate = false;
  }
}