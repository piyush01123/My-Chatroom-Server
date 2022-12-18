
#include <iostream>
#include <string>
#include <sys/socket.h> //socket(),setsockopt(),bind(),listen(),accept(),send()
#include <netinet/in.h> // sockaddr()
#include <arpa/inet.h> // inet_pton()
#include <unistd.h> // read()
#include <thread>
#include <regex>
#define PORT 8000
using namespace std;

void exit_with_error(string err_msg)
{
	cerr << err_msg << endl;
	exit(1);
}

bool isValidPrivateIP(char *host_ip)
{
	regex p("(^192.168.([0-9]|[0-9][0-9]|[0-2][0-5][0-5]).([0-9]|[0-9][0-9]|[0-2][0-5][0-5])$)|(^172.([1][6-9]|[2][0-9]|[3][0-1]).([0-9]|[0-9][0-9]|[0-2][0-5][0-5]).([0-9]|[0-9][0-9]|[0-2][0-5][0-5])$)|(^10.([0-9]|[0-9][0-9]|[0-2][0-5][0-5]).([0-9]|[0-9][0-9]|[0-2][0-5][0-5]).([0-9]|[0-9][0-9]|[0-2][0-5][0-5])$)");
	return regex_match(host_ip, p);
}

void recv_msg(int sockfd)
{
	while(1)
	{
		char buffer[1024] = {0};
		int valread = read(sockfd, buffer, 1024);
		string bufstr(buffer);
		cout << bufstr << endl;	
	}
}

void send_msg(int sockfd)
{
	while(1)
	{
		string msg;
		getline(cin,msg);
		send(sockfd, msg.c_str(), msg.length(), 0);
	}
}

int main(int argc, char **argv)
{
	if (argc!=2) exit_with_error("Call as ./Client host_ip");
	char *host_ip = argv[1];
	if (!isValidPrivateIP(host_ip)) exit_with_error("Invalid host IP.");
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);// creates socket
	if (sockfd==-1) 
		exit_with_error("Error creating socket");

	sockaddr_in address; //struct from netinet
	address.sin_family = AF_INET;
	if(inet_pton(AF_INET, host_ip, &address.sin_addr)<0)  
		exit_with_error("Error in host address");
	address.sin_port = htons(PORT);

	if (connect(sockfd, (sockaddr *)&address, sizeof(address))<0)
		exit_with_error("Error in connect()");

	// r+w can be in any order
	thread recv_thread(recv_msg, sockfd);
	thread send_thread(send_msg, sockfd);
	recv_thread.join();
	send_thread.join();
	return 0;
}