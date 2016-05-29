#ifndef __SOLARNODE_CONFIG_H__
#define __SOLARNODE_CONFIG_H__

#define CONFIG_MAGIC 0x37

typedef struct {
    char magic;
    char name[16];
} solarnode_config_t;

extern solarnode_config_t node_config;
void ConfigInit(void);
void ConfigSave(void);

#endif
