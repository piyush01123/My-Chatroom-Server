
#include <iostream>
#include <string>
#include <sys/socket.h> //socket(),setsockopt(),bind(),listen(),accept(),send()
#include <netinet/in.h> // sockaddr()
#include <arpa/inet.h> // inet_pton()
#include <unistd.h> // read()
#include <thread>
#include <regex>
#include <cstdlib>
#include <time.h>
#define PORT 8000
using namespace std;

void exit_with_error(string err_msg)
{
    cerr << err_msg << endl;
    exit(1);
}

bool isValidPrivateIP(char *host_ip)
{
    regex p1("^192\.168\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])$");
    regex p2("^172\.(1[6-9]|2[0-9]|3[0-1])\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])$");
    regex p3("^10\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-5][0-5])$");
    return regex_match(host_ip, p1) || regex_match(host_ip, p2) || regex_match(host_ip, p3);
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

void send_msg(int sockfd, string username)
{
    while(1)
    {
        string msg;
        getline(cin,msg);
        msg = username + ": " + msg;
        send(sockfd, msg.c_str(), msg.length(), 0);
    }
}

string receive_single_msg(int sockfd)
{
    char buffer[1024] = {0};
    int valread = read(sockfd, buffer, 1024);
    string bufstr(buffer);
    return bufstr;
}

void random_colorize(string &str)
{
    vector<string> colors = vector<string>{"\033[31m","\033[32m","\033[33m","\033[34m","\033[35m","\033[36m","\033[37m"};
    string reset = "\033[0m";
    srand(time(0));
    int idx = rand() % colors.size();
    str = colors[idx] + str + reset;
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

    cout << "Enter server password..." << endl;
    string password;
    getline(cin, password);
    send(sockfd, password.c_str(), password.length(), 0);
    string bufstr = receive_single_msg(sockfd);
    if (bufstr=="bad") exit_with_error("Bad password...");
    cout << "Welcome to our server..." << endl;
    cout << "Enter your username..." << endl;
    string username;
    getline(cin, username);
    random_colorize(username);
    send(sockfd, username.c_str(), username.length(), 0);
    cout << "Hi "<<username<<", You have been added to the server.\nJust type your message and press ↵\nThis is like any other group chatting app!" << endl;

    thread recv_thread(recv_msg, sockfd);
    thread send_thread(send_msg, sockfd, username);
    recv_thread.join();
    send_thread.join();
    return 0;
}

