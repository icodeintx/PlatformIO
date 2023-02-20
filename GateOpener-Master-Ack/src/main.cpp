//-----
// SimpleTxAckPayload - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[11] = "Gate18964";
int ackData[1] = {-1}; // to hold the two values coming from the slave
bool newData = false;
int button_pin = 2;
int ledPin = 3;
//===============

void setup() {
    Serial.begin(9600);
    Serial.println("GateOpener Starting");
    pinMode(button_pin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.enableAckPayload();
    radio.setRetries(5,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}


//================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.println(dataToSend);
    if (rslt) {
        if ( radio.isAckPayloadAvailable() ) {
            int payloadsize = radio.getDynamicPayloadSize();
            radio.read(&ackData, sizeof(ackData));
            newData = true;
            //Serial.println("PayloadSize: " + String(payloadsize));
        }
        else {
            Serial.println("  Acknowledge but no data ");
        }
    }
    else {
        Serial.println("  Tx failed");
    }
    radio.flush_rx();    
    delay(200);
 }

//=================

void showData() {
    if (newData == true) {
        Serial.print("  Acknowledge data ");
        Serial.print(ackData[0]);
        Serial.println();
        newData = false;

      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
    }
}

void loop() {

  int button_value = digitalRead(button_pin);

  delay(10);
  if (button_value == LOW) {
    const char text[] = "Button Pressed";
    send();
    
  }
  showData();
}
