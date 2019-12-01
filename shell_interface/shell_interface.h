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

#pragma once

#include "IfxAsclin_Asc.h"
#include "Ifx_Shell.h"
#include "Ifx_Console.h"

#define SHELL_ASC_TX_BUFFER_SIZE 2048
#define SHELL_ASC_RX_BUFFER_SIZE 512

#define SHELL_ASC_BAUDRATE 115200

typedef struct
{
    uint8 tx[SHELL_ASC_TX_BUFFER_SIZE + sizeof( Ifx_Fifo ) + 8];
    uint8 rx[SHELL_ASC_RX_BUFFER_SIZE + sizeof( Ifx_Fifo ) + 8];
} shell_asc_buffer_t;

typedef struct
{
    shell_asc_buffer_t buffer;
    IfxAsclin_Asc      asc;
    IfxStdIf_DPipe     dpipe;
    Ifx_Shell          shell;
} shell_interface_t;

void shell_init( void );
void shell_run( void );