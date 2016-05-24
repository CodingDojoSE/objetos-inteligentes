#include <alarm.h>
#include <machine.h>
#include <nic.h>
#include <utility/ostream.h>
__USING_SYS
const unsigned char SINK_ID = 0x01;

struct Msg {
	int id;
	int x;
	int y;
};

void sender(unsigned char id) {
	NIC nic;
	unsigned char prot;
	unsigned int size;
	Msg msg;
	int i;
	
	while (true) {
		for (i = 5; i < 8; i++) {
			//CPU::out8(Machine::IO::PORTA, (1 << i));
			msg.id = id;
			msg.x = 01;
			msg.y = 8008135;
			size = sizeof(msg);
			nic.send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, size);
			Alarm::delay(100000);
		}
	}
}

void receiver() {
	NIC nic;
	Msg msg;
	OStream cout;
	NIC::Address src;
	unsigned char prot;
	
	cout << "Sink\n";
	
	while (true) {
		while (!(nic.receive(&src, &prot, &msg, sizeof(msg)) > 0));
		cout<< "####################\n";
		cout<< "# Sender id = " << msg.id << "\n";
		cout<< "# x = " << msg.x << "\n";
		cout<< "# y = " << msg.y << "\n";
	}

}
int main() {
sender(1);
//receiver();
}
