#ifndef __SOLARNODE_HARDWARE_H__
#define __SOLARNODE_HARDWARE_H__

#define RES_R6  4750.0
#define RES_R7  10000.0

#define RES_R8  10000.0
#define RES_R9  10000.0

#define RES_R3  3570.0

#define VBATTMON_DIVIDER    (RES_R7 / (RES_R6 + RES_R7))
#define VSUPPMON_DIVIDER    (RES_R9 / (RES_R8 + RES_R9))
#define ICHARGE_DIVIDER     (RES_R3 / 900)

#endif
