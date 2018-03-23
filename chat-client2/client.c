#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSZ 1024

void logexit(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	char *host, *mensagem, letraChar, *cripChar;
  int s, server_port, len,letraAsc, numCrip, criptacao, i;

	host = argv[1];
	server_port = atoi(argv[2]);
	mensagem = argv[3];
	cripChar = argv[4];
	numCrip = atoi(cripChar);


	struct in_addr inaddr;
	inet_pton(AF_INET, host,
			&inaddr);

	struct sockaddr_in addr;
	struct sockaddr *addrptr = (struct sockaddr *)&addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	addr.sin_addr = inaddr;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) logexit("socket");

	if(connect(s, addrptr, sizeof(struct sockaddr_in)))
		logexit("connect");

	//Criptografar Mensagem
	len = strlen(mensagem);
	for(i=0;i<len;i++){
			letraChar = mensagem[i];
			letraAsc = letraChar + numCrip;
			if(letraAsc > 122){
						letraAsc = letraAsc - 26;
			}
			mensagem[i] = letraAsc;
	}

	//Enviar mensagem
	//Obter tamanho da mensagem
	int len_net;
	len_net = htonl(len);
	send(s,(const char*)&len_net,4,0);

	int lenCrip;
	lenCrip = htonl(numCrip);
	send(s,(const char*)&lenCrip,4,0);

	ssize_t count;
	count = send(s, mensagem, strlen(mensagem), 0);
	if(count != strlen(mensagem))
		logexit("send");

		/*
	//Enviar numero para criptografar
	printf("\n\nEnviando o char %s",cripChar);
	count = send(s, cripChar, strlen(cripChar), 0);
	if(count != strlen(cripChar))
		logexit("send");
*/
	printf("\n\nDados recebidos");


	char buf[512];
	memset(buf, 0, 512);
	unsigned total = 0;
	while(1) {
		count = recv(s, buf+total, 512, 0);
		if(count == 0) break;
		total += count;
	}

	printf("received %d bytes\n", (int)total);
	puts(buf);

	exit(EXIT_SUCCESS);
}
