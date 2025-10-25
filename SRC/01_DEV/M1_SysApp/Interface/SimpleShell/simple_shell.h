/************************************************
 *  @file     : simple_shell.h
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#ifndef simple_shell_H
#define simple_shell_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================*/
/*                        Includes                            */
/*============================================================*/

/*============================================================*/
/*                         Defines                            */
/*============================================================*/
#define SHELL_NEWLINE "\r\n"
#define SHELL_INITATION "m33@IMX93:~ $ "
#define SHELL_MAX_ARGS 4
#define SHELL_MAX_COMMANDS 16
#define SHELL_BUFFER_SIZE 32
//#define DEBUG_SHELL

/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/
typedef void (*SShell_CmdFunc)(int argc, char *argv[]);

typedef struct {
    const char *name;         // Command name
    SShell_CmdFunc func;       //
    const char *usage;        //
} SShell_Command_t;

/*============================================================*/
/*                Function Prototypes                         */
/*============================================================*/
void Shell_AddCommand(const char *name, SShell_CmdFunc func, const char *usage);

void Shell_Init(void (*write_char_func)(char c)) ;
void Shell_ReceiveChar(char ch);
void Shell_WriteString(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* simple_shell_H */