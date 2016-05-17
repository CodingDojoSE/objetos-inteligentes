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

__USING_SYS

// copied from memory_map.h
const unsigned int GPIO_BASE      = 0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;

const unsigned int MSG_LEN = 5;

// only leds 0 to 2 (RGB) are used
const unsigned int MAX_LEDS = 3;

//4294967294 //4.294.967.294
unsigned int effectDelay  = 1e5;
bool         finishThread = false;

unsigned int power[ MAX_LEDS ]; // only leds 0 to 2 (RGB) are used

OStream cout;

//Mutex* mutexEffect[MAX_LEDS];
bool effect[ MAX_LEDS ];

//Semaphore* semcout;
NIC * nic;

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

int PWMLeds()
{
    // semcout->p();
    cout << "Thread PWM LEDs initing\n";
    
    // semcout->v();
    
    int led[ 5 ]; // not all leds are actually used. Only the RGB ones (the first 3)
    led[ 0 ] = 10;
    led[ 1 ] = 9;
    led[ 2 ] = 11;
    led[ 3 ] = 23;
    led[ 4 ] = 8;
    
    unsigned int i;
    unsigned int cont = 0;
    
    for( i = 0; i < MAX_LEDS; i++ )
    {
        power[ i ] = 1; // leds start at 1% of the power (just to show app is running)
    
    }
    
    // PWM
    while( !finishThread )
    {
        cont == 99 ? cont = 0 : cont++;
        
        for( i = 0; i < MAX_LEDS; i++ )
        {
            turn_led( led[ i ], cont < power[ i ] );
        }
        
        /* if (cont==0 && power[i]>0)
              turn_led(led[i],true);
           else
              if (cont==power[i])
                 turn_led(led[i],false);   */
    }
    
    cout << "Thread PWM LEDs finishing\n";
    return 0;
}

void InterpretMessage( char msg[ MSG_LEN ] )
{
    unsigned int led, pow, i;
    
    switch( msg[ 0 ] )
    {
        case 'R':
            led = 0;
            break;
        case 'G':
            led = 1;
            break;
        case 'B':
            led = 2;
            break;
        case 'A':
            led = MAX_LEDS;
            break;
        default:
            // semcout->p();
            cout << "Invalid led value '" << msg[ 0 ] << "' on position 0\n";
            // semcout->v();
            return;
    }
    
    //led = ((unsigned int)msg[0])-48; // int based on ascii
    if( msg[ 1 ] == 'E' )
    {
        //effect
        if( msg[ 2 ] == 'N' ) // turn ON effect
        {
            if( led < MAX_LEDS ) // one led
            {// mutexEffect[led]->unlock();
                effect[ led ] = true;
            }
            else // all leds at once
            {
                for( i = 0; i < MAX_LEDS; i++ )
                {
                    // mutexEffect[i]->unlock();
                    effect[ i ] = true;
                }
            }
            // semcout->p();
            
            cout << "Effect[" << led << "]=ON\n";
            // semcout->v();
        }
        else if( msg[ 2 ] == 'F' ) // turn OFF effect
        {
            if( led < MAX_LEDS )
            {
                // mutexEffect[led]->lock();
                effect[ led ] = false;
            }
            else
            {
                for( i = 0; i < MAX_LEDS; i++ )
                {
                    // mutexEffect[i]->lock();
                    effect[ i ] = false;
                }
            }
            // semcout->p();
            cout << "Effect[" << led << "]=OFF\n";
            // semcout->v();
        }
        else   // set effect delay
        {
            unsigned int tempDelay = ( (unsigned int) msg[ 2 ] ) - 48;
            tempDelay  *= 10 ^ ( ( (unsigned int) msg[ 3 ] ) - 48 );
            effectDelay = tempDelay;
            // semcout->p();
            cout << "Delay=" << effectDelay << "\n";
            // semcout->v();
        }
    }
    else if( msg[ 1 ] == '0' || msg[ 1 ] == '1' )
    {
        // fixed power
        pow  = 100 * ( ( (unsigned int) msg[ 1 ] ) - 48 );
        pow +=  10 * ( ( (unsigned int) msg[ 2 ] ) - 48 );
        pow +=   1 * ( ( (unsigned int) msg[ 3 ] ) - 48 );
        
        if( pow > 100 )
        {
            pow = 100;
        }
        
        if( led < MAX_LEDS ) // only one led
        {
            power[ led ] = pow;
        }
        else // all leds at once
        {
            for( i = 0; i < MAX_LEDS; i++ )
            {
                power[ i ] = pow;
            }
        }
        // semcout->p();
        cout << "Power[" << led << "]=" << pow << "\n";
        // semcout->v();
    }
    else
    {
        // semcout->p();
        cout << "Invalid value '" << msg[ 1 ] << "' on position 1\n";
        // semcout->v();
    }
}

void SendMessageToNIC( char msg[ MSG_LEN ] )
{
    int r;
    
    while( ( r = nic->send( NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof( msg ) ) ) == 0 ) // != 11 ?
    {// semcout->p();
        cout << "Send failed " << r << "\n";
        // semcout->v();
    }
    
    // semcout->p();
    cout << "Message sent\n";
    
    // semcout->v();
}

int ReceiveCommandUART()
{
    unsigned int i;
    char         msg[ MSG_LEN ]; //[DATA_SIZE];
    
    // semcout->p();
    cout << "Thread UART initing\n";
    // semcout->v();
    UART * uart = new UART();
    
    while( !finishThread )
    {
        do
        {
            msg[ 0 ] = uart->get();
        }
        while( msg[ 0 ] != ':' ); // messages start with ":"
        i = 0;
        
        while( ( msg[ i - 1 ] != '\n' ) && ( i < MSG_LEN ) )
        {
            msg[ i++ ] = uart->get();
        }
        memset( msg + i, 0x00, MSG_LEN - i );
        // message received.
        SendMessageToNIC( msg );
        InterpretMessage( msg );
    }
    
    cout << "Thread UART finishing\n";
    return 0;
}

int ReceiveCommandNIC()
{
    // semcout->p();
    cout << "Thread NIC initing\n";
    // semcout->v();
    
    char msg[ MSG_LEN ];
    
    NIC::Protocol prot;
    NIC::Address  src;
    
    while( !finishThread )
    {
        while( !( nic->receive( &src, &prot, &msg, sizeof( msg ) ) > 0 ) )
        {
            // semcout->p();
            //cout << ".";
            // semcout->v();
        }
        // semcout->p();
        cout << "\nMessage received: " << msg << "\n";
        // semcout->v();
        InterpretMessage( msg );
    }
    
    cout << "Thread NIC finishing\n";
    return 0;
}

int LEDPowerEffect( unsigned int i )
{
    // semcout->p();
    cout << "Thread Effect " << i << " initing\n";
    // semcout->v();
    //unsigned int i = MAX_LEDS;
    unsigned int j;
    int          pow;
    
    while( !finishThread )
    {
        if( effect[ i ] )
        {
            //for (i=0; i<=MAX_LEDS; i++) {
            // semcout->p();
            cout << "Incresing power of led[" << i << "]\n";
            
            // semcout->v();
            for( pow = 0; pow <= 100; pow++ )
            {
                // mutexEffect[i]->lock();
                //if (i<MAX_LEDS) // only one led
                power[ i ] = pow;
                //else // all leds at once
                //   for (j=0; j<MAX_LEDS; j++)
                //      power[j]=pow;
                // mutexEffect[i]->unlock();
                Alarm::delay( effectDelay );
            }
        }
        
        if( effect[ i ] )
        {
            //cout << "Decresing power of led[" << i << "]\n";
            for( pow = 100; pow >= 0; pow-- )
            {
                // mutexEffect[i]->lock();
                //if (i<MAX_LEDS)
                power[ i ] = pow;
                //else
                //   for (j=0; j<MAX_LEDS; j++)
                //      power[j]=pow;
                // mutexEffect[i]->unlock();
                Alarm::delay( effectDelay );
            }
            //}
        }
        else
        {
            Alarm::delay( effectDelay );
        }
    }
    
    cout << "Thread Effect " << i << " finishing\n";
    return 0;
}

int main()
{
    cout << "EposMotesII app initing\n";
    
    unsigned int i;
    
    Alarm::delay( 100 );
    
    for( i = 0; i < MAX_LEDS; i++ )
    {
        //   mutexEffect[i]= new Mutex();
        //   mutexEffect[i]->lock(); // effect starts OFF (blocked)
        effect[ i ] = true;
    }
    
    nic = new NIC();
    // semcout = new Semaphore(1);
    
    Thread * thrdPWM;
    Thread * thrdUART;
    Thread * thrdNIC;
    Thread * thrdEffect[ MAX_LEDS ];
    
    thrdPWM  = new Thread( &PWMLeds );
    thrdUART = new Thread( &ReceiveCommandUART );
    
    //thrdNIC  = new Thread(&ReceiveCommandNIC);
    for( i = 0; i < MAX_LEDS; i++ )
    {
        thrdEffect[ i ] = new Thread( &LEDPowerEffect, (unsigned int) i );
        Alarm::delay( 5e6 );
    }
    
    // semcout->p();
    cout << "Waiting for threads to finish\n";
    // semcout->v();
    int status_thrdPWM  = thrdPWM->join();
    int status_thrdUART = thrdUART->join();
    
    //int status_thrdNIC  = thrdNIC->join();
    for( i = 0; i < MAX_LEDS; i++ )
    {
        int status_thrdEffect = thrdEffect[ i ]->join();
    }
    
    cout << "Threads finished. EposMotesII app finishing\n";
    
    return 0;
}
