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

#define BUFSZ 1024
#define MAX_LINE 256


//a funcao logexit retorna se tiver algum erro
void logexit(const char *str)
{
  perror(str); //Imprime o erro
  exit(EXIT_FAILURE);
}

//Funcao principal recebendo os parametros por linha de comando
int main(int argc, char *argv[]){

    printf("Entrou no programa \n");
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host, *mensagem;
    char buf[MAX_LINE];

    int s, server_port, len, numCrip, criptacao;
    host = argv[1];
    server_port = atoi(argv[2]);
    mensagem = argv[3];
    numCrip = atoi(argv[4]);
    printf("Meu host eh: %s \n",host);
    printf("Minha porta de comunicacao eh : %d \n",server_port);
    printf("A mensagem do cliente eh: %s \n",mensagem);
    printf("Inteiro sem sinal para criptografia eh %d \n\n", numCrip);

    //Obter informacao do host
    struct in_addr inaddr;
    inet_pton(AF_INET,host, &inaddr); //Pega o endereço IP do servidor e armazena em bin na inaddr

    //Monta a estrutura de dados do endereço
    struct sockaddr_in addr;
    struct sockaddr *addrptr = (struct sockaddr *)&addr;
    addr.sin_family = AF_INET; //Protocolo de comunicaçao
    addr.sin_port = htons(server_port); //indica a porta que quero conectar
    addr.sin_addr = inaddr;//Indica o servidor(IP) que quero conectar

    //Abertura ativa
    //Descritor socket para ler e escrever dados atraves do socket criado
    s = socket(AF_INET, SOCK_STREAM,0); //(dominio,tipo TCP,protocolo 0 representa IP) local AF_LOCAL ou AF_UNIX
    //Valida o socket
    if(s == -1){
        logexit("socket");
    }
    else{
      printf("Socket criado com sucesso \n");
    }
    /*
    //bind associa o endereco ip a uma porta
    if(bind(s,(struct sockaddr*)&addr,sizeof(addr))){
      logexit('bind');
    }
    */
    //Valida a conexao
    if(connect(s,(struct sockaddr *)&addr, sizeof(addr))){
        logexit("connect");
    }
    else{
        printf("Conexao estabelecida \n");
    }
    //Envia a mensagem
    criptacao = htons(numCrip); //Conversao de inteiro para a representacao da rede
    len = strlen(mensagem);


    send(s, mensagem, criptacao, 0);




    return 0;

}
