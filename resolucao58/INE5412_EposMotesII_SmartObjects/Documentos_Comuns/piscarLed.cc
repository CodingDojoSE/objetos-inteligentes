
/*Este programa apaga o led do eposmote
*/
#include <traits.h>
#include <utility/ostream.h>

__USING_SYS

const unsigned int GPIO_BASE = 0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;

bool led1=true;
bool led2=true;

 static void wait(unsigned int us) {
//	long t = clock();
   }


void toggle_on_led1() {
	int pin = 9;
	int          bit     = pin % 32;
    unsigned int regData = GPIO_DATA_SET0 + ( ( pin >> 5 ) << 2 );
    unsigned int regPad  = GPIO_PAD_DIR0  + ( ( pin >> 5 ) << 2 );
    unsigned int value   = CPU::in32( regPad );
    
    if( !led1 )
    {
        value |=  ( 1 << bit );
	led1 = true;
	
    }
    else
    {
        value &= !( 1 << bit );
	led1 = false;
    }
    
    CPU::out32( regPad, value );
    CPU::out32( regData, ( 1 << bit ) );
}
void turnByte(unsigned int* a){
	*a = !0;
}
void turnOffByte(unsigned int* a){
	*a = 0;
}

int main() {
 
int i=0;
OStream cout;

toggle_on_led1();

for(;;){
	for(int j=0;j<100;j++){
		cout<< "LOOP" << j;
	}		
		//wait(1000);
		toggle_on_led1();
		cout << "\nLed1-2\n";
}

  return 0;

 
}
