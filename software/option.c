#include "ch.h"
#include "hal.h"
#include "option.h"

void unlock_option_bytes(void) {
    /* (1) Wait till no operation is on going */
    /* (2) Check that the Flash is unlocked */
    /* (3) Perform unlock sequence for Flash */
    /* (4) Check that the Option Bytes are unlocked */
    /* (5) Perform unlock sequence for Option Bytes */
    while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
    {
     /* For robust implementation, add here time-out management */
    }
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) /* (2) */
    {
     FLASH->KEYR = FLASH_KEY1; /* (3) */
     FLASH->KEYR = FLASH_KEY2;
    }
    if ((FLASH->CR & FLASH_CR_OPTWRE) == 0) /* (4) */
    {
     FLASH->OPTKEYR = FLASH_OPTKEY1; /* (5) */
     FLASH->OPTKEYR = FLASH_OPTKEY2;
    }
}

bool set_option_byte(int *opt_addr, int data) {
    /* (1) Set the PG bit in the FLASH_CR register to enable programming */
    /* (2) Perform the data write */
    /* (3) Wait until the BSY bit is reset in the FLASH_SR register */
    /* (4) Check the EOP flag in the FLASH_SR register */
    /* (5) Clear the EOP flag by software by writing it at 1 */
    /* (6) Reset the PG Bit to disable programming */
    FLASH->CR |= FLASH_CR_OPTPG; /* (1) */
    *opt_addr = data; /* (2) */
    while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (3) */
    {
     /* For robust implementation, add here time-out management */
    }
    if ((FLASH->SR & FLASH_SR_EOP) != 0) {
     FLASH->SR |= FLASH_SR_EOP;
    } else {
     return FALSE;
    }
    FLASH->CR &= ~FLASH_CR_OPTPG;
    return TRUE;
}
