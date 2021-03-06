/*LICENSE *****************************************************************************************
 *
 * Software License Agreement
 *
 * Copyright (R) 2012 Microchip Technology Inc.  All rights reserved. Microchip licenses to you the
 * right to use, modify, copy and distribute Software only when embedded on a Microchip 
 * microcontroller or digital signal controller, which is integrated into your product or third 
 * party product (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for additional information 
 * regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR 
 * IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR 
 * OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT  
 * LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS  
 * OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY  
 * THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * ***********************************************************************************************/

#include <stdint.h>
#include "p33SMPS_irq.h"

// Device header file
#include <xc.h>


/*!p33GS_irq.c
 * ************************************************************************************************
 * Summary:
 * Driver file for the dsPIC33ExxGS Interrupt Controller SFRs
 *
 * Description:
 * The embedded Interrupt Controller offers some configuration options. This additional
 * driver file contains initialization routines for all majority required settings for
 * generic settings like uninterruptable exceptions (traps) and overall interrupt behavior.
 * ***********************************************************************************************/


/*!gsirq_init_irq
 * ************************************************************************************************
 * Summary:
 * Initializes the basic interrupt controller configuration
 *
 * Parameters:
 *	regCORCON	= holds the register value for the interrupt configuration registers (INTCONx)
 *
 * Description:
 * Basic options like global enable/disable, interrupt vector table selection and traps configurations
 * are set here.
 * ***********************************************************************************************/

inline volatile uint16_t gsirq_init_irq(uint16_t regINTCON1, uint16_t regINTCON2, uint16_t regINTCON3)
{

    volatile uint16_t reg_buf=0, fres=0;

	reg_buf = (regINTCON1 & REG_INTCON1_WRITE_BIT_MSK);	// Status-bits will be masked out, flag bits will be reset
	INTCON1 = reg_buf;                                  // Read register contents
	if((INTCON1 & REG_INTCON1_WRITE_BIT_MSK) == reg_buf) fres = 1; // Compare 

	reg_buf = (regINTCON2 & REG_INTCON2_WRITE_BIT_MSK);	// Status-bits will be masked out
	INTCON2 = reg_buf;	
	if((INTCON2 & REG_INTCON2_VALID_BIT_MSK) == reg_buf) fres &= 1;

	reg_buf = (regINTCON3 & REG_INTCON3_WRITE_BIT_MSK);	// Status-bits will be masked out
	INTCON3 = reg_buf;	
	if((INTCON3 & REG_INTCON3_VALID_BIT_MSK) == reg_buf) fres &= 1;

	return(fres);
	
}

/*!gsirq_get_current_irq_priority_level
 * ************************************************************************************************
 * Summary:
 * Reads the current interrupt priority level from SR and CORCON register
 *
 * Parameters:
 *	(none)
 *
 * Returns:
 *  uint16_t (0...15)
 * 
 * Description:
 * Read the currently active interrupt priority level from the SR and CORCON register and returns
 * a unsigned integer number
 * ***********************************************************************************************/

inline volatile uint16_t gsirq_get_current_irq_priority_level(void)
{

    volatile uint16_t reg_buf=0;

	reg_buf = (CORCONbits.IPL3);
    reg_buf <<= 3;
    reg_buf |= (SRbits.IPL);

	return(reg_buf);
	
}

/*!gsirq_init_soft_traps
 * ************************************************************************************************
 * Summary:
 * Initializes the soft traps for accumulator overflow options
 *
 * Parameters:
 *	(none)
 *
 * Returns:
 *  unsigned int (0...15)
 * 
 * Description:
 * Initializes the soft traps for accumulator overflow of accumulator a and/or accumulator b
 * and the catastrophic overflow event trap. (on/off options) 
 * ***********************************************************************************************/

inline volatile uint16_t gsirq_init_soft_traps(uint16_t accumulator_a_overflow_trap_enable, uint16_t accumulator_b_overflow_trap_enable, 
                    uint16_t accumulator_catastrophic_overflow_trap_enable)
{
    
    _OVATE = accumulator_a_overflow_trap_enable; // Enable Accumulator A Overflow Trap Enable bit
    _OVBTE = accumulator_b_overflow_trap_enable; // Enable Accumulator B Overflow Trap Enable bit
    _COVTE = accumulator_catastrophic_overflow_trap_enable; // Enable Catastrophic Overflow Trap Enable bit

    return (1);
}

