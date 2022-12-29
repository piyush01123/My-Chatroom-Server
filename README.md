
# Toy ChatServer

<p align="center">
<img src="images/groupchat.png" width="100" >
</p>

This is a toy chat server. To use, just [dowload](https://github.com/piyush01123/Toy-ChatServer/releases/tag/binaries) and run the binaries as per your OS (Tested on MacOS and Ubuntu).

Make binaries executable:
```
chmod +x *.linux *.mac
```

Run tracker:
```
./Tracker.linux
```

Run client with tracker's IP address as argument (must be on same network or else you will have to use a proxyserver):
```
./Client.linux $tracker_ip
```


**Main ideas:** Multi-threading, Socket programming


## Compilation

To compile on Linux machines:
```
g++ tracker.cpp -lpthread -o Tracker.linux
g++ client.cpp -lpthread -o Client.linux
```

To compile on MacOS:
```
g++ -std=c++17 tracker.cpp -lpthread -o Tracker.mac
g++ -std=c++17 client.cpp -lpthread -o Client.mac
```

## About this software
This runs on a client-server model. There should be just 1 tracker whereas there can be several clients. The machine running the tracker can be a client too. The client can join and leave as they wish. Just like whatsapp, a new client will be able to view messages in the group from the point she joins. After running the tracker, it asks for a server password that every client will need to join the chat server. For the clients, after validating the server password, it asks for a username. Then it shows a welcome message. Now the clients can chat. Anyone can join and leave in the middle. The clients can type `exit` to leave. When everybody leaves, the tracker will exit. All this while, the entire conversation gets recorded in a log file `chat.log`.

**Update:** Client can exit by typing exit. When all the clients exit, the tracker also exits. I decided that it is better this way than having a session run indefinitely.


**Update:** If a client presses ctrl+c, the tracker will detect that and gracefully handle it. Also, if tracker presses ctrl+c,all the clients will detect that and they will all gracefully exit.


## Snapshots
#### View from client 1 (alpha):

<img src="images/alpha.png" >

#### View from client 2 (beta):

<img src="images/beta.png" >

#### View from client 3 (gamma):

<img src="images/gamma.png" >

#### View from tracker:

<img src="images/tracker.png" >
