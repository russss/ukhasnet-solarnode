#ifndef __SOLARNODE_UKHASNET_H__
#define __SOLARNODE_UKHASNET_H__

void ukhasnetInit(void);

extern volatile char node_state;

#define STATE_REPEATER   0
#define STATE_ZOMBIE     1

#endif
