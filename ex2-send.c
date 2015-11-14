/*---------------------------------------------------------------------------
 * Programmazione di rete 
 * 
 * Semplicissimo sender UDP
 *
 * lanciare questo programma DOPO aver avviato il listener UDP.
 *
 * Ing. Alessio Palma
 *
 * $Id: ex2-send.c,v 1.1 2002/05/26 19:28:55 alessiop Exp $
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
		 
    /*
     * osserviamo che servirà una struttura sock_addr_in per definire 
     * l'indirizzo del socket remoto.
     *
     */
			 
     struct sockaddr_in socket_address, remote_address;
	 
     char buffer[ 255 ];
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
          printf("non posso aprire il socket\n");
          exit(1);
     }
		 
    /*
     * socket che il programma alloca ed userà
     *
     */
						
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( 0 );
		 
    /*
     * socket verso cui spedire i dati
     *
     */
			 
     remote_address.sin_family = AF_INET;
     remote_address.sin_addr.s_addr = htonl(INADDR_ANY);
     remote_address.sin_port = htons( 6601 );

     if ( bind( socket_descriptor, (struct sockaddr *) &socket_address, sizeof(socket_address)) ) {
          printf("non posso collegare il socket\n");
          exit(1);
     }

			
     for (i=0; i<20; i++) {

	strcpy( buffer, "messaggio da inviare");
											
	sendto( socket_descriptor, buffer, strlen(buffer)+1,0, 
                (struct sockaddr *) &remote_address,sizeof(remote_address) 
	      );	
     }
		 
     close(socket_descriptor);
}
