#include "crc16_xmodem.h"

#define CRC16_XMODEM_POLY 0x1021
#define CRC16_XMODEM_INIT 0x0000

static CRC16_XMODEM_Context crc_slots[CRC16_XMODEM_MAX_SLOTS];

static uint16_t UpdateCRC16_XMODEM(uint16_t crc, uint8_t byte) {
    crc ^= (uint16_t)byte << 8;
    for (uint8_t i = 0; i < 8; i++) {
        if (crc & 0x8000)
            crc = (crc << 1) ^ CRC16_XMODEM_POLY;
        else
            crc <<= 1;
    }
    return crc;
}

void CRC16_XMODEM_Init(void) {
    for (int i = 0; i < CRC16_XMODEM_MAX_SLOTS; i++) {
        crc_slots[i].crc = CRC16_XMODEM_INIT;
        crc_slots[i].active = 0;
    }
}

int CRC16_XMODEM_Begin(void) {
    for (int i = 0; i < CRC16_XMODEM_MAX_SLOTS; i++) {
        if (!crc_slots[i].active) {
            crc_slots[i].active = 1;
            crc_slots[i].crc = CRC16_XMODEM_INIT;
            return i;
        }
    }
    return -1; // No free slot
}

void CRC16_XMODEM_UpdateByte(int slot, uint8_t byte) {
    if (slot < 0 || slot >= CRC16_XMODEM_MAX_SLOTS || !crc_slots[slot].active)
        return;
    crc_slots[slot].crc = UpdateCRC16_XMODEM(crc_slots[slot].crc, byte);
}

void CRC16_XMODEM_UpdateBuffer(int slot, const uint8_t *data, size_t len) {
    if (slot < 0 || slot >= CRC16_XMODEM_MAX_SLOTS || !crc_slots[slot].active)
        return;
    for (size_t i = 0; i < len; i++) {
        crc_slots[slot].crc = UpdateCRC16_XMODEM(crc_slots[slot].crc, data[i]);
    }
}

uint16_t CRC16_XMODEM_Get(int slot) {
    if (slot < 0 || slot >= CRC16_XMODEM_MAX_SLOTS || !crc_slots[slot].active)
        return 0;
    return crc_slots[slot].crc;
}

uint16_t CRC16_XMODEM_Finish(int slot) {
    if (slot < 0 || slot >= CRC16_XMODEM_MAX_SLOTS || !crc_slots[slot].active)
        return 0;
    uint16_t result = crc_slots[slot].crc;
    crc_slots[slot].active = 0;
    crc_slots[slot].crc = CRC16_XMODEM_INIT;
    return result;
}
