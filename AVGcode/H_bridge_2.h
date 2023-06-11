#ifndef _H_BRIDGE_2_H_
#define _H_BRIDGE_2_H_

#define port_R2	PORTK
#define pin_R2	PK7
#define ddr_R2	DDRK

#define port_L2	PORTK
#define pin_L2	PK6
#define ddr_L2	DDRK

void init_motor_2(void);
void Motor_2_set_power(signed char power);

#endif /* _H_BRIDGE_2_H_ */
