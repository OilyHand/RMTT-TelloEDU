#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <ESP32SPISlave.h>
#include <RMTT_Libs.h>

// esp32 by Espressif Systems, version: 2.0.17
// ESP32SPISlave by hideakitai, version: 0.6.9

// SPI Pins
#define SPI_MOSI 21
#define SPI_MISO 2
#define SPI_SCLK 5
#define SPI_CS   4

// receive buffer
uint8_t rxbuf[13];
ESP32SPISlave slave;
static constexpr size_t RX_LEN = 2;   // Master write: 1 byte
static constexpr size_t TX_LEN = 2;   // Slave reply: 2 bytes (0x56, 0x78)
uint8_t rx1[RX_LEN];
uint8_t tx1[TX_LEN];

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
  memset(rx1, 0, RX_LEN);
  slave.queue(nullptr, rx1, RX_LEN);

  // // 2) 다음 트랜잭션으로 보낼 응답 준비 후 큐에 올림 (Master의 read에 제공)
  tx1[0] = 0x56; 
  tx1[1] = 0x78;
  slave.queue(tx1, nullptr, TX_LEN);

  // 3) 두 트랜잭션 완료까지 대기
  auto res = slave.wait();

  // 4) 디버그 출력
  if (res.size() == 2) {
    Serial.printf("Master->Slave: 0x%02X%02X\n", rx1[0], rx1[1]);
    // Serial.println("Slave->Master: 0x5678");
  }
}