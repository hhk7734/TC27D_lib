# TC27D libraries

## Setup

```bash
cd <path to workspace>/0_Src/AppSw/Tricore
```

```bash
git clone https://github.com/hhk7734/TC27D_lib.git
```

When cloning git, you have to clean project.

If not, a building may fail because the new directory path wasn't registered in Makefile.

## Usage

### ASC3 example

```c
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ASC3_print.h"

IfxCpu_syncEvent cpuSyncEvent = 0;

int core0_main( void )
{
    IfxCpu_enableInterrupts();
    /*
     * !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog( IfxScuWdt_getCpuWatchdogPassword() );
    IfxScuWdt_disableSafetyWatchdog( IfxScuWdt_getSafetyWatchdogPassword() );

    /* Cpu sync event wait*/
    IfxCpu_emitEvent( &cpuSyncEvent );
    IfxCpu_waitEvent( &cpuSyncEvent, 1 );

    ASC3_print_init( 9600 );

    ASC3_print_str( "Hello World\r\n" );

    while( 1 )
    {
        // Echo
        if( ASC3_available() > 0 )
        {
            ASC3_write( ASC3_read() );
        }
    }
    return ( 1 );
}
```
