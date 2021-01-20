/*
 * exti.h - Extended interrupts and events controller (EXTI) + (SYSCFG ... configure GPIO Interrupt lines)
 *
 *  Created on: 20 Nov 2020
 *      Author: tkrax
 */

#ifndef EXTI_H_
#define EXTI_H_

#include <stdint.h>
#include "bitFields.h"
#include "stm32l476xx.h"


typedef struct exti_PR1_t {
	uint32_t val0 :1;
	uint32_t val1 :1;
	uint32_t val2 :1;
	uint32_t val3 :1;
	uint32_t val4 :1;
	uint32_t val5 :1;
	uint32_t val6 :1;
	uint32_t val7 :1;
	uint32_t val8 :1;
	uint32_t val9 :1;
	uint32_t val10 :1;
	uint32_t val11 :1;
	uint32_t val12 :1;
	uint32_t val13 :1;
	uint32_t val14 :1;
	uint32_t val15 :1;
	uint32_t val16 :1;
	uint32_t val17 :1;
	uint32_t val18 :1;
	uint32_t val19 :1;
	uint32_t val20 :1;
	uint32_t val21 :1;
} exti_PR1_t;

typedef struct exti_SYSCFG_EXTICR1_t {
	uint32_t EXTI0 : 4;
	uint32_t EXTI1 : 4;
	uint32_t EXTI2 : 4;
	uint32_t EXTI3 : 4;
} exti_SYSCFG_EXTICR1_t;

typedef struct exti_SYSCFG_EXTICR2_t {
	uint32_t EXTI4 : 4;
	uint32_t EXTI5 : 4;
	uint32_t EXTI6 : 4;
	uint32_t EXTI7 : 4;
} exti_SYSCFG_EXTICR2_t;

typedef struct exti_cmsis_t {
	union {
			volatile Reg32_1b_t reg;		// for bit field access
			volatile uint32_t raw_mode;		// offset 0x0
		};
} exti_cmsis_t;

typedef struct exti_ftsr_cmsis_t {
	union {
			volatile Reg32_1b_t reg;		// for bit field access
			volatile uint16_t raw_mode;		// offset 0x0
		};
} exti_ftsr_cmsis_t;

typedef struct exti_PR1_cmsis_t {
	union {
			volatile exti_PR1_t val;		// for bit field access
			volatile uint32_t raw_mode;		// offset 0x0
		};
} exti_PR1_cmsis_t;

typedef struct syscfg_exti_cr1_cmsis_t {
	union {
			volatile exti_SYSCFG_EXTICR1_t cfg;		// for bit field access
			volatile uint32_t raw_mode;				// offset 0x0
		};
} syscfg_exti_cr1_cmsis_t;

typedef struct syscfg_exti_cr2_cmsis_t {
	union {
			volatile exti_SYSCFG_EXTICR2_t cfg;		// for bit field access
			volatile uint32_t raw_mode;				// offset 0x0
		};
} syscfg_exti_cr2_cmsis_t;

// see Reference Manual: p. 318
#define SYSCFG_BASE_ADDRESS		(0x40010000)
#define SYSCFG_EXTICR1			((syscfg_exti_cr1_cmsis_t *) (SYSCFG_BASE_ADDRESS + 0x08))
#define SYSCFG_EXTICR2			((syscfg_exti_cr2_cmsis_t *) (SYSCFG_BASE_ADDRESS + 0x0C))

// see Reference Manual: p. 401
#define EXTI_IMR1_RESET_VALUE	(0xFF820000)
#define EXTI_BASE_REGISTER		(0x40010400)
#define EXTI_IMR1_BASE_ADDRESS 	((exti_cmsis_t *) EXTI_BASE_REGISTER)
#define EXTI_FTSR1_BASE_ADDRESS ((exti_ftsr_cmsis_t *) (EXTI_BASE_REGISTER + 0x0C))
#define EXTI_PR1_BASE_ADDRESS 	((exti_PR1_cmsis_t *) (EXTI_BASE_REGISTER + 0x14))

#endif /* EXTI_H_ */
