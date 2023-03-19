/* -*-*- mode: c++ -*-*- */

#include <Arduino.h>
#include "eXoCAN.h"
#include "Ticker.h"

CanTxMsg TxMessage;

#define LED PC13
#define LED_ON LOW
#define LED_OFF HIGH
Ticker led_ticker;
Ticker usb_ticker;

// tx frame setup #1
int txMsgID = 0x069;
int rxMsgID = 0x005;   // needed for rx filtering

//  ****** uncomment the following for the second stm32f103 board ******
// int txMsgID = 0x005;
// int rxMsgID = 0x069;   // only needed if using a filter

int id, fltIdx;
uint8_t can_rx_data[8];
int can_rx_data_len = 0;

// 11b IDs, 250k bit rate, no transceiver chip, portA pins 11,12, no external resistor
eXoCAN* can = nullptr;

void canISR() // get CAN bus frame passed by a filter into fifo0
{
    can_rx_data_len = can->receive(id, fltIdx, can_rx_data);  // empties fifo0 so that another another rx interrupt can take place
}

void setup()
{
    // 11b IDs, 250k bit rate, no transceiver chip, portA pins 11,12, no external resistor
    // can = new eXoCAN(STD_ID_LEN, BR250K, PORTB_8_9_WIRE_PULLUP);
    // can->attachInterrupt(canISR);
    // can->filterMask16Init(0, rxMsgID, 0x7ff);   // filter bank 0, filter 0: allow ID = rxMsgID, mask = 0x7ff (must match)

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LED_ON);
    led_ticker.set_next(2000);

    Serial.begin(9600);
    usb_ticker.set_next(2000);
}

void loop()
{
    // byte buf[64];
    auto& pc = Serial;
    // int buf_len = pc.readBytes(buf, sizeof(buf));
    // bool io_op = buf_len > 0;

    // if (can_rx_data_len > 0) {
    //     pc.write(can_rx_data, can_rx_data_len);
    //     can_rx_data_len = 0;
    // }

    bool io_op = false;
    while (pc.available()) {
        pc.write(pc.read());
        io_op = true;
    }  

    // can->transmit(txMsgID, buf, buf_len);
    // if (can_rx_data_len > 0) {
    //     pc.write(can_rx_data, can_rx_data_len);
    //     can_rx_data_len = 0;
    //     digitalWrite(LED, LED_ON);
    // }

    if (io_op) {
        led_ticker.set_next(200);
        digitalWrite(LED, LED_ON);
    } else if (led_ticker.mark()) {
        led_ticker.set_next(5000);
        digitalWrite(LED, LED_OFF);
    }

    if (usb_ticker.mark()) {
        usb_ticker.set_next(2000);
        pc.write("tick\n");
    }
}
