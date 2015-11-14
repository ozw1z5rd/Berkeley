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
     int socket_descriptor, i;
     struct sockaddr_in socket_address, remote_address;
		 
		 char buffer[ 255 ];
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < -0 ) {
          printf("non posso aprire il socket\n");
          exit(1);
     }

		 printf("Il socket è stato aperto\n");
		 
		 /*
		  * oltre all'indirizzo locale del socket è necessario definire
			* anche l'indirizzo remoto, attenzione al codice che segue
			*
			*/
			
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( 6600 );
		 
		 remote_address.sin_family = AF_INET;
     remote_address.sin_addr.s_addr = htonl(INADDR_ANY);
     remote_address.sin_port = htons( 6601 );

     if ( bind( socket_descriptor, (struct sockaddr *) &socket_address, sizeof(socket_address) ) ) {
          printf("non posso collegare il socket alla porta e l'indirizzo\n");
          exit(1);
     }

		 /* 
		  * Il vostro codice parte da questo punto, precedentemente il socket è 
		  * stato solo allocato.
			*
			*/
			
		  for (i=0; i<20; i++) {
			 
							strcpy( buffer, "messaggio da inviare");
							
							sendto( socket_descriptor, buffer, strlen(buffer),0, 
							       (struct sockaddr *) &remote_address,sizeof(remote_address) 
							);
							
							delay( 1000 );
			
			}
		 
     close(socket_descriptor);
		 
}

