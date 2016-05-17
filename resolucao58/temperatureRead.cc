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
 */


#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>

#include <display.h>
#include <nic.h>


__USING_SYS

// copied from memory_map.h
const unsigned int GPIO_BASE      = 0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;



// only leds 0 to 2 (RGB) are used





 // only leds 0 to 2 (RGB) are used

OStream cout;

//Mutex* mutexEffect[MAX_LEDS];

//Semaphore* semcout;







void sensor() {

    unsigned char count;

    Temperature_Sensor temperature;
    int val;

    unsigned char src, prot;
    unsigned int size;



    while(1)
    {
	cout << "sensor is about to measure temperature\n";
	val = temperature.sample();

	cout << " temperature is: " << val << "\n" ;
    }
}




void turn_led( int pin, bool on )
{
    int          bit     = pin % 32;
    unsigned int regData = GPIO_DATA_SET0 + ( ( pin >> 5 ) << 2 );
    unsigned int regPad  = GPIO_PAD_DIR0  + ( ( pin >> 5 ) << 2 );
    unsigned int value   = CPU::in32( regPad );
    
    if( on )
    {
        value |=  ( 1 << bit );
    }
    else
    {
        value &= !( 1 << bit );
    }
    
    CPU::out32( regPad, value );
    CPU::out32( regData, ( 1 << bit ) );
}


int main()
{
    cout << "EposMotesII app initing\n";
    
    sensor();
    
    cout << "Threads finished. EposMotesII app finishing\n";
    
    return 0;
}
