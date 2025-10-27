/************************************************
 *  @file     : simple_shell.c
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/
#include <string.h>
#include <stdlib.h>
#include <stddef.h> 
#include "simple_shell.h"
#ifdef DEBUG_SHELL
#include "stdio.h"
#endif

/*============================================================*/
/*                      Defines                               */
/*============================================================*/
static const char LINE_BREAK = '\n';
static const char CARRIAGE_RETURN = '\r';
static const char BACKSPACE = 0x7F;

/*============================================================*/
/*                   Private Variables                        */
/*============================================================*/
static char Shell_Buffer[SHELL_BUFFER_SIZE];
static int Shell_Pos = 0;
static SShell_Command_t Shell_Commands[SHELL_MAX_COMMANDS];
static int Shell_CommandCount = 0;

/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/
void (*Shell_WriteChar)(char c);

/*============================================================*/
/*              Private Function Prototypes                   */
/*============================================================*/
static void CMD_Help(int argc, char *argv[]);

/*============================================================*/
/*                Function Implementations                    */
/*============================================================*/
static int Compare_Commands(const void *a, const void *b) {
    return strcmp(((SShell_Command_t *)a)->name, ((SShell_Command_t *)b)->name);
}

void Shell_Init(void (*write_char_func)(char c)) {
    Shell_WriteChar = write_char_func;
    Shell_AddCommand("help", CMD_Help, "Show help [command]");
    Shell_WriteString("|---------Simple Shell---------|");
    Shell_WriteString(SHELL_NEWLINE);
    Shell_WriteString(SHELL_INITATION);
}

void Shell_AddCommand(const char *name, SShell_CmdFunc func, const char *usage) {
    if (Shell_CommandCount < SHELL_MAX_COMMANDS) {
        Shell_Commands[Shell_CommandCount].name = name;
        Shell_Commands[Shell_CommandCount].func = func;
        Shell_Commands[Shell_CommandCount].usage = usage;
        Shell_CommandCount++;
        qsort(Shell_Commands, Shell_CommandCount, sizeof(SShell_Command_t), Compare_Commands);
    } else {
        Shell_WriteString("Error: Max commands reached");
        Shell_WriteString(SHELL_NEWLINE);
    }
}

static void Shell_Process(char *input) {
    char buffer_copy[SHELL_BUFFER_SIZE];
    strncpy(buffer_copy, input, SHELL_BUFFER_SIZE);
    buffer_copy[SHELL_BUFFER_SIZE - 1] = '\0';

    char *args[SHELL_MAX_ARGS + 1];
    int argc = 0;

    args[argc] = strtok(buffer_copy, " ");
    while (args[argc] != NULL && argc < SHELL_MAX_ARGS - 1) {
        args[++argc] = strtok(NULL, " ");
    }
    args[argc] = NULL;

    if (argc > 0) {
        SShell_Command_t key = {args[0], NULL, NULL};
        SShell_Command_t *found = bsearch(&key, Shell_Commands,
        									Shell_CommandCount,
											sizeof(SShell_Command_t), Compare_Commands);
        if (found) {
            found->func(argc, args);
            Shell_WriteString(SHELL_NEWLINE);
            Shell_WriteString(SHELL_INITATION);
        } else {
            Shell_WriteString("Command not found");
            Shell_WriteString(SHELL_NEWLINE);
            Shell_WriteString(SHELL_INITATION);
        }
    } else {
        Shell_WriteString(SHELL_NEWLINE);
        Shell_WriteString(SHELL_INITATION);
    }
}

void Shell_ReceiveChar(char ch) {
#ifdef DEBUG_SHELL
    char hex[6];
    snprintf(hex, sizeof(hex), "0x%02X ", ch);
    Shell_WriteString(hex);
#endif

    if (ch == LINE_BREAK || ch == CARRIAGE_RETURN) {
        Shell_Buffer[Shell_Pos] = '\0';
        if (Shell_Pos > 0) {
            Shell_WriteString(SHELL_NEWLINE);
            Shell_Process(Shell_Buffer);
            Shell_Pos = 0;
        } else {
            Shell_WriteString(SHELL_NEWLINE);
            Shell_WriteString(SHELL_INITATION);
        }
    } else if (ch == BACKSPACE) {
        if (Shell_Pos > 0) {
            Shell_Pos--;
            Shell_Buffer[Shell_Pos] = '\0';
            Shell_WriteChar('\b');
            Shell_WriteChar(' ');
            Shell_WriteChar('\b');
        }
    } else if (Shell_Pos < SHELL_BUFFER_SIZE - 1) {
        Shell_Buffer[Shell_Pos++] = ch;
        Shell_WriteChar(ch);
    }

#ifdef DEBUG_SHELL
    // Ctrl+D (0x04) to show buffer content
    if (ch == 0x04) {
        Shell_WriteString(SHELL_NEWLINE);
        Shell_WriteString("Buffer: [");
        for (int i = 0; i < Shell_Pos; i++) {
            char buf[4];
            snprintf(buf, sizeof(buf), "%c", Shell_Buffer[i]);
            Shell_WriteString(buf);
        }
        Shell_WriteString("]");
        Shell_WriteString(SHELL_NEWLINE);
        Shell_WriteString(SHELL_INITATION);
    }
#endif
}

void Shell_WriteString(const char *str) {
    while (*str) {
        Shell_WriteChar(*str++);
    }
}

/*************************************************
 *                   Command                     *
 *************************************************/
static void CMD_Help(int argc, char *argv[]) {
    if (argc == 1) {
        size_t max_len = 0;
        for (int i = 0; i < Shell_CommandCount; i++) {
            size_t len = strlen(Shell_Commands[i].name);
            if (len > max_len) {
                max_len = len;
            }
        }
        Shell_WriteString("Available commands:");
        Shell_WriteString(SHELL_NEWLINE);
        for (int i = 0; i < Shell_CommandCount; i++) {
            Shell_WriteString(Shell_Commands[i].name);
            size_t len = strlen(Shell_Commands[i].name);
            for (size_t j = len; j < max_len + 2; j++) {
                Shell_WriteString(" ");
            }
            Shell_WriteString(": ");
            Shell_WriteString(Shell_Commands[i].usage);
            Shell_WriteString(SHELL_NEWLINE);
        }
    } else if (argc == 2) {
        SShell_Command_t key = {argv[1], NULL, NULL};
        SShell_Command_t *found = bsearch(&key, Shell_Commands, Shell_CommandCount,
                                         sizeof(SShell_Command_t), Compare_Commands);
        if (found) {
            Shell_WriteString(found->usage);
            Shell_WriteString(SHELL_NEWLINE);
        } else {
            Shell_WriteString("Command not found");
            Shell_WriteString(SHELL_NEWLINE);
        }
    } else {
        Shell_WriteString("Usage: help [command]");
        Shell_WriteString(SHELL_NEWLINE);
    }
}


