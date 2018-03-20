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


int main(int argc, char *argv[])
{
	char *host, *mensagem, letraChar;
	int s, server_port, len,letraAsc, numCrip, criptacao, i;

	//Variaveis de passagem por argumento
	server_port = atoi(argv[1]);


	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) logexit("socket");

	struct in_addr inaddr;
	inet_pton(AF_INET, "127.0.0.1", &inaddr);

	struct sockaddr_in addr;
	struct sockaddr *addrptr = (struct sockaddr *)&addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	addr.sin_addr = inaddr;

	if(bind(s, addrptr, sizeof(struct sockaddr_in)))
		logexit("bind");

	if(listen(s, 10)) logexit("listen");
	printf("esperando conexao\n");

	while(1) {
		struct sockaddr_in raddr;
		struct sockaddr *raddrptr = (struct sockaddr *)&raddr;
		socklen_t rlen = sizeof(struct sockaddr_in);

		int r = accept(s, raddrptr, &rlen);
		if(r == -1) logexit("accept");

		char buf[512];
		char ipcliente[512];
		inet_ntop(AF_INET, &(raddr.sin_addr), ipcliente, 512);

		printf("conexao de %s %d\n", ipcliente, (int)ntohs(raddr.sin_port));

		//Receber mensagem criptografada (buf)
		size_t c = recv(r, buf, 512, 0);
		printf("recebemos a mensagem %s\n", buf);

		//Receber o inteiro para descriptografar
		char crip[512];
		c = recv(r, crip, 512, 0);
		numCrip = atoi(crip); //Converte a mensagem para inteiro
		printf("Recebemos o numero %s \n\n",crip);
		//Descriptografar mensagem
		len = strlen(buf);
		for(i=0;i<len;i++){
				letraChar = buf[i];
				letraAsc = letraChar - NumCrip;
				if(letraAsc > 122){
							letraAsc = letraAsc - 26;
				}
				buf[i] = letraAsc;
		}
		//puts(buf);
		printf("%s\n\n",buf);

		sprintf(buf, "seu IP eh %s %d\n", ipcliente, (int)ntohs(raddr.sin_port));
		printf("enviando %s\n", buf);

		send(r, buf, strlen(buf)+1, 0);
		printf("enviou\n");

		close(r);
	}

	exit(EXIT_SUCCESS);
}
