#ifndef __SOLARNODE_CONFIG_H__
#define __SOLARNODE_CONFIG_H__

#define CONFIG_MAGIC 0x37

typedef struct {
    char magic;
    char name[16];
    char position[12];
    char repeat_count;
    uint16_t tx_interval;
    uint16_t tx_interval_low;
    uint16_t low_power_threshold;
    int8_t output_power;
    bool zombie;
    bool rfm69h;
} solarnode_config_t;

extern solarnode_config_t node_config;
void ConfigInit(void);
int ConfigSave(void);

#define FLASH_OK            0
#define FLASH_UNLOCK_FAILED 1
#define FLASH_EOP_FAILED    2

#endif
