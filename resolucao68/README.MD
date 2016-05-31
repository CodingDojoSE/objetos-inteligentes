Foi lido o arquivo doc1.pdf e tentado executá-lo por completo, nao foi possivel;

Para criar a aplicação foi necessário incluir o programa na pasta app do EPOS
Não foi possível enviar a aplicação para o EPOS Motes


24/05/2016

Hoje vamos fazer comunicação em rede entre dois EPOSMotes2.
Para isso pegamos um código inicial do github no seguinte endereço:

Este é o receiver
https://github.com/evandrocoan/INE5412_EposMotesII_SmartObjects/blob/develop/EPOS_MotesII_Lamps/testeReceiverNIC.cc

Este é o sender
https://github.com/evandrocoan/INE5412_EposMotesII_SmartObjects/blob/develop/EPOS_MotesII_Lamps/testeSenderNIC.cc


Conseguimos a comunicação em rede rodando o receiver na maquina 68 e o sender na maquina 58. 

Usamos os scripts de compilação e instalação do link:
https://github.com/evandrocoan/INE5412_EposMotesII_SmartObjects



#ATUALIZAÇÃO 26/05/2016: Para compilar e instalar o programa

Siga o manual na nossa primeiro página do github:
https://github.com/CodingDojoSE/objetos-inteligentes

#ATUALIZAÇÃO 31/05/2016: Usar o sensor de ilumincação com ADC 

Ligamos os 3 pinos do sensor de iluminação recebido pelo professor nos seguintes pinos do modulo J2 do EPOS:
 Pino 1: VCC
 Pino 11: ADC7
 Pino 20: GND

Instanciamos um objeto do tipo ADC para ler o valor recebido no ADC7, que é correspondente a iluminação lida pelo sensor
  documentação do ADC: http://epos.lisha.ufsc.br/EPOS+User+Guide#ADC

Arquivo com o código para ler o ADC:
  adc_light_sensor.cc
  

 



