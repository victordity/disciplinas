#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSZ 1024

//a funcao logexit retorna se tiver algum erro
void logexit(const char *str)
{
	perror(str);//Imprime o erro
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	char *host, *mensagem, letraChar, *cripChar;
	//char buf[MAX_LINE];
	int s, server_port, len,letraAsc, numCrip, criptacao, i;

	//Variaveis de passagem por argumento
	host = argv[1];
	server_port = atoi(argv[2]);
	mensagem = argv[3];
	cripChar = argv[4];
	numCrip = atoi(argv[4]);


	//Obter informacao do host
	struct in_addr inaddr;
	inet_pton(AF_INET, host, &inaddr);//Pega o endereço IP do servidor e armazena em bin na inaddr

	//Monta a estrutura de dados do endereço
	struct sockaddr_in addr;
	struct sockaddr *addrptr = (struct sockaddr *)&addr;
	addr.sin_family = AF_INET;//Protocolo de comunicaçao
	addr.sin_port = htons(server_port); //indica a porta que quero conectar
	addr.sin_addr = inaddr; //Indica o servidor(IP) que quero conectar

	//Abertura ativa
	//Descritor socket para ler e escrever dados atraves do socket criado
	s = socket(AF_INET, SOCK_STREAM, 0);
	//Valida o socket
	if(s == -1) logexit("socket");

	if(connect(s, addrptr, sizeof(struct sockaddr_in)))
		logexit("connect");

	//Obter mensagem
	printf("mensagem eh %s ",mensagem);
	ssize_t count;

	//Criptografar mensagem
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
	count = send(s, mensagem, strlen(mensagem)+1, 0);
	if(count != strlen(mensagem)+1)
		logexit("send");

	//Enviar numero para criptografia (CripChar)
	count = send(s, cripChar, strlen(cripChar)+1, 0);
	if(count != strlen(cripChar)+1)
		logexit("send");



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
