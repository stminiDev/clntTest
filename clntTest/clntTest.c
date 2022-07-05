#pragma comment(lib,"ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>


void error_handling(char* message);

int main(int argc, char* argv[]){

	WSADATA wsa_data;
	SOCKET h_socket;
	SOCKADDR_IN serv_addr;

	char tmp[100];
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		error_handling("WSAStartup() error!");

	h_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (h_socket == INVALID_SOCKET)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; 
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(h_socket, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		error_handling("connect() error!");

	while (1) {

		puts("Input message:  ");
		gets(tmp);

		send(h_socket, tmp, strlen(tmp) + 1, 0);
		str_len = recv(h_socket, message, sizeof(message) - 1, 0);

		if (str_len <= 0)
			error_handling("read() error!");

		printf("Message from server : %s \n", message);
	}

	closesocket(h_socket);
	WSACleanup();

	return 0;
}
void error_handling(char* message) {

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}