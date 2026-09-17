#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define LED 2
#define LED_INVERT true
#define SERIAL_BAUD 115200
#define CH_TIME 140
#define PKT_RATE 5
#define PKT_TIME 1

const short channels[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };

const size_t CH_COUNT = sizeof(channels) / sizeof(channels[0]);

int ch_index { 0 };
volatile int packet_rate { 0 };
int attack_counter { 0 };
unsigned long update_time { 0 };
unsigned long ch_time { 0 };

void sniffer(uint8_t *buf, uint16_t len) {
  if (!buf || len < 28) return;

  byte pkt_type = buf[12];

  if (pkt_type == 0xA0 || pkt_type == 0xC0) {
    ++packet_rate;
  }
}

void attack_started() {
  digitalWrite(LED, !LED_INVERT);
}

void attack_stopped() {
  digitalWrite(LED, LED_INVERT);
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LED_INVERT);

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);
  wifi_set_promiscuous_rx_cb(sniffer);
  wifi_set_channel(channels[0]);
  wifi_promiscuous_enable(true);
}

void loop() {
  unsigned long current_time = millis();

  if (current_time - update_time >= (unsigned long)CH_COUNT * CH_TIME) {
    update_time = current_time;

    if (packet_rate >= PKT_RATE) {
      ++attack_counter;
    } else {
      if (attack_counter >= PKT_TIME) attack_stopped();
      attack_counter = 0;
    }

    if (attack_counter == PKT_TIME) {
      attack_started();
    }

    packet_rate = 0;
  }

  if (CH_COUNT > 1 && current_time - ch_time >= CH_TIME) {
    ch_time = current_time;

    ch_index = (ch_index + 1) % CH_COUNT;
    short ch = channels[ch_index];

    wifi_set_channel(ch);
  }
}
