/* MCUXpresso autogen includes. */
#include "pin_mux.h"
#include "board.h"

/* User includes. */
#include "bsp.h"
#include "bsp_core.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Board Support main Init
 */
void BSP_Init(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    /* Init core peripheral. */
    bsp_core_init();
}