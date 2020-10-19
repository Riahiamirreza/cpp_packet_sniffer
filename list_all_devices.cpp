#include <iostream>
#include <pcap.h>

using namespace std;

void list_devs(){
	
	pcap_if_t *d;
	pcap_if_t *interface;
	char err[PCAP_ERRBUF_SIZE];
	int i=0;
	if(pcap_findalldevs(&interface, err)==-1){
		cout << "Error:" << err << endl;
		return ;
	}
	cout << "Devices:" << endl;
	for(d=interface; d; d=d->next)
		cout << ++i << " : " << d->name << endl;
	return ;

}

int main(){
	
	list_devs();
	return 0;
}
