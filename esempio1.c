/*----------------------------------------------------
 * Programmazione di rete
 *
 * Ing. Alessio Palma
 *
 * $Id: esempio1.c,v 1.1 2002/05/26 13:32:57 alessiop Exp $
 *
 */


#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>

main()
{
     int socket_descriptor;
     struct sockaddr_in socket_address;
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < -0 ) {
          printf("non posso aprire il socket\n");
          exit(1);
     }

     printf("Il socket è stato aperto\n");
		 
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( 6600 );

     if ( bind( socket_descriptor, (struct sockaddr *) &socket_address, sizeof(socket_address) ) ) {
          printf("non posso collegare il socket\n");
          exit(1);
     }

     printf("Il socket è stato collegato\n");
		 
     close(socket_descriptor);
		 
     printf("Il socket è stato chiuso.\n");
}

