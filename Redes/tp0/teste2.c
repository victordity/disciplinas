#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//Bibliotecas para se usar socket
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

//a funcao logexit retorna se tiver algum erro
void logexit(const char *str)
{
  perror(str); //Imprime o erro
  exit(EXIT_FAILURE);
}

//Funcao principal recebendo os parametros por linha de comando
int main(int argc, char *argv[]){

    printf("Entrou no programa \n");

    char *host, *mensagem, mensagemCrip;
    int s, server_port, len, numCrip, criptacao;

    host = argv[1];
    server_port = atoi(argv[2]);
    mensagem = argv[3];
    numCrip = atoi(argv[4]);
    printf("Meu host eh: %s \n",host);
    printf("Minha porta de comunicacao eh : %d \n",server_port);
    printf("A mensagem do cliente eh: %s \n",mensagem);
    printf("Inteiro sem sinal para criptografia eh %d \n\n", numCrip);

    //Obter informacao do host (servidor)
    struct in_addr inaddr;
    inet_pton(AF_INET, host, &inaddr);//Pega o endereço IP do servidor e armazena em bin na inaddr

    struct sockaddr_in addr;
    struct sockaddr *addrptr = (struct sockaddr *)&addr;
    addr.sin_family = AF_INET;//Protocolo de comunicaçao
    addr.sin_port = htons(server_port);//indica a porta que quero conectar
    addr.sin_addr = inaddr;//Indica o servidor(IP) que quero conectar

    //Abertura ativa
    //Descritor socket para ler e escrever dados atraves do socket criado
    s = socket(AF_INET, SOCK_STREAM, 0);
  	if(s == -1){
        logexit("socket");
    }
    else{
        printf("Socket criado com sucesso \n");
    }

    //Valida a conexao
  	if(connect(s, addrptr, sizeof(struct sockaddr_in))){
  	     logexit("connect");
    }
    else{
        printf("COnexao estabelecida com sucesso \n");
    }

    //Enviar mensagem

    //ssize_t count;
  	//count = send(s, mensagem, strlen(mensagem)+1, 0);

    //Criptografar mensagem
    char letraChar;
    int letraAsc, i;

    len = strlen(mensagem);
    for(i=0;i<len;i++){
        letraChar = mensagem[i];
        letraAsc = letraChar + numCrip;
        if(letraAsc > 122){
              letraAsc = letraAsc - 26;
        }
        mensagem[i] = letraAsc;
    }

     //Enviar mensagem para o host
  	if(send(s, mensagem, strlen(mensagem)+1, 0))
  		logexit("send");

  	/*char buf[512];
  	memset(buf, 0, 512);
  	unsigned total = 0;
  	while(1) {
  		count = recv(s, buf+total, 512, 0);
  		if(count == 0) break;
  		total += count;
  	}

  	printf("received %d bytes\n", (int)total);
  	puts(buf);
    */
  	exit(EXIT_SUCCESS);


}
