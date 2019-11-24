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

#include "Ifx_Types.h"

#include <_PinMap/IfxAsclin_PinMap.h>
#include <Asclin/Asc/IfxAsclin_Asc.h>

extern IfxAsclin_Asc asc;

void ASC3_print_init( uint32 baudrate );
void ASC3_print_str( const char *str );
void ASC3_print_u32( uint32 data );
void ASC3_print_s32( sint32 data );
void ASC3_print_f32( float data, uint16 digits );

static inline sint32 ASC3_available( void )
{
    return IfxAsclin_Asc_getReadCount( &asc );
}

static inline uint8 ASC3_read( void )
{
    return IfxAsclin_Asc_blockingRead( &asc );
}

static inline void ASC3_read_buf( uint8 *buffer, Ifx_SizeT size )
{
    IfxAsclin_Asc_read( &asc, buffer, &size, TIME_INFINITE );
}

static inline void ASC3_write( uint8 data )
{
    IfxAsclin_Asc_blockingWrite( &asc, data );
}

static inline void ASC3_write_buf( uint8 *buffer, Ifx_SizeT size )
{
    IfxAsclin_Asc_write( &asc, buffer, &size, TIME_INFINITE );
}
