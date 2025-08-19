#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <ESP32SPISlave.h>
#include <RMTT_Libs.h>

// esp32 by Espressif Systems, version: 2.0.17
// ESP32SPISlave by hideakitai, version: 0.6.9

// SPI Pins
#define SPI_MOSI 2
#define SPI_MISO 5
#define SPI_SCLK 12
#define SPI_CS   21

// receive buffer
uint8_t rxbuf[13];
ESP32SPISlave slave;

void setup() {
  Wire.begin(27, 26);
  Serial.begin(115200);
  // Serial1.begin(1000000, SERIAL_8N1, 23, 18); // to tello
  delay(5000);
  Serial.println("*** RMTT Firmware get started ***");

  RMTT_RGB::Init();
  RMTT_RGB::SetRGB(255, 0, 0);
  Serial.println(" ** RMTT LED is available");

  slave.setDataMode(SPI_MODE0);
  slave.setQueueSize(sizeof(rxbuf));
  slave.begin(VSPI, SPI_SCLK, SPI_MISO, SPI_MOSI, SPI_CS);
  Serial.println(" ** SPI Slave interface is available");

  delay(1000);

  RMTT_RGB::SetRGB(0, 255, 0);
}

void loop() {
  size_t n = slave.pop(rxbuf, sizeof(rxbuf), 1000);
  if (n == sizeof(rxbuf)){
    uint8_t seq = rxbuf[0];
    float x, y, z;
    memcpy(&x, &rxbuf[1], 4);
    memcpy(&y, &rxbuf[5], 4);
    memcpy(&z, &rxbuf[9], 4);
    Serial.printf("SEQ=%u  X=%.3f  Y=%.3f  Z=%.3f\n", seq, x, y, z);
  }
}