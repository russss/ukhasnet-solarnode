#ifndef __SOLARNODE_RFM69_H__
#define __SOLARNODE_RFM69_H__

extern volatile bool radio_ok;

void rfm69Init(void);
void rfm69Send(const char *packet);

#endif
