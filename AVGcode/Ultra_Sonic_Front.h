#ifndef _Ultra_Sonic_H_
#define _Ultra_Sonic_H_

#define port_tri	PORTL
#define pin_tri		PL6
#define ddr_tri		DDRL

#define port_ech	PINF
#define pin_ech		PF7
#define ddr_ech		DDRF

void init_SonicFront(void);
int GetDistanceFront(void);
void TriggerPulse (void);

#endif /* _Ultra_Sonic_H_ */
