
#include <iostream>
#include <string>
#include <sys/socket.h> //socket(),setsockopt(),bind(),listen(),accept(),send()
#include <netinet/in.h> // sockaddr()
#include <unistd.h> // read()
#include <thread>
#define PORT 8000
using namespace std;

void exit_with_error(string err_msg)
{
	cerr << err_msg << endl;
	exit(1);
}

void recv_msg(int client_sockfd)
{
	while(1)
	{
		char buffer[1024] = {0};
		int valread = read(client_sockfd, buffer, 1024);
		string bufstr(buffer);
		cout << bufstr << endl;	
	}
}

void send_msg(int client_sockfd)
{
	while(1)
	{
		string msg;
		getline(cin,msg);
		send(client_sockfd, msg.c_str(), msg.length(), 0);
	}
}

// server: socket() -> bind() -> listen() -> accept() -> r+w
// client: socket() -> connect() -> r+w
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0), opt=1; // creates socket
	if (sockfd<0)  exit_with_error("Error creating socket");
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // force use addr
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)); // force use port
	sockaddr_in address; //struct from netinet
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	if (::bind(sockfd, (sockaddr *) &address, sizeof(address))<0) //want socket::bind not std::bind
		exit_with_error("Error binding socket to address");
	if (listen(sockfd, 1000)<0) //at max 1000 ppl can "wait" for connection
		exit_with_error("Error in listening");

	int addr_len = sizeof(address);
	int client_sockfd = accept(sockfd,(sockaddr *)&address, (socklen_t *)&addr_len);
	if (client_sockfd<0) exit_with_error("Error in accepting client");

	// r+w can be in any order
	thread recv_thread(recv_msg, client_sockfd);
	thread send_thread(send_msg, client_sockfd);
	recv_thread.join();
	send_thread.join();
	return 0;
}