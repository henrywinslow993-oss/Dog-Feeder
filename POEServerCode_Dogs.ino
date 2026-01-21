#include <WiFiServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "Dog";
char pass[] = "Gooddogs";
char clientIP[] = "192.168.50.107";
WiFiServer server(5000);
int sensorPin = A0;
int pressureValue = 0;
int ledPin = 7;
int threshold = 50;
int button_pin = A1;
int motorPin= 3;
int buttonValue = LOW;
bool buttonPressed = false;



void setup() {
  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("      ");
  pinMode(ledPin, OUTPUT);

  Serial.print("Connnecting to ");
  Serial.println(ssid);
  int status = WiFi.begin(ssid, pass); 
  while(status != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    status = WiFi.status(); 
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("Server IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started waiting for client...");
}

void loop() {
  int buttonState = digitalRead(button_pin);
   WiFiClient client = server.available();
   lcd.setCursor(0,0);
   lcd.print("                 ");
   if(client){
     Serial.println("Client connected!");
     while(client.connected()) {
      if(client.available()) {
        String msg = client.readStringUntil('\n');
        msg.trim();
        Serial.print("Recieved: ");
        Serial.println(msg);
        pressureValue = analogRead(sensorPin);
        if (pressureValue > threshold) {
          lcd.setCursor(0,0);
          lcd.print("Dog at door!");
          digitalWrite(ledPin, HIGH);
          Serial.println("LED ON for 5 seconds");
          delay(5000);
          digitalWrite(ledPin, LOW);
          Serial.println("LED OFF");
       } else {  
         digitalWrite(ledPin, LOW);
         if (digitalRead(button_pin) == HIGH){                 
         delay(300);  
      }
      client.stop();
      Serial.println("Client disconnected.");
    }

    
   }
  }
 }
}
   
