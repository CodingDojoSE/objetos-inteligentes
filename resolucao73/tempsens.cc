#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
__USING_SYS


/**
 * 
 * Estamos fazendo um programa para ler a temperatura então acender um 
 * led de acordo com a temperatura. Também imprimir para cout o tempetatura.
 * 
 * 
 * 
 * 
 */

OStream cout;

int main() 
{
  bool isTemperatureEnabled = false;
  int  readTemperature = 0;
  
  cout << "EposMotesII app initing\n";
  
  // Constructs a Temperature sensor object.
  Temperature_Sensor myTemperature_Sensor;
  
  isTemperatureEnabled = myTemperature_Sensor.enable();
  
  cout << "Enabling the myTemperature_Sensor: " << isTemperatureEnabled;
  
  if( isTemperatureEnabled )
  {
    readTemperature = myTemperature_Sensor.sample();
   
    cout << "The temperature is: " << readTemperature;
  }
  
  
  
}
