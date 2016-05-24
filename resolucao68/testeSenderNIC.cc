#include <alarm.h>
#include <machine.h>
#include <nic.h>
#include <utility/ostream.h>
#include <sensor.h>

__USING_SYS

const unsigned char SINK_ID = 0x01;

struct Msg {
    int id;
    int x;
    int y;
};

void sender(unsigned char id) {
    NIC nic;

    unsigned char src, prot;
    unsigned int size;

    Msg msg;

    int i;
    while (true) {
        for (i = 5; i < 8; i++) {
            //CPU::out8(Machine::IO::PORTB, (1 << i));

            msg.id = id;
            msg.x  = 10;
            msg.y  = 20;

            nic.send(SINK_ID, 0, &msg, sizeof(msg));

            Alarm::delay(100000);
        }
    }
}

int receiver() {
	NIC nic;
    NIC::Protocol prot;
    NIC::Address src;

    Msg msg;

    OStream cout;

    //unsigned char src, prot;
    int i;
    
    cout << "Sink\n";

    while (true) {
        while (!(nic.receive(&src, &prot, &msg, sizeof(msg)) > 0));

        cout << "####################\n";
        cout << "# Sender id = " << msg.id << "\n";
        cout << "# x = " << msg.x << "\n";
        cout << "# y = " << msg.y << "\n";
    }
	return 0;
}

int main() {
    sender(1);
//    receiver();
}
