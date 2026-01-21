#include <WiFiS3.h>
#include <HCPCA9685.h>
char ssid[] = "Dog";
char pass[] = "Gooddogs";
WiFiServer server(5000);
char clientIP[] = "192.168.50.107";
char serverIP[] = "192.168.50.119";
int serverPort = 5000;
char msg[] = "MOTOR_ON";
int button_pin = A1;
WiFiClient client;
int sensorPin = A0;
int threshold = 1;
int buttonPin = A1;
int motorPin= 3;
int buttonValue = LOW; 
bool buttonPressed = false;
HCPCA9685 HCPCA9685(0x40);
void setup() {
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("."); 
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  int sensorValue = analogRead(sensorPin);
  Serial.print("Pressure sensor value: ");
  Serial.println(sensorValue);

  if(sensorValue < threshold) {
    if(client.connect(serverIP, serverPort)) {
      Serial.println("Connected to server, sending signal...");
      client.println("ON");
      client.stop(); 
    } else {
      Serial.println("Connection failed.");
    }
    delay(500);
  }

  delay(500);
  if(msg == "MOTOR_ON") {
    Serial.println("Motor command recieved!");
    HCPCA9685.Servo(0, HIGH);
    delay(3000);
    HCPCA9685.Servo(0, LOW );
    Serial.println("Motor stopped");
  }
   WiFiClient client = server.available();
  
   if(client){
     Serial.println("Client connected!");
     while(client.connected()) {
//      if(client.available()) {
        String msg = client.readStringUntil('\n');
        msg.trim();
        Serial.print("Recieved: ");
        Serial.println(msg); 
        buttonValue = analogRead(button_pin);
         if (buttonValue = HIGH){
          Serial.println("Motor On!");
          motorPin = HIGH;
          delay(5000);
          motorPin = LOW;
         }
         
    }
   }
  
}

void sendCommand(String cmd) {
  WiFiClient client;
  if(client.connect(clientIP, 5000)){
    Serial.print("Sending command: ");
    Serial.println(cmd);
    client.println(cmd);
    client.stop();
  } else {
    Serial.println("Connection failed.");
  }
}
