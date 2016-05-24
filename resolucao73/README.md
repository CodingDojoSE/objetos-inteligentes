


Nos compilarmos utilizando:
make APPLICATION=objetos-inteligentes/resolucao73/structuredLEDControl

Esse programa foi fornecido pelo professor, para nosso trabalho da disciplina.

Utilizamos esse comando para gerar uma imagem, após compilar.
arm-objcopy -I elf32-little -O binary img/structuredLEDControl.img img/structuredLEDControl.bin

Com a imagem gerada nós conectamos o EPOSMotesII à entrada USB da máquina.
Tivemos um problema com o reconhecimento do dispositivo USB na máquina virtual, isso foi resolvido no menu de dispositivo do Oracle Virtual Box na sessão atual.
Então utilizamos o comando 
python red-bsl.py -t /dev/ttyUSB0 -f img/mc13224v_app.bin
para enviar a imagem para o EPOSMotesII.

Foi solicitado que o EPOSMotesII fosse resetado pelo botão, feito isso a aplicação foi enviada e rodou conforme o esperado.




17-05-16:

Foi copiado o codigo do https://epos.lisha.ufsc.br/EPOS+User+Guide#UART e alterado (app/uartTest.cc), com ele conseguimos ver uma resposta da uart, basicamente o que o cout estava printando
Usando o terminal minicom com baud 9600;
$make APPLICATION=uartTest

Tambem foi gravado o programa tempsens.cc e ele funcionou adequadamente reportando a temperatura de 20
$make APPLICATION=tempsens
$arm-objcopy -I elf32-little -O binary img/tempsens.img img/tempsens.bin
$python red-bsl.py -t /dev/ttyUSB0 -f img/tempsens.bin -S

:)


