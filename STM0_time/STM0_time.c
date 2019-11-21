/*
 * MIT License
 * Copyright (c) 2019 Hyeonki Hong <hhk7734@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "STM0_time.h"
#include "Ifx_IntPrioDef.h"

#include <Stm/Std/IfxStm.h>

Ifx_STM *            stmSfr;
IfxStm_CompareConfig stmConfig;

volatile uint32 micros_tick = 0;
volatile uint32 millis_tick = 0;

IFX_INTERRUPT( stm0Sr0ISR, 0, IFX_INTPRIO_STM0_SR0 )
{
    IfxStm_clearCompareFlag( stmSfr, stmConfig.comparator );
    IfxStm_increaseCompare( stmSfr, stmConfig.comparator, stmConfig.ticks );
    ++micros_tick;
    if( ( micros_tick % 1000 ) == 0 )
    {
        ++millis_tick;
    }
}

void STM0_time_init( void )
{
    stmSfr = &MODULE_STM0;
    IfxStm_initCompareConfig( &stmConfig );
    // configure to generate interrupt every 10 us
    sint32 ticks              = IfxStm_getTicksFromMicroseconds( stmSfr, 1 );
    stmConfig.ticks           = ticks;
    stmConfig.triggerPriority = IFX_INTPRIO_STM0_SR0;
    stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
    IfxStm_initCompare( stmSfr, &stmConfig );
}


uint32 STM0_micros( void )
{
    return micros_tick;
}

uint32 STM0_millis( void )
{
    return millis_tick;
}
void STM0_delay_us( uint32 us )
{
    if( us > 0 )
    {
        uint32 start  = STM0_micros();
        uint32 target = start + us;
        if( target > start )
        {
            while( STM0_micros() < target )
            {
            }
        }
        else
        {
            while( STM0_micros() > start || STM0_micros() < target )
            {
            }
        }
    }
}

void STM0_delay_ms( uint32 ms )
{
    if( ms < 10 )
    {
        STM0_delay_us( ms * 1000 );
    }
    else
    {
        uint32 start  = STM0_millis();
        uint32 target = start + ms;
        if( target > start )
        {
            while( STM0_millis() < target )
            {
            }
        }
        else
        {
            while( STM0_millis() > start || STM0_millis() < target )
            {
            }
        }
    }
}
