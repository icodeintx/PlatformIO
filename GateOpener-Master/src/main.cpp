/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int button = 2;
int led = 3;
bool newData = false;
char dataToSend[32] = "Gate18964";
char dataReceived[32] = "";
RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

boolean buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  Serial.println("Setup Complete");
  radio.startListening();
}

void showData() {
    if (newData == true) {
        Serial.print("  Acknowledge data ");
        Serial.println(dataReceived);
        newData = false;

      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
    }
}

void send(){
    radio.stopListening();
    radio.write( &dataToSend, sizeof(dataToSend) );
    radio.startListening();
    Serial.println("Data Sent");
    //radio.flush_rx();    
}

void loop() {
  delay(5);

  int button_value = digitalRead(button);
  if (button_value == LOW) {
    delay(200); // longer delay needed to keep from transmitting multiple times.
    send();
  }

  
  if ( radio.available()) {
    radio.read(&dataReceived, sizeof(dataReceived));
    
    if (String(dataReceived) == String("GateCopy")){
      newData = true;
      showData();
      delay(10);
    }
    
  }
}