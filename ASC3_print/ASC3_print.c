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

#include "ASC3_print.h"
#include "Ifx_IntPrioDef.h"

#include <_PinMap/IfxAsclin_PinMap.h>
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Cpu/Irq/IfxCpu_Irq.h>


static IfxAsclin_Asc asc;
#define ASC_TX_BUFFER_SIZE 64
static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE + sizeof( Ifx_Fifo ) + 8];
#define ASC_RX_BUFFER_SIZE 64
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE + sizeof( Ifx_Fifo ) + 8];

IFX_INTERRUPT( asclin3TxISR, 0, IFX_INTPRIO_ASCLIN3_TX )
{
    IfxAsclin_Asc_isrTransmit( &asc );
}
IFX_INTERRUPT( asclin3RxISR, 0, IFX_INTPRIO_ASCLIN3_RX )
{
    IfxAsclin_Asc_isrReceive( &asc );
}
IFX_INTERRUPT( asclin3ErISR, 0, IFX_INTPRIO_ASCLIN3_ER )
{
    IfxAsclin_Asc_isrError( &asc );
}

void ASC3_print_init( uint32 baudrate )
{
    // create module config
    IfxAsclin_Asc_Config ascConfig;

    IfxAsclin_Asc_initModuleConfig( &ascConfig, &MODULE_ASCLIN3 );

    // set the desired baudrate
    ascConfig.baudrate.prescaler = 1;
    ascConfig.baudrate.baudrate
        = baudrate;    // FDR values will be calculated in initModule
    // ISR priorities and interrupt target
    ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN3_TX;
    ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN3_RX;
    ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN3_ER;
    ascConfig.interrupt.typeOfService
        = IfxCpu_Irq_getTos( IfxCpu_getCoreIndex() );
    // FIFO configuration
    ascConfig.txBuffer     = &ascTxBuffer;
    ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
    ascConfig.rxBuffer     = &ascRxBuffer;
    ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;
    // pin configuration
    const IfxAsclin_Asc_Pins pins
        = { NULL,
            IfxPort_InputMode_pullUp,    // CTS pin not used
            &IfxAsclin3_RXD_P32_2_IN,
            IfxPort_InputMode_pullUp,    // Rx pin
            NULL,
            IfxPort_OutputMode_pushPull,    // RTS pin not used
            &IfxAsclin3_TX_P15_7_OUT,
            IfxPort_OutputMode_pushPull,    // Tx pin
            IfxPort_PadDriver_cmosAutomotiveSpeed1 };
    ascConfig.pins = &pins;
    IfxAsclin_Asc_initModule( &asc, &ascConfig );
}

sint32 ASC3_available( void )
{
    return IfxAsclin_Asc_getReadCount( &asc );
}

uint8 ASC3_read( void )
{
    return IfxAsclin_Asc_blockingRead( &asc );
}

void ASC3_read_buf( uint8 *buffer, uint16 size )
{
    IfxAsclin_Asc_read( &asc, buffer, &size, TIME_INFINITE );
}

void ASC3_write( uint8 data )
{
    IfxAsclin_Asc_blockingWrite( &asc, data );
}

void ASC3_write_buf( uint8 *buffer, uint16 size )
{
    IfxAsclin_Asc_write( &asc, buffer, &size, TIME_INFINITE );
}

void ASC3_print_str( const char *str )
{
    while( *str )
    {
        ASC3_write( *str++ );
    }
}

void ASC3_print_u32( uint32 data )
{
}

void ASC3_print_i32( uint32 data )
{
}

void ASC3_print_f32( float data )
{
}