// ./arduino-cli compile -b arduino:avr:micro adapt-o-matic --upload --port /dev/cu.usbmodemHIDPC1
/* ----- USB -----
 * 1          RAW *
 * 0          GND *
 * GND        RST *
 * GND        VCC *
 * 2          21  *
 * 3          20  *
 * 4          19  *
 * 5          18  *
 * 6          15  *
 * 7          14  *
 * 8          16  *
 * 9          10  *
 */
struct Step {
  int ledPin;
  int requiredPotValue;
};
const int numSteps = 11;
const Step steps[numSteps] = {
  { 14, 0 },
  { 15, 13 },
  { 18, 139 },
  { 19, 260 },
  { 20, 375 },
  { 2, 515 },
  { 3, 640 },
  { 4, 750 },
  { 5, 865 },
  { 6, 1000 },
  { 7, 1023 }
};
const int keyPin = 10;
const int potPin = A9;
void setup() {
  Serial.begin(9600);
  setPinModes();
  startupDance();
}
void setPinModes() {
  pinMode(keyPin, INPUT_PULLUP);
  for (int i = 0; i < numSteps; i++) {
    pinMode(steps[i].ledPin, OUTPUT);
  }
}
void startupDance() {
  for (int i = 0; i < numSteps; i++) {
    digitalWrite(steps[i].ledPin, HIGH);
    delay(100);
  }
  delay(300);
  for (int i = numSteps; i > 0; i--) {
    digitalWrite(steps[i].ledPin, LOW);
    delay(100);
  }
}
void loop() {
  int sensorValue = analogRead(potPin);
  int isKeyDisabled = digitalRead(keyPin);
  int activeStep = 0;
  for (int i = 0; i < numSteps; i++) {
    int requiredValue = steps[i].requiredPotValue;
    if (sensorValue >= requiredValue) {
      activeStep = i;
      if (i < numSteps - 1 || isKeyDisabled == false)  {
        digitalWrite(steps[i].ledPin, HIGH);
      } else {
        digitalWrite(steps[i].ledPin, LOW);
      }
    } else {
      digitalWrite(steps[i].ledPin, LOW);
    }
  }
  Serial.println(String(sensorValue) + "," + String(activeStep + 1) + "," + String(isKeyDisabled == false));
  delay(10);
}
