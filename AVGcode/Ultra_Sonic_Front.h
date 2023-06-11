#ifndef _Ultra_Sonic_H_
#define _Ultra_Sonic_H_

#define port_tri_F	PORTA//22
#define pin_tri_F	PA0
#define ddr_tri_F	DDRA

#define port_ech_F	PINK//A11
#define pin_ech_F	PK3
#define ddr_ech_F	DDRK

#define port_tri_L	PORTA//24
#define pin_tri_L	PA2
#define ddr_tri_L	DDRA

#define port_ech_L	PINK//A10
#define pin_ech_L	PK2
#define ddr_ech_L	DDRK

#define port_tri_R	PORTA//26
#define pin_tri_R	PA4
#define ddr_tri_R	DDRA

#define port_ech_R	PINK//A9
#define pin_ech_R	PK1
#define ddr_ech_R	DDRK

void init_SonicSensors(void);

int GetDistanceFront(void);
void TriggerPulseFront (void);
int GetDistanceLeft(void);
void TriggerPulseLeft (void);
int GetDistanceRight(void);
void TriggerPulseRight (void);

#endif /* _Ultra_Sonic_H_ */
