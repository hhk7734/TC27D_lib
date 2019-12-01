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

### Shell on Cpu0

```c
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "shell_interface.h"

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

    shell_init();

    while( 1 )
    {
        shell_run();
    }
    return ( 1 );
}
```
