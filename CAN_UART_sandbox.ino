/* -*-*- mode: c++ -*-*- */

#include <Arduino.h>
#include "Ticker.h"

#define BUFFER_UART_SIZE (64 * 4)

Ticker led_ticker;
#define LED PC13
#define LED_ON LOW
#define LED_OFF HIGH

auto& p2 = Serial2; // A2 (TX) A3 (RX)

static char buf2[BUFFER_UART_SIZE];
static int buf2_len = 0;

void setup()
{
    while (!p2);
    p2.begin(400000);

    pinMode(LED, OUTPUT);
    led_ticker.set_next(1000);
    digitalWrite(LED, LED_ON);
}

void loop()
{
    // UART RX
    int p2_len = p2.available();

    if (p2_len > 0) {
        digitalWrite(LED, LED_ON);
        p2_len = min(p2_len, (int)sizeof(buf2) - buf2_len);
        buf2_len += p2.readBytes(buf2 + buf2_len, p2_len);

        if (buf2_len < (int)sizeof(buf2))
            return;
    }

    // UART TX
    if (buf2_len > 0) {
        digitalWrite(LED, LED_ON);
        int len = p2.write(buf2, buf2_len);
        buf2_len -= len;
        memmove(buf2, buf2 + len, buf2_len);

        if (len > 0)
            return;
    }

    // NO OP
    digitalWrite(LED, LED_OFF);
}
