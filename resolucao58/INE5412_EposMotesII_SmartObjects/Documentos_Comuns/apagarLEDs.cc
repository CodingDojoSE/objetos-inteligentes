

/*Este programa apaga o led do eposmote

*/
#include <traits.h>

__USING_SYS

unsigned int *GPIO_BASE = (unsigned int*)0x80000000;

bool led1=true;
bool led2=true;

 static void wait(unsigned int us) {
//	long t = clock();
   }


void toggle_on_led1() {
	if(!led1){
		*GPIO_BASE = *GPIO_BASE | (1 << 23);
		led1=true;
	}else{
		*GPIO_BASE = *GPIO_BASE & !(1 << 23);
		led1=false;
	}
}
void turnByte(unsigned int* a){
	*a = !0;
}
void turnOffByte(unsigned int* a){
	*a = 0;
}


void toggle_on_led2() {
    if(!led2){
		*GPIO_BASE = *GPIO_BASE | (1 << 24);
		led2=true;
	}else{
		*GPIO_BASE = *GPIO_BASE & !(1 << 24);
		led2=false;
	}
}

void toggleAll(){
/*
unsigned int* a=GPIO_BASE;
 for(int i = (int)GPIO_BASE ; i< (int)(GPIO_BASE+0x180) ;i++){
		*(a) = !(0);
		a++;

  }
  */
}

int main() {
 
int i=20;

//turnOffByte(GPIO_BASE);
//toggleAll();
toggle_on_led1();

//wait(1000);
 //toggle_on_led2();

  return 0;

 
}

