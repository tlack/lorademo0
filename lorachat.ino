// This example just provide basic LoRa function test;
// Not the LoRa's farthest distance or strongest interference immunity.
// For more informations, please vist www.heltec.cn or mail to support@heltec.cn

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

#define PINGINTERVAL 5000 * 1000
#define MAXLINE 7
static int counter = 0;
static int lastSend = 0;
static int lastLine = 0;

void msg(String s) {
  Serial.println(s);
  /*
  oled.setTextXY(lastLine,0);
  lastLine++;
  if(lastLine==MAXLINE) lastLine=0;
  */
  oled.putString(s);
}
void setup() {
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer

  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);

  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 to high  
  
  Wire.begin(4,15);  
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);
  msg("Lora Chat booting");

  oled.setTextXY(1,0);
  if (!LoRa.begin(BAND)) {
    msg("LoRa failure");
    while (1) yield();
  } else {
    msg("LoRa connected");
  }

  // register the receive callback
  //LoRa.onReceive(onReceive);
  // put the radio into receive mode
  //LoRa.receive();
}

void loop() {
  // do nothing
  counter++;
  if(counter%PINGINTERVAL==0) {
    int pi=counter/PINGINTERVAL;
    oled.setTextXY(5,0);
    msg(String("sent ") + String(pi));
    LoRa.beginPacket();
    LoRa.print(String(pi));
    LoRa.endPacket();
  }
  int packetSize = LoRa.parsePacket();
  if (packetSize) onReceive(packetSize);
}

void onReceive(int packetSize) {
  char buf[1024]={0};

  // read packet
  for (int i = 0; i < packetSize; i++) {
    char ch=LoRa.read();
    buf[i]=ch;
    Serial.print(ch);
  }
  // print RSSI of packet
  oled.setTextXY(7,0);
  msg(String("recv ")+String(buf));
}

