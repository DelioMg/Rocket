/*nRF24L01  
 1 - GND
 2 - VCC-3.3V
 3 - CE-D4 
 4 - CSN-D8
 5 - SCK-D5
 6 - MOSI-D7
 7 - MISO-D6
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

const uint64_t pipeIn =  0xE8E8F0F0E1LL;

RF24 radio(D4, D8); //CE-CSN


// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  unsigned long mill;
  float altmp;
  float temp;
  float pres;
  float alt;
  float vaalt;
  float acz;
  int parachute; 
// 4+4+4+4+4+4+4+4=32bytes  
  };

PacketData data;


void setup()
{
  Serial.println(".");
  Serial.begin(115200);
  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
  Serial.println(".");
  Serial.println("Inicializado");
}

/**************************************************/

void recvData()
{  
  while ( radio.available() ) {        
    radio.read(&data, sizeof(PacketData));
    String dados = String(data.mill,3)+","+String(data.temp, 3)+","+String(data.alt, 3)+","+String(data.vaalt, 3)+","+String(data.acz, 3)+","+String(data.altmp, 3)+","+String(data.pres, 3)+","+String(data.parachute);
    Serial.println(dados); //printa os Dados
  }
}

void loop()
{
  recvData();
 
}







