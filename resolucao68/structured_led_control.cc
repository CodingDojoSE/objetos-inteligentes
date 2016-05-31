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
 * Program notes
 *
 * The code is not working using the default scheduler provided by the EPOSMoteII platform.
 * We are still deciding which scheduling algorithm and quantum to use, but for test purposes
 * we believe that it's okay to use Round_Robin. The problem is that while using the Round_Robin scheduler,
 * you get the feel that the LED is blinking, which should not be available to the user.
 *
 * How to change the scheduler:
 * In the EPOSMoteII directory, edit the file include/traits.h
 *
 * The scheduling algorithm should be located inside the templatized Traits<Thread> structure.
 * Also, you can change the quantum which each thread will be given by the scheduler.
 *
 * pq tem os 3 coloridos ali
 * e tem um outro q fica do lado
 * qnd ele ta ligado eh pq deu merda
 * hmmm
 * ql escalonador vc ta usando?
 * vc mudou o escalonador?
 *
 * vai na pasta include
 * do epos isso
 * vai em traits
 * vai ter um Traits<Thread>
 * la dentro, muda pra Round Robin
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


//bool useSensor = false;

int defaultPower( int power )
{
    return power;
}

int sensorCalculatePower( int power )
{
    return 0;
}

/**
 *  Base IO adress. GPIO Base is the base address of all IO operations.
 *  copied from memory_map.h
 */
const unsigned int GPIO_BASE = 0x80000000;

/**
 * DATA_SET is just so magical I can't express my thoughts into words.
 */
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;

/**
 * PAD_DIR0 contains the base address which contains the registers' values.
 */
const unsigned int GPIO_PAD_DIR0 = GPIO_BASE + 0x00;

/**
 * MAX_MESSAGE_LENGTH_ALLOWED is the maximum length of the message which can be sent and received via UART/NIC
 */
const unsigned int MAX_MESSAGE_LENGTH_ALLOWED = 5;

/**
 * Only LEDs 0 to 2 (RGB) are used
 */
const unsigned int MAX_LEDS_ALLOWED_TO_BE_USED = 3;

/**
 * Please save yourself the trouble and don't read the documentation bellow. If you do
 * and still don't get it like me, then please increase the following counter to warn
 * future code revisor and save them the trouble. You have been warned.
 * count = 2
 * //4294967294 //4.294.967.294
 */
unsigned int g_effectDelay = 1e5;

/**
 * Used to stop all running threads which are running throw an while true, to perform busy wait
 * from this board devices.
 */
bool g_finishThread = false;

/**
 * Power of the lads.
 */
unsigned int power[ MAX_LEDS_ALLOWED_TO_BE_USED ]; // only leds 0 to 2 (RGB) are used

//Mutex* mutexEffect[MAX_LEDS_ALLOWED_TO_BE_USED];
bool g_effect[ MAX_LEDS_ALLOWED_TO_BE_USED ];

/**
 * Semaphore* semcout;
 */
NIC* g_nic;


/**
 * Functions prototypes to allow they to be declared after the main function.
 */
int myClassObjectTest();
void configureTheLedsEffects();
void turn_led( int pin, bool on );
int ReceiveCommandUART();
int ReceiveCommandNIC();
int LEDPowerEffect();
void PWMInterrupt();


/**
 * Main function entry point.
 */
int main()
{
  
  const char* const PROGRAM_VERSION = "2.1";
    
    PRINTLN( 1, "EposMotesII app initing..." ); 
    PRINTLN( 1, "Program version: " << PROGRAM_VERSION );
    
#if defined DEBUG
    myClassObjectTest();
#endif
   
    PRINTLN( 1, "To send commands to the EPOSMotes2 by USB device, use:" );
    PRINTLN( 1, "echo :R100 > /dev/ttyUSB0" );
    PRINTLN( 1, "Try also :REN, :BEN, :GEN or :AEN" ); 
    
    // To creates a interrupt by stealing one Operation System Interrupt. This theft has know side
    // affects until now, but it could be causing the 'traits.h' debug level info, to crash this
    // program while running. The the file at 'file/trace_debug_info_error.txt' on this project
    // for more information about it.
    // The values tested as the interruption time to cause the LEDs to show a human visual blink
    // when the LEDs power are set to lower values as 1, instead of 100 are 250. Note that not all
    // colors showed blinking when using higher values. Some color as Red and Green, just presented
    // themselves with lower bight then they should when the their minimum power is set.
    // When it is set to 100, there are any blink perception.
    DEBUGGERLN( 4, "RUNNING: TSC_Timer pwmTimer( 100, &PWMInterrupt )" );
    TSC_Timer pwmTimer( 100, &PWMInterrupt );
    
    DEBUGGERLN( 4, "RUNNING: configureTheLedsEffects()" );
    configureTheLedsEffects();
    
    DEBUGGERLN( 4, "RUNNING: g_nic = new NIC()" );
    g_nic = new NIC();
    
    Thread* uartThread;
    Thread* ledEffectThread;
    
    // Thread( int (* entry)(), const State & state = READY, const Criterion & criterion = NORMAL, unsigned int stack_size = STACK_SIZE )
    // 
    // Creates a thread with the following parameters:
    //
    // entry:
    // entry point for the thread (defines the thread behavior). entry should be a C++
    // function with signature int func().
    //
    // state:
    // defines the state of the thread upon its creation. Default value is READY, i.e.,
    // it is able to run the next time the defined period is reached.
    //
    // criterion:
    // defines the criterion to be used for this thread. The criterion is based on the
    // Criterion defined by the Scheduler. It is better explained in the Scheduler section of this guide.
    //
    // stack_size:
    // defines the size of the thread's stack. By default it takes the value set by the
    // system's Traits. If a larger (or smaller) stack is desired, this parameter will allow you to do so.
    DEBUGGERLN( 4, "RUNNING: uartThread = new Thread..." );
    uartThread = new Thread( &ReceiveCommandUART );
    
    DEBUGGERLN( 4, "RUNNING: ledEffectThread = new Thread..." );
    ledEffectThread = new Thread( &LEDPowerEffect );
    
    // To wait one second to be able to see the following messages after the uartThread and
    // ledEffectThread messages.
    Alarm::delay( 1e6 );
    
    
    ADC adc( ADC::SINGLE_ENDED_ADC7 );
    adc.enable();
    PRINTLN( 1, "THE GREAT LIGHT VALUE IS: " << adc.get() );
    
    
    
    
    
    
    
    
    
    
    // The join() method suspends the execution of the calling thread (i.e., the
    // thread that is running) until the called thread finishes its execution.
    PRINTLN( 1, "Waiting for uartThread to finish" );
    int uartThreadStatus = uartThread->join();
    
    DEBUGGER( 1, "uartThreadStatus: " << uartThreadStatus );
    PRINTLN( 1, "Waiting for uartThread to finish" );
    int ledEffectThreadStatus = ledEffectThread->join();
    
    DEBUGGER( 1, "ledEffectThreadStatus: " << ledEffectThreadStatus );
    PRINTLN( 1, "Threads finished. EposMotesII app finishing" );
    
    
    
    
    
    
    //Lista das pessoas que se importam com essa parte do código:
    //Evandro  Coan
    //Fim da lista
    
    //Thread* nicThread;
    //Thread* pwmThread;
    
    //Uncomment later when use photo sensor.
    //useSensor = myCuteSensor.enable();
    
    //pwmThread = new Thread(&PWMLeds);
    //nicThread = new Thread(&ReceiveCommandNIC);
    
    //int status_thrdNIC  = nicThread->join();
    //int status_thrdPWM = pwmThread->join();
    return 0;
}

//TODO
//Get register addresses from include/mach/mc13224v/memory_map.h and implement function.
//Also implement method setReg.
//Please do not suicide.
/*
 void PWM1_Init(void)
 {
 setReg(TMR0_CNTR,0); // Reset counter
 /// TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
 //Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=1,OPS=0,OEN=1
 setReg(TMR0_SCTRL,0x05); // Enable output
 setReg(TMR0_COMP1,1500); // Store initial value to the duty-compare register
 // TMR0_CTRL: CM=1,PCS=8,SCS=0,ONCE=0,LENGTH=0,DIR=0,Co_INIT=0,OM=6
 setReg(TMR0_CTRL, 0x3006);
 }
*/

/**
 * This function is used to toggle the led on or off.
 *
 * @param pin        The pin number that we want to toggle
 *
 * @param on         If this is true, we toggle the bit on. If this is false, we toggle the bit off
 *
 */
void turn_led( int pin, bool on )
{
    int          bit     = pin % 32;
    unsigned int regData = GPIO_DATA_SET0 + ( ( pin >> 5 ) << 2 );
    unsigned int regPad  = GPIO_PAD_DIR0 + ( ( pin >> 5 ) << 2 );
    unsigned int value   = CPU::in32( regPad );
    
    if( on )
    {
        value |= ( 1 << bit );
    }
    else
    {
        value &= !( 1 << bit );
    }
    
    CPU::out32( regPad, value );
    CPU::out32( regData, ( 1 << bit ) );
}

/**
 * This function executes the pulse width modulation for the LEDS in software.
 *
 * @return              I have no idea.
 */

/*
int PWMLeds()
{
    PRINTLN( 1, "Thread PWM LEDs initing" );
    powerCalculateFunc func;
    
    if (useSensor)
    {
        func = sensorCalculatePower;
    }
    else
    {
        func = defaultPower;
    }
    
    // not all leds are actually used. Only the RGB ones (the first 3)
    int leds[5]; 
    
    leds[0] = 10;
    leds[1] = 9;
    leds[2] = 11;
    leds[3] = 23;
    leds[4] = 8;
    
    unsigned int cont;
    unsigned int currentIndex;
    int          calculatedPow[ MAX_LEDS_ALLOWED_TO_BE_USED ];
    
    cont = 0;
    
    for(currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++)
    {
        power[currentIndex] = 50; // leds start at 1% of the power (just to show app is running)
    }

    // PWM
    while (!g_finishThread)
    {
        if (!cont)
        {
            for (int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex)
            {
                calculatedPow[currentIndex] = func(power[currentIndex]);
            }
        }
        
        //PRINTLN( 1, "Still executing PWM. " << cont );
        cont == 99 ? cont = 0 : cont++;

        for (currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++)
        {
            turn_led(leds[currentIndex], cont < calculatedPow[currentIndex]);
        }

        // if (cont==0 && power[currentIndex]>0)
        //turn_led(leds[currentIndex],true);
        //else
        //if (cont==power[currentIndex])
        //turn_led(leds[currentIndex],false);
    }
     
    PRINTLN( 1, "Thread PWM LEDs finishing" );
    return 0;
}
*/

void InterpretMessage( char msg[ MAX_MESSAGE_LENGTH_ALLOWED ] )
{
    unsigned int led;
    unsigned int pow;
    unsigned int tempDelay;
    unsigned int currentIndex;
    
    switch( msg[ 0 ] )
    {
        case 'R':
        {
            led = 0;
            break;
        }
        case 'G':
        {
            led = 1;
            break;
        }
        case 'B':
        {
            led = 2;
            break;
        }
        case 'A':
        {
            led = MAX_LEDS_ALLOWED_TO_BE_USED;
            break;
        }
        default:
        {
            PRINTLN( 1, "Invalid led value '" << msg[ 0 ] << "' on position 0" );
            return;
        }
    }
    
    //led = ((unsigned int)msg[0])-48; // int based on ascii
    if( msg[ 1 ] == 'E' )
    {
        //g_effect
        if( msg[ 2 ] == 'N' ) // turn ON g_effect
        {
            if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // one led
            {         // mutexEffect[led]->unlock();
                g_effect[ led ] = true;
            }
            else   // all leds at once
            {
                for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                {
                    // mutexEffect[currentIndex]->unlock();
                    g_effect[ currentIndex ] = true;
                }
            }
            
            PRINTLN( 1, "Effect[" << led << "]=ON" );
        }
        else if( msg[ 2 ] == 'F' ) // turn OFF g_effect
        {
            if( led < MAX_LEDS_ALLOWED_TO_BE_USED )
            {
                // mutexEffect[led]->lock();
                g_effect[ led ] = false;
            }
            else
            {
                for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                {
                    // mutexEffect[currentIndex]->lock();
                    g_effect[ currentIndex ] = false;
                }
            }
            
            PRINTLN( 1, "Effect[" << led << "]=OFF" );
        }
        else     // set g_effect delay
        {
            tempDelay     = ( (unsigned int) msg[ 2 ] ) - 48;
            tempDelay    *= 10 ^ ( ( (unsigned int) msg[ 3 ] ) - 48 );
            g_effectDelay = tempDelay;
            
            PRINTLN( 1, "Delay=" << g_effectDelay );
        }
    }
    else if( msg[ 1 ] == '0' || msg[ 1 ] == '1' )
    {
        // fixed power
        pow  = 100 * ( ( (unsigned int) msg[ 1 ] ) - 48 );
        pow += 10 * ( ( (unsigned int) msg[ 2 ] ) - 48 );
        pow += 1 * ( ( (unsigned int) msg[ 3 ] ) - 48 );
        
        if( pow > 100 )
        {
            pow = 100;
        }
        
        if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // only one led
        {
            power[ led ] = pow;
        }
        else   // all leds at once
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
            {
                power[ currentIndex ] = pow;
            }
        }
        
        PRINTLN( 1, "Power[" << led << "]=" << pow );
    }
    else
    {
        PRINTLN( 1, "Invalid value '" << msg[ 1 ] << "' on position 1" );
    }
}

void SendMessageToNIC( char msg[ MAX_MESSAGE_LENGTH_ALLOWED ] )
{
    int r;
    
    // != 11 ?
    while( ( r = g_nic->send( NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof( msg ) ) ) == 0 )
    {
        PRINTLN( 1, "Send failed " << r );
    }
    
    PRINTLN( 1, "Message sent" );
}

int ReceiveCommandUART()
{
    PRINTLN( 1, "Thread UART initing..." );
    
    unsigned int currentIndex;
    char         msg[ MAX_MESSAGE_LENGTH_ALLOWED ]; //[DATA_SIZE];
    
    UART* uart = new UART();
    
    while( !g_finishThread )
    {
        // messages start with ":"
        do
        {
            PRINTLN( 1, "uart->get()..." );
            msg[ 0 ] = uart->get();
        } while( msg[ 0 ] != ':' );
        
        PRINTLN( 1, "currentIndex = 0..." );
        currentIndex = 0;
        
        while( ( msg[ currentIndex ] != '\n' ) && ( currentIndex < MAX_MESSAGE_LENGTH_ALLOWED ) )
        {
            msg[ currentIndex++ ] = uart->get();
        }
        
        memset( msg + currentIndex, 0x00, MAX_MESSAGE_LENGTH_ALLOWED - currentIndex );
        
        // message received.
        SendMessageToNIC( &msg[ 0 ] );
        InterpretMessage( &msg[ 0 ] );
    }
    
    PRINTLN( 1, "Thread UART finishing..." );
    return 0;
}

int ReceiveCommandNIC()
{
    PRINTLN( 1, "Thread NIC initing..." );
    
    char msg[ MAX_MESSAGE_LENGTH_ALLOWED ];
    
    NIC::Protocol prot;
    NIC::Address  src;
    
    while( !g_finishThread )
    {
        while( !( g_nic->receive( &src, &prot, &msg, sizeof( msg ) ) > 0 ) )
        {
            //PRINTLN( 1, "." );
        }
        
        PRINTLN( 1, "\nMessage received: " << msg );
        InterpretMessage( msg );
    }
    
    PRINTLN( 1, "Thread NIC finishing..." );
    return 0;
}

int LEDPowerEffect()
{
    PRINTLN( 1, "Thread Effect initing..." );
    
    int          pow;
    unsigned int currentIndex;
    //unsigned int collunmIndex;
    
    //currentIndex = MAX_LEDS_ALLOWED_TO_BE_USED;
    
    while( !g_finishThread )
    {
        //for (currentIndex=0; currentIndex<=MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++) {
        //PRINTLN( 1, "Incresing power of led[" << i << "]" );
        
        for( pow = 0; pow <= 100; pow++ )
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
            {
                // mutexEffect[currentIndex]->lock();
                //if (currentIndex<MAX_LEDS_ALLOWED_TO_BE_USED) // only one led
                
                if( g_effect[ currentIndex ] )
                {
                    power[ currentIndex ] = pow;
                    
                    //else // all leds at once
                    //   for (collunmIndex=0; collunmIndex<MAX_LEDS_ALLOWED_TO_BE_USED; collunmIndex++)
                    //      power[collunmIndex]=pow;
                    // mutexEffect[currentIndex]->unlock();
                    Alarm::delay( g_effectDelay );
                }
            }
        }
        
        for( pow = 100; pow >= 0; pow-- )
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
            {
                if( g_effect[ currentIndex ] )
                {
                    // mutexEffect[currentIndex]->lock();
                    //if (currentIndex<MAX_LEDS_ALLOWED_TO_BE_USED)
                    power[ currentIndex ] = pow;
                    //else
                    //   for (collunmIndex=0; collunmIndex<MAX_LEDS_ALLOWED_TO_BE_USED; collunmIndex++)
                    //      power[collunmIndex]=pow;
                    // mutexEffect[currentIndex]->unlock();
                    Alarm::delay( g_effectDelay );
                }
            }
        }
    
    }
    
    // By calling this method, the currently running thread is stopped and put in FINISHING state.
    // If there are "joining threads" for the running thread (i.e., threads that called join() for
    // the running thread), these threads have its state set back to READY and are reinserted into
    // the scheduling queue.
    // static void exit(int status = 0)
    
    PRINTLN( 1, "Thread Effect finishing..." );
    return 0;
}

void PWMInterrupt()
{
    static unsigned int dummyCounter = 0;
    
    // not all leds are actually used. Only the RGB ones (the first 3)
    int leds[ 5 ]; 
    
    leds[ 0 ] = 10;
    leds[ 1 ] = 9;
    leds[ 2 ] = 11;
    leds[ 3 ] = 23;
    leds[ 4 ] = 8;
    
    for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
    {
        if( dummyCounter < power[ currentIndex ] )
        {
            turn_led( leds[ currentIndex ], true );
        }
        else
        {
            turn_led( leds[ currentIndex ], false );
        }
    }
    
    dummyCounter = ( dummyCounter + 1 ) % 100;
}

/**
 * Stills a mystery what does it do exactly. Maybe to initialize the led effects.
 */
void configureTheLedsEffects()
{
    //Alarm::delay(100);
    
    for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
    {
        //   mutexEffect[currentIndex]= new Mutex();
        //   mutexEffect[currentIndex]->lock(); // g_effect starts OFF (blocked)
        g_effect[ currentIndex ] = false;
    }
}

#if defined DEBUG
/**
 * To creates an objecto from the test class 'MyClass' and to call its only member to printf
 * its hi int number to OStream.
 * 
 * @return a dummy value just to allows it to be used within 'DEBUGGER(...)'.
 */
int myClassObjectTest()
{
    MyClass myClassObject;
    
    DEBUGGERLN( 1, "myClassObject.get_hi(): " << myClassObject.get_hi() );
    return 0;
}
#endif
