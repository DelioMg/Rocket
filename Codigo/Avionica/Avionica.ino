/*
Transmissão simples constatante de pacote de 32 bytes 

Conexões com Esp8266

nRF24L01  
 1 - GND
 2 - VCC-3.3V
 3 - CE-D4 
 4 - CSN-D8
 5 - SCK-D5
 6 - MOSI-D7
 7 - MISO-D6

BMP280
 1 - VCC - 3.3V
 2 - GND
 3 - SCL
 4 - SDA

MPU6050
 1 - VCC-3.3V
 2 - GND
 3 - SCL-D1
 4 - SDA-D2

SD
 1 - GND
 2 - VCC-3.3V
 3 - MISO-D6
 4 - MOSI-D7
 5 - SCK-D5
 6 - CS-D0

Paraquedas/Buzzer
1-D3(GPIO0)
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>   

//Configuração dos Modulos 

Adafruit_MPU6050 mpu;
const uint64_t pipeOut =  0xE8E8F0F0E1LL;
Adafruit_BMP280 bmp; // I2C

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

File myFile;
PacketData data;

RF24 radio(12, 2); //CE-CSN

//====================================================================================

//------------------------------------------------------------------------------------
// inicialização do SD
//------------------------------------------------------------------------------------

void sdini() {
  Serial.print("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado

  
  if (!SD.begin(5)) { // ESP8266 GPIO16 D0
    // programa encerrado 
    Serial.println("Falha, verifique se o cartão está presente.");
    return;                                                      
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("/data.txt", FILE_APPEND);                        
  // Escreve dados no arquivo
  if (myFile) {
    Serial.print("Gravando...");
    myFile.println("Tempo, Temperatura , Altitude, VAltitude , AceZ, Altitudemp, Pressão, Paraquedas");
    myFile.close();

  } else {
    Serial.println("Error ao abrir data.txt");

  }
}
//====================================================================================

//------------------------------------------------------------------------------------
// Grava dados em formato de string
//------------------------------------------------------------------------------------

void writeOnSD(String str) {
  myFile = SD.open("/data.txt", FILE_APPEND);

  if (myFile) {
    Serial.println("(OK)");
    myFile.println(str);
    myFile.close();

  } else {
    Serial.println("Error ao gravar em data.txt");

  }
}
//====================================================================================
float altini;
void setup() {
  
  //Configuração do NRF24L01+
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  //Chama a função que incializa o Sd
  sdini();
  //zera os dados da struct
  data.mill = 0;
  data.altmp = 0;
  data.temp= 0;
  data.pres= 0;
  data.alt= 0;
  data.vaalt= 0;
  data.acz= 0; 
  altini=0;  
  //Inicializa a serial
  Serial.begin(115200);
  //Configura o BMP280
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  //Configura o MPU6050
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X4,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X8,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_63); /* Standby time. */

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    break;
  case MPU6050_RANGE_4_G:
    break;
  case MPU6050_RANGE_8_G:
    break;
  case MPU6050_RANGE_16_G:
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    break;
  case MPU6050_RANGE_500_DEG:
    break;
  case MPU6050_RANGE_1000_DEG:
    break;
  case MPU6050_RANGE_2000_DEG:
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
  break;
  case MPU6050_BAND_184_HZ:
   break;
  case MPU6050_BAND_94_HZ:
   break;
  case MPU6050_BAND_44_HZ:
   break;
  case MPU6050_BAND_21_HZ:
   break;
  case MPU6050_BAND_10_HZ:
   break;
  case MPU6050_BAND_5_HZ:
   break;
  }

  altini=bmp.readAltitude(1017.3);

  delay(100);  
    
}


void loop() {

  //Armazena o tempo do microcontrolador.
  data.mill= millis()/1000;
  //Medições BMP280
  data.temp= bmp.readTemperature();
  data.pres= bmp.readPressure();
  data.alt= bmp.readAltitude(1017.3);
  data.vaalt= bmp.readAltitude(1017.3)-altini;
  //Medições MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  data.acz= a.acceleration.z;  

  //Grava e transmiti os dados:
  String dados = String(data.mill,3)+","+String(data.temp, 3)+","+String(data.alt, 3)+","+String(data.vaalt, 3)+","+String(data.acz, 3)+","+String(data.altmp, 3)+","+String(data.pres, 3)+","+String(data.parachute);
  writeOnSD(dados); //Grava no cartão sd
  Serial.println(dados); //printa os Dados
  radio.write(&data, sizeof(PacketData)); //transmite o pacote  
  delay(50);  
}

