#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

void logexit(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}


int main(void)
{
	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) logexit("socket");

	struct in_addr inaddr;
	inet_pton(AF_INET, "127.0.0.1", &inaddr);

	struct sockaddr_in addr;
	struct sockaddr *addrptr = (struct sockaddr *)&addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5152);
	addr.sin_addr = inaddr;

	if(bind(s, addrptr, sizeof(struct sockaddr_in)))
		logexit("bind");

	if(listen(s, 10)) logexit("listen");
	printf("esperando conexao\n");

	while(1) {
		struct sockaddr_in raddr;
		struct sockaddr *raddrptr =
			(struct sockaddr *)&raddr;
		socklen_t rlen = sizeof(struct sockaddr_in);

		int r = accept(s, raddrptr, &rlen);
		if(r == -1) logexit("accept");

		char buf[512];
		bzero(buf,512);
		char cripChar[512];
		bzero(cripChar,512);
		char ipcliente[512];
		inet_ntop(AF_INET, &(raddr.sin_addr),
				ipcliente, 512);

		//Receber tamanho da mensagem
		int len_net;
		int len_msg;
		size_t d = recv(r, &len_net,4,0);
		printf("\nrecebemos %d bytes do tamanho da mensagem\n", (int)d);
		len_msg = ntohl(len_net);
		printf("O numero recebido foi %d\n",len_msg);

		int len_netCrip;
		int len_msgCrip;
		size_t e = recv(r, &len_netCrip,4,0);
		printf("\nrecebemos %d bytes do tamanho do char crip \n", (int)e);
		len_msgCrip = ntohl(len_netCrip);
		printf("O numero recebido foi %d\n",len_msgCrip);

		//Receber mensagem criptografada
		size_t c = recv(r, buf, len_msg, 0);
		printf("recebemos %d bytes\n", (int)c);
		puts(buf);

		//Receber numero pra descriptografar
		//c = recv(r, cripChar, 512, 0);
		int numCrip = len_msgCrip;
		printf("\n\n O numero para criptografar eh %d\n",len_msgCrip);

		//Descriptografar mensagem
		int i, letraAsc, len;
		char letraChar;

		//numCrip = atoi(cripChar);
		len = strlen(buf);
		for(i=0;i<len;i++){
				letraChar = buf[i];
				letraAsc = letraChar - numCrip;
				if(letraAsc < 97){
							letraAsc = letraAsc + 26;
				}
				buf[i] = letraAsc;
		}
		puts(buf);

		/*
		ssize_t count;
		count = send(s, buf, strlen(buf)+1, 0);
		if(count != strlen(buf)+1)
			logexit("send");*/


		printf("\nenviando %s\n", buf);

		//Enviar string descriptografada
		send(r, buf, strlen(buf), 0);
		printf("enviou\n");

		close(r);
	}

	exit(EXIT_SUCCESS);
}
