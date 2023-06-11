#ifndef _H_BRIDGE_1_H_
#define _H_BRIDGE_1_H_

#define port_R1	PORTK
#define pin_R1	PK5
#define ddr_R1	DDRK

#define port_L1	PORTK
#define pin_L1	PK4
#define ddr_L1	DDRK

//37 36

void init_motor_1(void);
void Motor_1_set_power(signed char power);

#endif /* _H_BRIDGE_1_H_ */
