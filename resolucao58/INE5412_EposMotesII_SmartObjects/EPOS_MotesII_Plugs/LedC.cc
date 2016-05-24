#include <thread.h>
#include <uart.h>
#include <traits.h>
#include <mutex.h>

__USING_SYS

const unsigned int GPIO_BASE =0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;

int LedStatus[] = {0,0,0}; //>0 = on, 0,1,2= red, green, blue

const unsigned int MSG_LEN = 6;

Mutex* mutex;
OStream cout;

enum LED {
RGB_R = 10,
RGB_G = 9,
RGB_B = 11,
GREEN = 8,
RED = 23
};

bool fthread = true;

//Turns led on if value is > 0, or off if value == 0
int turn_led(int led, bool on){
	
	int          bit     = led % 32;
	unsigned int regData = GPIO_DATA_SET0 + ( ( led >> 5 ) << 2 );
    	unsigned int regPad  = GPIO_PAD_DIR0  + ( ( led >> 5 ) << 2 );
    	unsigned int value   = CPU::in32( regPad );
	
	if(!on){
		//turn off
		value &= ~( 1 << bit );		

	} else {  
		//turn on
		value |= ( 1 << bit );
	}

	CPU::out32(regPad,value);
	CPU::out32(regData, ( 1 << bit ) );

	return 0;
} 

int powerThreads(){
	int cR = 0;
	int cB = 0;
	int cG = 0;

	while(fthread){
		if(LedStatus[0] != 0 && cR >= 100/LedStatus[0]){
			turn_led(RGB_R,true);
			cR = 0;
		} else {
			turn_led(RGB_R,false);
		}
		cR++;

		if(LedStatus[2] != 0 && cB >= 100/LedStatus[2]){
			turn_led(RGB_B,true);
			cB = 0;
		} else {
			turn_led(RGB_B,false);
		}
		cB++;

		if(LedStatus[1] != 0 && cG >= 100/LedStatus[1]){
			turn_led(RGB_G,true);
			cG = 0;
		} else {
			turn_led(RGB_G,false);
		}
		cG++;
		
	}

	return 0;
}
			

//interprets message
int getMessage(char msg[]){
	
	int j;

	cout<<"Message: " << msg[0] << " being processed\n";
	
	switch(msg[0]){
		case 'R':
			j = 0;
			break;
		case 'G':
			j = 1;
			break;
		case 'B':
			j = 2;
			break;
		case 'E':
			fthread = false;
			return 0;
		default:
			j = 3;
			break;
	}

	int power = 0;
	power += 100 * ((unsigned int)msg[1] - 48);
	power += 10 * ((unsigned int)msg[2] - 48);
	power += 1 * ((unsigned int)msg[3] - 48);
	
	
	LedStatus[j] = power;

	return 0;
}



int receiveUART(){

	char msg[MSG_LEN];

	UART *uart = new UART(9600,8,0,1,0);
	unsigned int i = 0;

	while(fthread){
		
		mutex->lock();
		cout<<"\nWaiting for Message!\n";
		
		while(msg[0] != ':'){
			msg[0] = uart->get();
		}

		while(msg[i -1] != 'z' && i < MSG_LEN){
			msg[i++] = uart->get();
		}
		cout<<"Message Received!\n";
		getMessage(msg);
		memset(msg,0,sizeof(msg));
		i = 0;
	}
	return 0;
}

int main(){
	
	mutex = new Mutex();

	Thread * thrdUART;
	
	Thread * thrdPWR;

	thrdUART = new Thread( &receiveUART );

	thrdPWR = new Thread( &powerThreads );
	
	thrdUART->join();

	thrdPWR->join();
	
	//wait(10);

	cout<<"App finished!!!\n";

	return 0;
}


