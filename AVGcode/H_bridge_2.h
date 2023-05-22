#ifndef _H_BRIDGE_2_H_
#define _H_BRIDGE_2_H_

#define port_R2	PORTC
#define pin_R2	PC5 // 32
#define ddr_R2	DDRC

#define port_L2	PORTC
#define pin_L2	PC4 // 15
#define ddr_L2	DDRC // 33

void init_motor_2(void);
void Motor_2_set_power(signed char power);

#endif /* _H_BRIDGE_2_H_ */
