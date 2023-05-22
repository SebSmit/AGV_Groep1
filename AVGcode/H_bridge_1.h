#ifndef _H_BRIDGE_1_H_
#define _H_BRIDGE_1_H_

#define port_R1	PORTC // 11
#define pin_R1	PC0
#define ddr_R1	DDRC

#define port_L1	PORTC // 10
#define pin_L1	PC1
#define ddr_L1	DDRC

//37 36

void init_motor_1(void);
void Motor_1_set_power(signed char power);

#endif /* _H_BRIDGE_1_H_ */
