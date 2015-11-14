/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Listener UDP : seconda versione con socket non bloccanti
 *
 * Questo programma termina con un CRTL+C, entra in ciclo infinito.
 * lanciare questo programma PRIMA di avviare il sender UDP.
 *
 * Ing. Alessio Palma
 *
 * $Id: ex4-recv.c,v 1.1 2002/05/28 11:39:20 alessiop Exp $
 *
 */


#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<fcntl.h>

main()
{
     int socket_descriptor, i, bytes_read;
		 unsigned int counter;
     struct sockaddr_in socket_address, remote_address;
		 socklen_t addr_len;
		  
		 char buffer[ 255 ];
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
          printf("non posso aprire il socket\n");
          exit(1);
     }
		 
		 /*
		  * Vogliamo un socket che non si blocchi.
			*
			*/
		 fcntl( socket_descriptor, F_SETFL, O_NONBLOCK);

		 
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( 6601 );

     if ( bind( socket_descriptor, 
		            (struct sockaddr *) &socket_address, sizeof(socket_address) ) 
				) {
          printf("non posso collegare il socket\n");
          exit(1);
     }
		 		 
		 for (i=0;;) {
		 
		
							/*
							 * Ci sono dati?
							 * 
							 * 		SI: allora recvfrom restiusce un numero != -1 
							 *
							 */
							
			        bytes_read = recvfrom( socket_descriptor, buffer, 255,0,(struct sockaddr *) &remote_address, &addr_len );
													
							if ( bytes_read != -1 ) {
							        bytes_read = recvfrom( socket_descriptor, buffer, 255,0,(struct sockaddr *) &remote_address, &addr_len );	
							        printf("> counter: %u :: letti %d bytes : %s\n", counter, bytes_read, buffer);
								      i++;
											
						 } 
						 
						  /*
							 * Il contatotore continua a camminare e questo significa che il 
							 * programma non si blocca se non sono presenti dati nel socket.
							 *
							 */
						 
						 counter++;
							
			}
		 
     close(socket_descriptor);
}

