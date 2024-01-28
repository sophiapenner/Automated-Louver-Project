// PURPOSE OF PROGRAM
// If the air in the duct is very humid, then we want the louver to be open to expel the humid air.
// IF the air is not humid, then we want the louver closed so that air can re-circulate.

//Includes the Arduino Stepper Library
#include <Stepper.h>
#include <DHT.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;
const int stepsPerState = 200;              // play with this value - the 5 states should only span 90 degrees
const int DHT_Pin = 2;
const int HumidityThreshold1 = 70;          // play with these values - no idea what the humidity in the ducts will be
const int HumidityThreshold2 = 60;
const int HumidityThreshold3 = 50;
const int HumidityThreshold4 = 40;

// Global Variables
int humidity;
int stepperPosition;
enum State {
  STATE_1, // OPEN LOUVER
  STATE_2,
  STATE_3,
  STATE_4,
  STATE_5  // CLOSED LOUVER
};
State currentState;

DHT dht(DHT_Pin, DHT11);  // Initialize DHT sensor

// Creates two instances of stepper class
// one for recirc dampener and one for outside air dampener
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper recircStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper outsideAirStepper = Stepper(stepsPerRevolution, 4, 5, 6, 7);

void setup() {
    // Nothing to do (Stepper Library sets pins as outputs)
    // TODO 
    // homing function for stepper?
    // do it manually
    dht.begin();              // Start the sensor
    currentState = STATE_1;   // Default to open louver
    Serial.begin(9600);          // Get rid of print statements - system won't have serial monitor
    Serial.print("DHT Test");    // Could replace with LCD?
}

void loop() {

  // Read from the sensor
  humidity = dht.readHumidity();
  Serial.print("Humidity is: ");    // Get rid of these print statements - system won't have access to serial monitor
  Serial.println(humidity);         // Could replace with LCD?

  // Determine desired louver state based on humidity thresholds
  if (humidity >= HumidityThreshold1){
    currentState = STATE_1;
  } else if (humidity >= HumidityThreshold2){
    currentState = STATE_2;
  } else if (humidity >= HumidityThreshold3){
    currentState = STATE_3;
  } else if (humidity >= HumidityThreshold4){
    currentState = STATE_4;
  } else {
    currentState = STATE_5;
  }

  // Rotate the steppers to reflect the desired louver state
  recircStepper.step((stepperPosition-currentState)*stepsPerState);
  outsideAirStepper.step(-(stepperPosition-currentState)*stepsPerState);
  delay(2000);

  // Update stepperPosition
  stepperPosition = currentState;
}

