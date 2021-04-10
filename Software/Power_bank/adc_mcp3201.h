#ifndef ADC_MCP3201_H_
#define ADC_MCP3201_H_

#include "main.h"

float mcp3201_get_voltage(void);


#define RES_DIV		(5.6 / (13.0 + 5.6))

#endif /* ADC_MCP3201_H_ */