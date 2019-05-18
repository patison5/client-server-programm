#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>


using namespace std;



int main () {

	// create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == -1) {
		cerr << "can't create a socket!";
		return -1;
	}

	


	// bind the socket to a IP / PORT
	sockaddr_in channel; //store ip-address

	channel.sin_family = AF_INET;
	channel.sin_port = htons(8080);
	inet_pton(AF_INET, "0.0.0.0", &channel.sin_addr);

	if (bind(listening, (sockaddr*)&channel, sizeof(channel)) == -1) {
		cerr << "can't bind to IP / PORT";
		return -2;
	}



	// Mark the socket for listening in
	if (listen(listening, SOMAXCONN) == -1) {
		cerr << "can't listen!";
		return -3;
	}

	


	// Accept a call
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == -1) {
		cerr << "Problem with client connecting!";
		return -4;
	}




	// Close the listening socket
	close(listening);

	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);


	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXSERV, svc, NI_MAXSERV, 0);

	if (result) {
		cout << "hello world: " << svc << ", ... " << host << endl; 
		cout << host << " connected on " << svc << endl;
	} else {
		cout << "hello world: " << svc << ", ... " << host << endl; 

		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on " << ntohs(client.sin_port) << endl; 
	}





	// while receiving - display message, echo message

	char buf[4096];

	while (true) {
		memset(buf, 0, 4096); //очищаем буфер (?)

		int bytesRecv = recv(clientSocket, buf, 4096, 0); //читаем данные из соекета

		if (bytesRecv == -1) {
			cerr << "there was a connection issue" << endl;
			break;
		}

		if (bytesRecv == 0) {
			cout << "the client disconected!" << endl;
			break;
		}

		cout << "Recieved: " << string(buf, 0, bytesRecv) << endl;

		send(clientSocket, buf, bytesRecv + 1, 0);
		// send(clientSocket, "hello man!", 4096, 0);
	}

	// close socket

	close(clientSocket);

	return 0;
}