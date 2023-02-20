// SimpleRxAckPayload- the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataCheck[11] = "Gate18964";  // value to use for validation
char dataReceived[11]; // type to hold incoming data, this must match dataToSend in the TX
int ackData[1] = {18964}; // the two values to be sent to the master
bool newData = false;
int ledPin = 3;
//==============

void setup() {

    Serial.begin(9600);
    Serial.println("SimpleRxAckPayload Starting");
    pinMode(ledPin, OUTPUT);

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.enableAckPayload();
    radio.startListening();
    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        
        newData = true;
    }
}

//================

void showData() {
    if (newData == true) {
        if (String(dataReceived) == String(dataCheck)) {
          Serial.print("Data received ");
          Serial.println(dataReceived);
          Serial.print(" ackPayload sent ");
          Serial.println(ackData[0]);

          radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time

          newData = false;

          // Turn on LED for 1 second
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
        }
    }
}


void loop() {
  
    delay(10);
    getData();
    showData();
}
