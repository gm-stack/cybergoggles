#include <Adafruit_NeoPixel.h>

#define LED_PIN 1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, LED_PIN);

#define ITG3200_WHO                0x00
#define ITG3200_SMPL        0x15
#define ITG3200_DLPF        0x16
#define ITG3200_INT_C        0x17
#define ITG3200_INT_S        0x1A
#define ITG3200_TMP_H        0x1B
#define ITG3200_TMP_L        0x1C
#define ITG3200_GX_H        0x1D
#define ITG3200_GX_L        0x1E
#define ITG3200_GY_H        0x1F
#define ITG3200_GY_L        0x20
#define ITG3200_GZ_H        0x21
#define ITG3200_GZ_L        0x22
#define ITG3200_PWR_M        0x3E
#define GYRO_ADDRESS 0x68

#include <TinyWireM.h>
#include <SendOnlySoftwareSerial.h>

//uint32_t color  = 0x000000; // Start red
uint32_t c = 0;

uint32_t h2c(uint32_t h) {
  uint8_t r,g,b;
  if (h < 256) {
    r = (255 - h);
    g = h;
    b = 0;
  } else if (h >= 256 & h < 512) {
    r = 0;
    g = (511 - h);
    b = (h - 256);
  } else if (h >= 512) {
    r = (h - 512);
    g = 0;
    b = (255 - (h - 512));
  }
  uint32_t ret = (uint32_t)b | ((uint32_t)g << 8) | ((uint32_t)r << 16);
  return ret;
}

uint32_t rgb2c(uint8_t r, uint8_t g, uint8_t b) {
  return (uint32_t)b | ((uint32_t)g << 8) | ((uint32_t)r << 16);
}

//SendOnlySoftwareSerial mySerial (3);


 
int8_t ITG3200_read(uint8_t _register)
{
    int8_t data;
    TinyWireM.beginTransmission(GYRO_ADDRESS);
    TinyWireM.write(_register);
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(GYRO_ADDRESS, 1);
    if(TinyWireM.available() > 0)
    {
        data = TinyWireM.read();
    }
        TinyWireM.endTransmission();
    return data;
}

int16_t ITG3200_read(uint8_t addressh, uint8_t addressl)
{
    int data, t_data;
 
    TinyWireM.beginTransmission(GYRO_ADDRESS);
    TinyWireM.write(addressh);
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(GYRO_ADDRESS, 1);
    if(TinyWireM.available() > 0)
    {
        t_data = TinyWireM.read();
        data = t_data << 8;
    }
    TinyWireM.beginTransmission(GYRO_ADDRESS);
    TinyWireM.write(addressl);
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(GYRO_ADDRESS, 1);
    if(TinyWireM.available() > 0)
    {
        data |= TinyWireM.read();
    }
    return data;
}

void ITG3200_write(uint8_t _register, uint8_t _data)
{
        TinyWireM.begin();
        TinyWireM.beginTransmission(GYRO_ADDRESS);
        TinyWireM.write(_register);   
        TinyWireM.write(_data);
        TinyWireM.endTransmission();
}
 
 void ITG3200_init()
{
        TinyWireM.begin();
        ITG3200_write(ITG3200_PWR_M,0x80);//send a reset to the device
        ITG3200_write(ITG3200_SMPL,0x00);//sample rate divider
        ITG3200_write(ITG3200_DLPF,0x18);//+/-2000 degrees/s (default value)
}
 
 


void setup() {
  delay(100);
  pixels.begin();
  pixels.setBrightness(30);
  TinyWireM.begin();
  ITG3200_init();
  //mySerial.begin(9600);
  delay(25);
}

void setLeftEyePixel(uint8_t pixel, uint32_t color) {
  pixel = (pixel + 15) % 16;
  pixels.setPixelColor(pixel, color);
}

void setRightEyePixel(uint8_t pixel, uint32_t color) {
  pixel = 16 - pixel;
  pixel = (pixel + 5) % 16;
  pixels.setPixelColor(pixel + 16, color);
}

#define setEyePixel(p,c) setLeftEyePixel(p,c);setRightEyePixel(p,c);

void loop() {
  
  /*int16_t x = ITG3200_read(ITG3200_GX_H,ITG3200_GX_L);
  int16_t y = ITG3200_read(ITG3200_GY_H,ITG3200_GY_L);
  int16_t z = ITG3200_read(ITG3200_GZ_H,ITG3200_GZ_L);*/
  
  
  //uint32_t color = h2c(c);  
  /*mySerial.print("gyrX: ");
  mySerial.print(x);
  mySerial.print(" gyrY: ");
  mySerial.print(y);
  mySerial.print(" gyrZ: ");
  mySerial.print(z);
  mySerial.println("");*/
  
  
  for (int i=0; i<16; i++) {
  //    pixels.setPixelColor(i, rgb2c(abs(x)/128,abs(y)/128,abs(z)/128));
    //pixels.setPixelColor(i, 0xFFFFFF);
    setEyePixel(i,h2c(((i*768/16) + c) % 768));
    //setEyePixel(i,rgb2c(c % 255,0,0));
  }
  pixels.show();
  delay(1);
  
 // for (int i=0; i<32; i++) {
 //   pixels.setPixelColor(i, 0x000000);
 // }
 // pixels.show();
 // delay(1000);

  //pixels.setPixelColor(1,0xFFFFFF);
  //pixels.setPixelColor(16,0xFFFFFF);

  //pixels.show();
    
  c += 3;
  if (c >= 768) c = 0;
}



