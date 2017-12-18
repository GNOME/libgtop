#include "dev_handles.h"
int main()
{
	packet_handle *handles = open_pcap_handles();
	printf("\n PCAP HANDLES \n");
	print_pcap_handles(handles);
}