
#include <iostream>
#include <string>
#include <sys/socket.h> //socket(),setsockopt(),bind(),listen(),accept(),send()
#include <netinet/in.h> // sockaddr()
#include <unistd.h> // read()
#include <thread>
#include <vector>
#include <algorithm> //remove()
#define PORT 8000
using namespace std;
vector<int> clients;

void exit_with_error(string err_msg)
{
	cerr << err_msg << endl;
	exit(1);
}

void send_to_all(string msg, int sender)
{
	for (int client_fd: clients)
	{
		if (client_fd==sender) continue;
		send(client_fd, msg.c_str(), msg.length(), 0);
	}
}

void recv_msg(int client_fd)
{
	cout << "Client " << client_fd << " will be listened from now on..."<<endl;
	while(1)
	{
		char buffer[1024] = {0};
		int valread = read(client_fd, buffer, 1024);
		string bufstr(buffer);
		cout << "new msg from " << client_fd << endl;
		cout << bufstr << endl;
		if (bufstr.size()>=4 && bufstr.substr(bufstr.size()-4)=="exit")
		{
			cout << client_fd << " is exiting..." << endl;
			close(client_fd);
			clients.erase(remove(clients.begin(),clients.end(),client_fd), clients.end());
			if (clients.empty())
			{
				cout << "Everybody has left. Exiting..." << endl;
				exit(1);
			}
			return; // Client-specific thread will exit here
		}
		send_to_all(bufstr, client_fd);
	}
}

string receive_single_msg(int client_sockfd)
{
	char buffer[1024] = {0};
	int valread = read(client_sockfd, buffer, 1024);
	string bufstr(buffer);
	return bufstr;
}


int main()
{
	cout << "Enter server password to be used for login..." << endl;
	string password;
	getline(cin, password);

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
	vector<thread> threads;
	while(1)
	{
		cout << "Listening..."<<endl; 
		int client_sockfd = accept(sockfd,(sockaddr *)&address, (socklen_t *)&addr_len);
		if (client_sockfd<0) exit_with_error("Error in accepting client");
		string password_recv = receive_single_msg(client_sockfd);
		cout << "New client:" << client_sockfd << endl;
		string good="good", bad="bad";
		if (password_recv==password) send(client_sockfd, good.c_str(), good.length(), 0);
		else send(client_sockfd, bad.c_str(), bad.length(), 0);
		string username = receive_single_msg(client_sockfd);
		cout << username << " has been added..." << endl;
		clients.push_back(client_sockfd);
		thread th(recv_msg, client_sockfd);
		threads.push_back(move(th));
	}
	for (auto &t: threads) t.join();
	shutdown(sockfd, SHUT_RDWR);
	return 0;
}
