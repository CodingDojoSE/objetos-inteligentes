/**
 * Structured LED EposMotesII Controlling
 *
 * This work is licensed under the EPOS Software License v1.0.
 * A copy of this license is available at the EPOS system source tree root.
 * Note that EPOS Software License applies to both source code and executables.
 *
 *  You should have received a copy of the EPOS Software License v1.0 along
 *  with this program. If not, see
 *  <http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0>.
 *
 * *****************************************************************************
 * 
 * 
*/

#include <utility/ostream.h>
#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <traits.h>
#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include <classes/MyClass.h>



/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * Main function entry point. It need the light sensor attached to the EPOSMotes2 board to work.
 * See this image to know how to do it, or also see the EPOSMotes2 schematics:
 * <http://i.imgur.com/COifVUs.jpg>
 * <http://epos.lisha.ufsc.br/EPOSMote+II>
 */
int main()
{
    const char* const PROGRAM_VERSION = "0.1";
    
    PRINTLN( 1, "EposMotesII app ADC_LIGHT_SENSOR initing..." ); 
    PRINTLN( 1, "Program version: " << PROGRAM_VERSION );
    
    ADC adc( ADC::SINGLE_ENDED_ADC7 );
    adc.enable();
    
    int i = 0;
    
    while(true)
    {
        // Alarm::delay( 3e6 );
        // PRINTLN( 1, "THE GREAT LIGHT VALUE IS: " << adc.sample() );
        if ( i == 100 )
            cout << "Hoi!! \n";
        
        cout << "THE GREAT LIGHT VALUE IS: " << adc.sample() << endl;
        // PRINTLN( 1, "THE WRONG GUY: " << adc.get() );
    }
    
    PRINTLN( 1, "Threads finished. EposMotesII app finishing" );
    
    return 0;
}










