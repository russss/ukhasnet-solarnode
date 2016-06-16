#ifndef __SOLARNODE_ONEWIRE_H__
#define __SOLARNODE_ONEWIRE_H__

void oneWireInit(void);
uint8_t oneWireTempRead(float* value);

#define OW_SUCCESS      0
#define OW_ERROR        1
#define OW_NO_DEVICES   2
#define OW_CRC_ERROR    3

#endif
