#include <Servo.h>
#include <LiquidCrystal.h>

#define S0 8
#define S1 9

Servo SERVO1;
Servo SERVO2;

int data1, data2;

const int buttonPin = 10; // Pushbutton connected to pin 2
bool isServo1Active = true; // Tracks which servo is active
unsigned long lastDebounceTime = 0; // Tracks the last time the button was pressed
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds
int lastButtonState = HIGH; // Tracks the previous button state
int buttonState = HIGH; // Current button state

// Initialize the LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);

  // Attach servos to their respective pins
  SERVO1.attach(S0);
  SERVO2.attach(S1);

  // Initialize servos to 0 degrees
  SERVO1.write(0);
  SERVO2.write(0);
  delay(1000);

  // Set up the button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize the LCD
  lcd.begin(16, 2); // Set up the LCD's number of columns and rows
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Read the raw button state
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // If the debounce time has passed, update the button state
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // If the button is pressed (LOW), toggle the active servo
      if (buttonState == LOW) {
        isServo1Active = !isServo1Active;
        lcd.clear(); // Clear the LCD when switching servos
      }
    }
  }

  // Save the current button state for the next iteration
  lastButtonState = reading;

  // Read potentiometer values
  data1 = analogRead(A0);
  data2 = analogRead(A1);

  // Map and constrain the potentiometer values
  data1 = map(data1, 0, 1023, 0, 180);
  data2 = map(data2, 0, 1023, 0, 180);

  // Control the active servo
  if (isServo1Active) {
    SERVO1.write(data1); // Move SERVO1 based on A0
    SERVO2.write(0);     // Stop SERVO2
  } else {
    SERVO2.write(data2); // Move SERVO2 based on A1
    SERVO1.write(0);     // Stop SERVO1
  }

  // Update the LCD display
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Servo: ");
  lcd.print(isServo1Active ? "1" : "2");

  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("Angle: ");
  lcd.print(isServo1Active ? data1 : data2);
  lcd.print(" deg");

  // Print the mapped values for debugging
  Serial.print(data1);
  Serial.print("\t");
  Serial.println(data2);

  delay(100); // Small delay for stability
}
