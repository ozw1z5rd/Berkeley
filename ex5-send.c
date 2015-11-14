/*---------------------------------------------------------------------------
 * Programmazione di rete 
 * 
 * Semplicissimo sender UDP.
 * Accetta tre parametri e DEVONO ESSERE PASSATI per poter permettere al 
 * programma di funzionare.
 *
 * ./send indirizzoIP numero porta "messaggio"
 *
 * lanciare questo programma DOPO aver avviato il listener UDP.
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

main(int argc, char **argv )
{
     int socket_descriptor;
		 
		 /*
		  * osserviamo che servirà una struttura sock_addr_in per definire 
			* l'indirizzo del socket remoto.
			*
			*/
			 
     struct sockaddr_in socket_address, remote_address;
     
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
		  * socket ed indirizzo IP verso cui spedire i dati
			*
			*/
			 
		 remote_address.sin_family = AF_INET;
     remote_address.sin_port = htons( atoi(argv[2]) );
     inet_aton( argv[1], &remote_address.sin_addr );
		 memset(&remote_address.sin_zero, 0x00, 8);


     if ( bind( socket_descriptor, (struct sockaddr *)&socket_address, sizeof(socket_address) ) ) {
          printf("non posso collegare il socket\n");
          exit(1);
     } 
												
		 sendto( socket_descriptor, argv[3], strlen(argv[3])+1,0,(struct sockaddr *) &remote_address,sizeof(remote_address));	
		 
     close(socket_descriptor);

}
