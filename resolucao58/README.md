Iniciado seguindo o tutorial
http://epos.lisha.ufsc.br/EPOS+User+Guide#Downloading_and_Installing

Criado manual do Trabalho, O programa compilado está na pasta do $EPOS/app/pc_app.cc.
Vá no virtualbox, clique no menu, Dispositivos -> Dispositivos USB -> FTDI FT232R - USB UART IC
Adicionar caminho para o EPOS
Adicionar o caminho no PATH para o EPOS e para o arm/gcc
Acessar pasta do EPOS
make

Agora para compilar a aplicação use:
make APPLICATION=nome_do_arquivo_que_tem_o_main_sem_sua_extensao_e_esta_na_pasta_app
Exemplo para o arquivo '$EPOS/app/structuredLEDcontrol.cc':
make APPLICATION=objetos_inteligentes/resolucao58/temperatureRead

Agora execute o programa:
arm-objcopy -I elf32-little -O binary img/structuredLEDcontrol.img img/structuredLEDcontrol.bin
cp tools/emote/ssl.bin
cp tools/emote/red-bsl.py

Para colocar para a placa, em sua memória flash:
python red-bsl.py -t /dev/ttyUSB0 -f img/structuredLEDcontrol.bin -S

Extras:
sudo chmod 777 /dev/ttyUSB0

Dia 24-05-16
A comunicação entre placas funcionou usando os arquivos teste{Sender,Receiver}NIC.cc disponíveis
neste repositório, que foram baixar originalmente de:
https://github.com/evandrocoan/INE5412_EposMotesII_SmartObjects/tree/develop/


#ATUALIZAÇÃO 26/05/2016: Para compilar e instalar o programa

Siga o manual na nossa primeiro página do github:
https://github.com/CodingDojoSE/objetos-inteligentes



