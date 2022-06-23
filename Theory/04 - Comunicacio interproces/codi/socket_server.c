#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     

#define MAXPENDING   5    // Veure fitxa 6 
#define BUFSIZE      100  // Mida del buffer 

int main(int argc, char *argv[])
{
  int i, len;
  int server_fd;                  // Descriptor de socket del servidor 
  int client_fd;                  // Descriptor de socket del client 
  struct sockaddr_in server_addr; // Adreça local 
  struct sockaddr_in client_addr; // Adreça del client 
  unsigned short server_port;     // Port del servidor 
  unsigned int client_addr_len;  
  char buffer1[BUFSIZE], buffer2[BUFSIZE];  // Buffer 

  if (argc != 2)    
  {
    fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
    exit(1);
  }

  server_port = atoi(argv[1]);  

  // Crear una connexio TCP/IP 
  server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_fd < 0) {
    printf("socket() ha fallat.\n");
    exit(EXIT_FAILURE);
  }

  // Construeix l'estructura de l'adreça del servidor 
  memset(&server_addr, 0, sizeof(server_addr));   
  server_addr.sin_family = AF_INET;               
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(server_port);      

  // Associa server_fd amb server_addr 
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("bind() ha fallat. Prova un altre port.\n");
    exit(EXIT_FAILURE);
  }

  // Avisa al SO que server_fd rebra peticions d'entrada 
  if (listen(server_fd, MAXPENDING) < 0) {
    printf("listen() failed.\n");
    exit(EXIT_FAILURE);
  }

  for (;;) // Bucle infinit  
  {
    client_addr_len = sizeof(client_addr);

    printf("Esperant que un client es connecti en el port %d.\n", server_port);

    // Espera que un client es connecti 
    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd < 0) {
      printf("accept() failed.\n");
      exit(EXIT_FAILURE);
    }

    // client_fd esta connectat a un client 

    printf("Se m'ha connectat un client.\n");
    printf("La seva adreça IP es %s\n", inet_ntoa(client_addr.sin_addr));

    // Esperem rebre cadena: primer longitud, despres la cadena  
    read(client_fd, &len, 4);
    read(client_fd, buffer1, len);
    buffer1[len] = '\0';
    printf("He rebut del client: '%s'\n", buffer1);

    // Enviem la cadena invertida
    for(i = 0; i < len; i++)
      buffer2[i] = buffer1[len - i - 1];

    buffer2[len] = '\0';
    printf("Envio la cadena: '%s'\n", buffer2);
    write(client_fd, &len, 4);
    write(client_fd, buffer2, len); 

    close(client_fd);
  }
}
