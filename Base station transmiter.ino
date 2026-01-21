#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
Servo servo;
// Define pins
#define CE_PIN 9
#define CSN_PIN 8
#define SWITCH_PIN 2
#define servo_pin 3
#define SWITCH_PIN2 4
// Create RF24 radio object
RF24 radio(CE_PIN, CSN_PIN);

//address through which two modules communicate.
const byte address[6] = "00001";

int ledState = HIGH;          // the current state of the output pin
int buttonState;              // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin
int button2State;              // the current reading from the input pin
int lastButton2State = HIGH;

unsigned long lastDebounceTime = 0;      // the last time the output pin was toggled
const unsigned long debounceDelay = 50;  // the debounce time

void setup() {
  servo.attach(servo_pin);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN2, INPUT_PULLUP);
  pinMode(servo_pin, OUTPUT);
  servo.write(90);
  Serial.begin(9600);

  radio.begin();

  //set the address
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);

  //Set module as receiver
  radio.startListening();
}
void loop() {
  // Debounce the switch
  int reading = digitalRead(SWITCH_PIN);
   int reading2 = digitalRead(SWITCH_PIN2);
  if (reading != lastButtonState) {
    //lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
         
        radio.stopListening();  // Stop listening to send data
         const char text = '3';
        radio.write(&text, sizeof(text));
        radio.startListening();  // Resume listening

        Serial.print("Data sent: ");
        Serial.println('3');
      }
    }
     if (reading2 != button2State) {
      button2State = reading2;

      // only toggle the LED if the new button state is HIGH
      if (button2State == HIGH) {
        
        
        radio.stopListening();  // Stop listening to send data
         const char text = '7';
        radio.write(&text, sizeof(text));
        radio.startListening();  // Resume listening

        Serial.print("Data sent: ");
        Serial.println('7');
      }
     }
  lastButtonState = reading;
  lastButton2State = reading2;

  if (radio.available()) {
    char receivedState;
    radio.read(&receivedState, sizeof(receivedState));
      if (receivedState == '7'){
        servo.write(0);
        delay(7000);
        servo.write(90);
        delay(200);
      }
      else if(receivedState == '3'){
        servo.write(0);
        delay(2000);
        servo.write(90);
        delay(200);
      }
    
    Serial.print("Data received: ");
    Serial.println(receivedState);
    }
   
  }
}
