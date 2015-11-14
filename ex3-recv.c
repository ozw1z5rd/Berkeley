/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Listener UDP : seconda versione
 *
 * Questo programma termina con un CRTL+C, entra in ciclo infinito.
 * lanciare questo programma PRIMA di avviare il sender UDP.
 *
 * Ing. Alessio Palma
 *
 * $Id: ex3-recv.c,v 1.1 2002/05/28 10:39:54 alessiop Exp $
 *
 */


#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/time.h>

main()
{
     int socket_descriptor, i, bytes_read;
		 unsigned int counter;
     struct sockaddr_in socket_address, remote_address;
		 socklen_t addr_len;
		 
		 /*
		  * file descriptor
			* serve per poter avere informazioni sullo stato del socket.
			*
			*/
			
		  fd_set fd_read;
		 
		 /*
		  * timeval
			* ci serve per indicare quanto tempo deve durare il controllo
			* sullo stato del socket
			*
			*/
		 struct timeval timeout;
		 
		 char buffer[ 255 ];
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < -0 ) {
          printf("non posso aprire il socket\n");
          exit(1);
     }

     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( 6601 );

     if ( bind( socket_descriptor, 
		            (struct sockaddr *) &socket_address, sizeof(socket_address) ) 
				) {
          printf("non posso collegare il socket\n");
          exit(1);
     }
		
		 /*
		  * Inizializziamo le struttura del timeout
			* nessuna attesa. P.S: usec = microsecondi non milli.
			*
			*/
 
		 timeout.tv_sec = 0;
		 timeout.tv_usec = 0;
		 
		 /*
		  * Inizializzazione della struttra File Description
			*
			*/
			
		 FD_ZERO( &fd_read );
		 		 
		 for (i=0;;) {
		 
		 		 			/*
		  				 * Inizializziamo le struttura del file descriptor
							 *
							 * FD_SET associa la struttura al nostro socket.
							 * 
			         */
							 					
							 FD_SET ( socket_descriptor, &fd_read);
							 
							/*
							 * Interroga il socket
							 * Attenzione al +1
							 * 
							 */							 						
							
              if ( select( socket_descriptor+1, &fd_read, 0, 0, &timeout) == -1 ) {
								     printf("select error\n");
								     exit(1);
							}
							
							/*
							 * Ci sono dati?
							 * 
							 * 		SI: allora leggiamoli, recvfrom non si bloccherà. 
							 *
							 */
							
							if ( FD_ISSET( socket_descriptor, &fd_read) ) {
							 
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

