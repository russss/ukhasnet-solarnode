#ifndef __SOLARNODE_UKHASNET_H__
#define __SOLARNODE_UKHASNET_H__

void ukhasnetInit(void);
char repeatPacket(uint8_t *packet, uint8_t packet_len);

extern volatile char node_state;

#define PACKET_REPEAT    0
#define PACKET_DROP      1

#define STATE_REPEATER   0
#define STATE_ZOMBIE     1

#endif
