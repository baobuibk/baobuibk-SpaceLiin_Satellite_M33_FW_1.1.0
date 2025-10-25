/************************************************
 *  @file     : cmdline.c
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "cmdline.h"
#include "fsl_debug_console.h"
#include <stdio.h>

/*External Include -----------------------------------------------------------*/
#include "Interface/SimpleShell/simple_shell.h"
#include "DateTime/date_time.h"
#include "CRC/crc16_xmodem.h"
#include "Define/define.h"
#include "OS/Task/rtos_tasks.h"
#include "Interface/RemoteCall/remote_call.h"
/*============================================================*/
/*                      Defines                               */
/*============================================================*/
#define RPMSG_SMEM_BASE  ((uint8_t *)0xA4220000U) 
#define RPMSG_SMEM_SIZE  (0x08000000U)   // 128 MB

#define RAM_TEST_BASE  ((uint8_t *)0xA4220000U)
#define RAM_TEST_END   ((uint8_t *)(0xA4220000U + 10U + 0x08000000U))

/*============================================================*/
/*              Private Function Prototypes                   */
/*============================================================*/
static void writeCharShell(char c)
{
    PUTCHAR(c); 
}

static int is_addr_valid(uint8_t *addr)
{
    return (addr >= RAM_TEST_BASE && addr <= RAM_TEST_END);
}

/*******************************************************************************
 * SimpleShell command
 ******************************************************************************/
static void CMD_a55_ping(int argc, char *argv[])
{
    Shell_WriteString("Sending ping to A55...\r\n");
    if (RemoteCall_SendCommand("a55_ping\n") == E_OK)
    {
        Shell_WriteString("Ping sent\r\n");
    }
    else
    {
        Shell_WriteString("Ping failed\r\n");
    }
}

static void CMD_a55_exec(int argc, char *argv[])
{
    if (argc < 2)
    {
        Shell_WriteString("Usage: a55_exec <script_name>\r\n");
        return;
    }

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "a55_exec %s\n", argv[1]);
    
    Shell_WriteString("Sending script command to A55...\r\n");
    if (RemoteCall_SendCommand(cmd) == E_OK)
    {
        Shell_WriteString("Script command sent\r\n");
    }
    else
    {
        Shell_WriteString("Script command failed\r\n");
    }
}

static void CMD_a55_file(int argc, char *argv[])
{
    if (argc < 3)
    {
        Shell_WriteString("Usage: a55_file <size> <filename>\r\n");
        return;
    }

    uint32_t size = atoi(argv[1]);
    const char *filename = argv[2];

    if (RemoteCall_SendFileRequest(size, filename) == E_OK)
    {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "File request sent: %s (size=%u)\r\n", 
                 filename, (unsigned int)size);
        Shell_WriteString(buffer);
    }
    else
    {
        Shell_WriteString("Failed to send file request\r\n");
    }
}

static void CMD_ram_write(int argc, char *argv[])
{
    if (argc < 3) {
        Shell_WriteString("Usage: ram_write <address> <string>\r\n");
        return;
    }

    uint32_t offset = (uint32_t)strtoul(argv[1], NULL, 0);
    uint8_t *addr = RAM_TEST_BASE + offset;

    if (!is_addr_valid(addr)) {
        Shell_WriteString("Error: Address out of allowed range (0xA4220000 - 0xA421FFFF)\r\n");
        return;
    }

    const char *data = argv[2];
    size_t len = strlen(data);

    memcpy(addr, data, len);

    char buf[128];
    snprintf(buf, sizeof(buf),
             "Written %lu bytes to address 0x%08lX\r\n",
             (unsigned long)len, (unsigned long)addr);
    Shell_WriteString(buf);
}

static void CMD_ram_read(int argc, char *argv[])
{
    char buffer[128];
    const uint32_t bytes_per_line = 16;
    uint32_t byte_count = 0;

    if (argc < 3) {
        Shell_WriteString("Usage: ram_read <address> <size>\r\n");
        return;
    }

    uint32_t offset = (uint32_t)strtoul(argv[1], NULL, 0);
    uint8_t *addr = RAM_TEST_BASE + offset;
    uint32_t size = (uint32_t)strtoul(argv[2], NULL, 0);

    if (!is_addr_valid(addr)) {
        Shell_WriteString("Error: Start address out of allowed range!\r\n");
        return;
    }

    snprintf(buffer, sizeof(buffer),
             "Reading %lu bytes from 0x%08lX\r\n",
             (unsigned long)size, (unsigned long)addr);
    Shell_WriteString(buffer);

    int crc_slot = CRC16_XMODEM_Begin();
    if (crc_slot < 0) {
        Shell_WriteString("CRC slots full!\r\n");
        return;
    }

    for (uint32_t i = 0; i < size; i += bytes_per_line) {
        uint8_t *line_addr = addr + i;
        snprintf(buffer, sizeof(buffer), "0x%08lX: ", (unsigned long)line_addr);
        char *ptr = buffer + strlen(buffer);

        for (uint32_t j = 0; j < bytes_per_line && (i + j) < size; j++) {
            uint8_t val = line_addr[j];
            snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%02X ", val);
            ptr += 3;
            byte_count++;
            CRC16_XMODEM_UpdateByte(crc_slot, val);
        }

        *ptr++ = ' ';
        *ptr++ = '|';
        for (uint32_t j = 0; j < bytes_per_line && (i + j) < size; j++) {
            uint8_t c = line_addr[j];
            *ptr++ = (c >= 32 && c <= 126) ? c : '.';
        }
        *ptr++ = '|';
        *ptr = '\0';

        Shell_WriteString(buffer);
        Shell_WriteString("\r\n");
    }

    uint16_t crc = CRC16_XMODEM_Finish(crc_slot);
    snprintf(buffer, sizeof(buffer),
             "Read complete. Counted bytes: %lu, CRC16-XMODEM: 0x%04X\r\n",
             (unsigned long)byte_count, crc);
    Shell_WriteString(buffer);
}

static void CMD_ram_fill(int argc, char *argv[])
{
    Shell_WriteString("RAM fill pattern command executed.\r\n");


}

static void CMD_ram_dump(int argc, char *argv[])
{
    char buffer[128];
    const uint32_t bytes_per_line = 16;
    uint32_t byte_count = 0;

    if (argc < 2) {
        Shell_WriteString("Usage: ram_dump <size_in_bytes>\r\n");
        return;
    }

    uint32_t size = (uint32_t)strtoul(argv[1], NULL, 0);
    if (size == 0 || size > RPMSG_SMEM_SIZE) {
        snprintf(buffer, sizeof(buffer),
                 "Invalid size. Must be between 1 and %lu bytes\r\n",
                 (unsigned long)RPMSG_SMEM_SIZE);
        Shell_WriteString(buffer);
        return;
    }

    snprintf(buffer, sizeof(buffer),
             "Dumping %lu bytes from RPMSG_SMEM (0x%08lX)\r\n",
             (unsigned long)size, (unsigned long)RPMSG_SMEM_BASE);
    Shell_WriteString(buffer);

    int crc_slot = CRC16_XMODEM_Begin();
    if (crc_slot < 0) {
        Shell_WriteString("CRC slots full!\r\n");
        return;
    }

    for (uint32_t i = 0; i < size; i += bytes_per_line) {
        uint8_t *addr = RPMSG_SMEM_BASE + i;

        snprintf(buffer, sizeof(buffer), "0x%08lX: ", (unsigned long)addr);
        char *ptr = buffer + strlen(buffer);

        for (uint32_t j = 0; j < bytes_per_line && (i + j) < size; j++) {
            uint8_t val = addr[j];
            snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%02X ", val);
            ptr += 3;
            byte_count++;
            CRC16_XMODEM_UpdateByte(crc_slot, val);
        }

        *ptr++ = ' ';
        *ptr++ = '|';
        for (uint32_t j = 0; j < bytes_per_line && (i + j) < size; j++) {
            uint8_t c = addr[j];
            *ptr++ = (c >= 32 && c <= 126) ? c : '.';
        }
        *ptr++ = '|';
        *ptr = '\0';

        Shell_WriteString(buffer);
        Shell_WriteString("\r\n");
    }

    uint16_t crc = CRC16_XMODEM_Finish(crc_slot);
    snprintf(buffer, sizeof(buffer),
             "Dump complete. Counted bytes: %lu, CRC16-XMODEM: 0x%04X\r\n",
             (unsigned long)byte_count, crc);
    Shell_WriteString(buffer);
}

static void CMD_a55_sleep(int argc, char *argv[]) 
{
    Shell_WriteString("A55 entering sleep mode...\r\n");
    /* Add code to put A55 into sleep mode here */
}

static void CMD_a55_wake(int argc, char *argv[]) 
{
    Shell_WriteString("A55 waking up from sleep mode...\r\n");
    MU1_MUA->GCR |= MU_GCR_GIR1_MASK;
}

static void CMD_cls(int argc, char *argv[]) 
{
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "\33[2J");
    Shell_WriteString(buffer);  
}

static void CMD_date(int argc, char *argv[])
{
    s_DateTime dt;
    Utils_GetRTC(&dt);
    Shell_WriteString("-> Current DateTime: ");
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%02u/%02u/20%02u %02u:%02u:%02u\r\n", dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second);
    Shell_WriteString(buffer);  

    uint8_t days;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    Utils_GetWorkingTime((uint32_t *)&days, &hours, &minutes, &seconds);
    Shell_WriteString("-> Up Time: ");
    snprintf(buffer, sizeof(buffer), "%u days, %02u:%02u:%02u\r\n", days, hours, minutes, seconds);
    Shell_WriteString(buffer);

}

static void CMD_hello(int argc, char *argv[])
{
    Shell_WriteString("Hello from i.MX93!\r\n");
}

void setup_dynamic_shell(void)
{   
    Shell_Init(writeCharShell);
    Shell_AddCommand("cls",             CMD_cls,            "Clear screen");
    Shell_AddCommand("hello",           CMD_hello,          "Print hello");
    Shell_AddCommand("date",            CMD_date,           "Print date time");
    Shell_AddCommand("a55_sleep",       CMD_a55_sleep,      "A55 enter sleep mode");
    Shell_AddCommand("a55_wake",        CMD_a55_wake,       "A55 wake up from sleep mode");
    Shell_AddCommand("ram_fill",        CMD_ram_fill,       "RAM fill pattern @ ");
    Shell_AddCommand("ram_dump",        CMD_ram_dump,       "RAM dump content @ ");
    Shell_AddCommand("ram_read",        CMD_ram_read,       "RAM read content @");
    Shell_AddCommand("ram_write",       CMD_ram_write,      "RAM write content @");
    Shell_AddCommand("a55_ping",        CMD_a55_ping,       "Send ping to A55");
    Shell_AddCommand("a55_exec",        CMD_a55_exec,       "Execute script on A55");
    Shell_AddCommand("a55_file",        CMD_a55_file,       "Send file request to A55");
}
