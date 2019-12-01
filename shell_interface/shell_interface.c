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

#include "shell_interface.h"
#include "IfxAsclin_PinMap.h"
#include "Ifx_IntPrioDef.h"
#include "IfxCpu_Irq.h"
#include "Assert.h"

shell_interface_t shell_interface;

IFX_INTERRUPT( asclin3_tx_isr, 0, IFX_INTPRIO_ASCLIN3_TX )
{
    IfxStdIf_DPipe_onTransmit( &shell_interface.dpipe );
}
IFX_INTERRUPT( asclin3_rx_isr, 0, IFX_INTPRIO_ASCLIN3_RX )
{
    IfxStdIf_DPipe_onReceive( &shell_interface.dpipe );
}
IFX_INTERRUPT( asclin3_er_isr, 0, IFX_INTPRIO_ASCLIN3_ER )
{
    IfxStdIf_DPipe_onError( &shell_interface.dpipe );
}

const Ifx_Shell_Command shell_commands[] = { {
                                                 "help",
                                                 SHELL_HELP_DESCRIPTION_TEXT,
                                                 &shell_interface.shell,
                                                 &Ifx_Shell_showHelp,
                                             },
                                             IFX_SHELL_COMMAND_LIST_END };

static void shell_asc_init( void )
{
    // create module config
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig( &ascConfig, &MODULE_ASCLIN3 );

    // set the desired baudrate
    ascConfig.baudrate.prescaler = 1;
    ascConfig.baudrate.baudrate
        = SHELL_ASC_BAUDRATE;    // FDR values will be calculated in initModule

    // ISR priorities and interrupt target
    ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN3_TX;
    ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN3_RX;
    ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN3_ER;
    ascConfig.interrupt.typeOfService
        = IfxCpu_Irq_getTos( IfxCpu_getCoreIndex() );

    // FIFO configuration
    ascConfig.txBuffer     = shell_interface.buffer.tx;
    ascConfig.txBufferSize = SHELL_ASC_TX_BUFFER_SIZE;
    ascConfig.rxBuffer     = shell_interface.buffer.rx;
    ascConfig.rxBufferSize = SHELL_ASC_RX_BUFFER_SIZE;

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
    IfxAsclin_Asc_initModule( &shell_interface.asc, &ascConfig );
}

void shell_init( void )
{
    shell_asc_init();

    IfxAsclin_Asc_stdIfDPipeInit( &shell_interface.dpipe,
                                  &shell_interface.asc );
    Ifx_Console_init( &shell_interface.dpipe );
    Ifx_Assert_setStandardIo( &shell_interface.dpipe );

    Ifx_Shell_Config shell_config;
    Ifx_Shell_initConfig( &shell_config );
    shell_config.standardIo     = &shell_interface.dpipe;
    shell_config.commandList[0] = &shell_commands[0];

    Ifx_Shell_init( &shell_interface.shell, &shell_config );

    Ifx_Console_print( "Enter 'help' to see the available commands" ENDL
                           IFX_CFG_SHELL_PROMPT );
}

void shell_run( void )
{
    Ifx_Shell_process( &shell_interface.shell );
}