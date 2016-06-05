#ifndef __SOLARNODE_RFM69_H__
#define __SOLARNODE_RFM69_H__

// If the radio is currently operating
extern volatile bool radio_ok;
// Time of the last radio reset
extern volatile systime_t radio_last_reset;

void rfm69Init(void);
void rfm69Send(const char *packet);

#endif
