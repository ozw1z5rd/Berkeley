/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Listener UDP : seconda versione con socket non bloccanti
 * INDICARE IL NUMERO DI PORTA SU CUI ASCOLTARE 
 *
 * Questo programma termina con un CRTL+C, entra in ciclo infinito.
 * lanciare questo programma PRIMA di avviare il sender UDP.
 *
 * Ing. Alessio Palma
 *
 * $Id$
 *
 */


#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<fcntl.h>


main(int argc, char **argv )
{
		 
     int socket_descriptor, i, bytes_read;
		 unsigned int counter;
     struct sockaddr_in socket_address, remote_address;
		 socklen_t addr_len;
		 
		 char str_message[255];

		 printf("Listener\n");
		 printf("Tento di mettermi in ascolto sulla porta %s ...\n",argv[1]);  
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
          printf("Errore: non posso aprire il socket\n");
          exit(1);
     }
		 
		 /*
		  * Vogliamo un socket che non si blocchi.
			*
			*/
		 fcntl( socket_descriptor, F_SETFL, O_NONBLOCK);

		 
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( atoi(argv[1]) );

     if ( bind( socket_descriptor, 
		            (struct sockaddr *) &socket_address, sizeof(socket_address) ) 
				) {
          printf("Errore: non posso collegare il socket\n");
          exit(1);
     }
		 
		 printf("OK.\n");
		 		 
		 while(1) {
		 
			        bytes_read = recvfrom( socket_descriptor, str_message, 255,0,(struct sockaddr *) &remote_address, &addr_len );											
							if ( bytes_read != -1 ) {							       
								 	 		printf("Messaggio da: %s porta %d>>", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port));	
							        printf("%s\n", str_message);
						 } 
							
			}
		 
     close(socket_descriptor);
}

