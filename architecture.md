Here is the architecture of the system.

## Tracker:
```
int clients[];
main(){
    cin >> pwd;
    thread threads[];
    int sockfd = socket();
    while(1){
        int client = accept(sockfd);
        if (!auth(client, pwd)) continue;
        clients.pb(client);
        thread th(recv_msg, client);
        threads.pb(th);
    }
    for (auto &t: threads) t.join();
}

void recv_msg(int client){
    while(1){
        char buffer[];
        for (int clt: clients){
          if (clt==client) continue;
          send(buffer, clt);
        }
    }
}
```

## Client:
```
main()
{
    int sockfd = socket();
    cin >> pwd >> username;
    if (!auth(pwd)) exit(1);
    thread t1(recv_msg, sockfd);
    thread t2(send_msg, sockfd);
    t1.join();
    t2.join();
}

void recv_msg(int sockfd)
{
    while(1){
        char buffer[];
        read(sockfd, buffer);
        cout << buffer << endl;
    }
}

void send_msg(int sockfd)
{
    while(1){
        string msg;
        cin >> msg;
        send(sockfd, msg);
    }
}
```
