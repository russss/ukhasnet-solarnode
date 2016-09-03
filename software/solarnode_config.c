#include <string.h>
#include "ch.h"
#include "hal.h"
#include "solarnode_config.h"

typedef __IO uint16_t* flashaddr_t;
typedef uint16_t flashdata_t;

static int flashWrite(flashaddr_t address, const char* buffer, size_t size);
static bool flashUnlock(void);
static int flashWriteData(flashaddr_t address, const flashdata_t data);
static int flashErasePage(flashaddr_t page);

#define flashWaitWhileBusy() { while (FLASH->SR & FLASH_SR_BSY) {} }
#define flashLock() { FLASH->CR |= FLASH_CR_LOCK; }

// Allocate a flash page to hold user configuration (512 two-byte words = 1kB)
// The .user_data section is carved out in the linker script.
__attribute__((__section__(".user_data"))) volatile flashdata_t flash_config[512];

solarnode_config_t node_config;

// Default configuration if there is no user config in flash:
// This struct is const so gets put in flash rather than RAM
const solarnode_config_t default_config = {
    CONFIG_MAGIC,
    "",
    "",
    3,
    60,
    300,
    3900,
    13,
    0,
    0
};

void ConfigInit() {
    memcpy((void *)&node_config, &flash_config, sizeof(node_config));
    if (node_config.magic != CONFIG_MAGIC) {
        memcpy((void *)&node_config, (void *)&default_config, sizeof(node_config));
        ConfigSave();
    }
}

int ConfigSave() {
    int ret;
    ret = flashErasePage((flashaddr_t)&flash_config);
    if (ret != FLASH_OK) {
        return ret;
    }
    ret = flashWrite((flashaddr_t)&flash_config, (const char*)&node_config, sizeof(node_config));
    if (ret != FLASH_OK) {
        return 10 + ret;
    }
    if (memcmp((const void *)flash_config, (const void *)&node_config, sizeof(solarnode_config_t)) != 0) {
        return 20;
    }
    return 0;
}

static bool flashUnlock()
{
    if (!(FLASH->CR & FLASH_CR_LOCK))
        return TRUE;

    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    if (FLASH->CR & FLASH_CR_LOCK)
        return FALSE;
    return TRUE;
}


static int flashWrite(flashaddr_t address, const char* buffer, size_t size) {
    int ret;
    if (!flashUnlock()) {
        return FLASH_UNLOCK_FAILED;
    }
    flashWaitWhileBusy();
    while (size >= sizeof(flashdata_t)) {
        ret = flashWriteData(address, *(const flashdata_t*)buffer);
        if (ret != FLASH_OK) {
            flashLock();
            return ret;
        }
        address++;
        buffer += sizeof(flashdata_t);
        size -= sizeof(flashdata_t);
    }
    if (size > 0) {
        flashdata_t tmp = *(volatile flashdata_t*)address;
        memcpy(&tmp, buffer, size);
        ret = flashWriteData(address, tmp);
        if (ret != FLASH_OK) {
            flashLock();
            return ret;
        }
    }
    flashLock();
    return FLASH_OK;
}

static int flashWriteData(flashaddr_t address, const flashdata_t data)
{
    FLASH->CR |= FLASH_CR_PG;
    *(flashdata_t*)address = data;
    flashWaitWhileBusy();
    if ((FLASH->SR & FLASH_SR_EOP) != 0) {
        FLASH->SR |= FLASH_SR_EOP;
    } else {
        FLASH->CR &= ~FLASH_CR_PG;
        return FLASH_EOP_FAILED;
    }
    FLASH->CR &= ~FLASH_CR_PG;
    return FLASH_OK;
}

static int flashErasePage(flashaddr_t page) {
    if (!flashUnlock()) {
        return FLASH_UNLOCK_FAILED;
    }
    flashWaitWhileBusy();
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = page;
    FLASH->CR |= FLASH_CR_STRT;
    chThdSleepMicroseconds(50);
    flashWaitWhileBusy();
    if ((FLASH->SR & FLASH_SR_EOP) != 0) {
        FLASH->SR &= ~FLASH_SR_EOP;
    } else {
        FLASH->CR &= ~FLASH_CR_PER;
        flashLock();
        return FLASH_EOP_FAILED;
    }
    FLASH->CR &= ~FLASH_CR_PER;
    flashLock();
    return FLASH_OK;
}
