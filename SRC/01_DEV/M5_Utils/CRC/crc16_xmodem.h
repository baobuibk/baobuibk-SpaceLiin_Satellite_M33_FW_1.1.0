#ifndef CRC16_XMODEM_H
#define CRC16_XMODEM_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CRC16_XMODEM_MAX_SLOTS 4

typedef struct {
    uint16_t crc;
    uint8_t active;
} CRC16_XMODEM_Context;

void CRC16_XMODEM_Init(void);

int CRC16_XMODEM_Begin(void);

void CRC16_XMODEM_UpdateByte(int slot, uint8_t byte);

void CRC16_XMODEM_UpdateBuffer(int slot, const uint8_t *data, size_t len);

uint16_t CRC16_XMODEM_Get(int slot);

uint16_t CRC16_XMODEM_Finish(int slot);

#ifdef __cplusplus
}
#endif

#endif // CRC16_XMODEM_H
