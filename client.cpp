
#include <iostream>
#include <string>
#include <sys/socket.h> //socket(),setsockopt(),bind(),listen(),accept(),send()
#include <netinet/in.h> // sockaddr()
#include <arpa/inet.h> // inet_pton()
#include <unistd.h> // read()
#define host_ip "10.1.65.93"
#define PORT 8000
using namespace std;

void exit_with_error(string err_msg)
{
	cerr << err_msg << endl;
	exit(1);
}

// server: socket() -> bind() -> listen() -> accept() -> r+w
// client: socket() -> connect() -> r+w
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);// creates socket
	if (sockfd==-1) 
		exit_with_error("Error creating socket");

	sockaddr_in address; //struct from netinet
	address.sin_family = AF_INET;
	// address.sin_addr.s_addr = htonl(host_ip); //try inet_addr,inet_pton,try removing s_addr
	if(inet_pton(AF_INET, host_ip, &address.sin_addr)<=0)  
		exit_with_error("Error in host address");
	address.sin_port = htons(PORT);

	if (connect(sockfd, (sockaddr *)&address, sizeof(address))<0)
		exit_with_error("Error in connect()");

	// r+w can be in any order
	string hello_msg = "Hello from client...";
	send(sockfd, hello_msg.c_str(), hello_msg.length(), 0);
	char buffer[1024] = {0};
	int valread = read(sockfd, buffer, 1024);
	string bufstr(buffer);
	cout << bufstr << endl;
	return 0;
}