#ifndef __SOLARNODE_RFM69_H__
#define __SOLARNODE_RFM69_H__

#define MAX_MESSAGE 64

// If the radio is currently operating
extern volatile bool rfm69_ok;
// Time of the last radio reset
extern volatile systime_t rfm69_last_reset;
// RSSI threshold
extern volatile char rfm69_rssi_threshold;

void rfm69Init(void);
void rfm69Send(const char *packet);

#endif
