#include <string.h>
#include "ch.h"
#include "hal.h"
#include "solarnode_config.h"

typedef __IO uint16_t* flashaddr_t;
typedef uint16_t flashdata_t;

static void flashWrite(flashaddr_t address, const char* buffer, size_t size);
static bool flashUnlock(void);
static void flashWriteData(flashaddr_t address, const flashdata_t data);
static void flashErasePage(flashaddr_t page);

#define flashWaitWhileBusy() { while (FLASH->SR & FLASH_SR_BSY) {} }
#define flashLock() { FLASH->CR |= FLASH_CR_LOCK; }

__attribute__((__section__(".user_data"))) volatile flashdata_t flash_config[512];
solarnode_config_t node_config;


void ConfigInit() {
    memcpy((void *)&node_config, &flash_config, sizeof(node_config));
    if (node_config.magic != CONFIG_MAGIC) {
        node_config.magic = CONFIG_MAGIC;
        memset(node_config.name, 0, sizeof(node_config.name));
        ConfigSave();
    }
}

void ConfigSave() {
    flashErasePage((flashaddr_t)&flash_config);
    flashWrite((flashaddr_t)&flash_config, (const char*)&node_config, sizeof(node_config));
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


static void flashWrite(flashaddr_t address, const char* buffer, size_t size) {
    flashUnlock();
    flashWaitWhileBusy();
    while (size >= sizeof(flashdata_t)) {
        flashWriteData(address, *(const flashdata_t*)buffer);
        address += sizeof(flashdata_t);
        buffer += sizeof(flashdata_t);
        size -= sizeof(flashdata_t);
    }
    if (size > 0) {
        flashdata_t tmp = *(volatile flashdata_t*)address;
        memcpy(&tmp, buffer, size);
        flashWriteData(address, tmp);
    }
    flashLock();
}

static void flashWriteData(flashaddr_t address, const flashdata_t data)
{
    FLASH->CR |= FLASH_CR_PG;
    *(flashdata_t*)address = data;
    flashWaitWhileBusy();
    if ((FLASH->SR & FLASH_SR_EOP) != 0) {
        FLASH->SR |= FLASH_SR_EOP;
    }
    FLASH->CR &= ~FLASH_CR_PG;
}

static void flashErasePage(flashaddr_t page) {
    flashUnlock();
    flashWaitWhileBusy();
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = page;
    FLASH->CR |= FLASH_CR_STRT;
    chThdSleepMicroseconds(5);
    flashWaitWhileBusy();
    FLASH->SR &= ~FLASH_SR_EOP;
    flashLock();
}
