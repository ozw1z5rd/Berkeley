/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Semplicissimo listener UDP
 *
 * lanciare questo programma PRIMA di avviare il sender UDP.
 *
 * Ing. Alessio Palma
 *
 * $Id: ex2-recv.c,v 1.1 2002/05/26 19:30:14 alessiop Exp $
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
     int socket_descriptor, i, bytes_read;
     struct sockaddr_in socket_address, remote_address;
     socklen_t addr_len;
		 
     char buffer[ 255 ];
     
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
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
		
     for (i=0; i<20; i++) {
			
		/*
		 * SE NON SONO PRESENTI DATI IL PROGRAMMA LI ATTENDE
		 * 
		 */
					 			
	 bytes_read = recvfrom( socket_descriptor, buffer, 255,0, 
	                        (struct sockaddr *) &remote_address, &addr_len
	              );	
							
	printf(">letti %d bytes : %s\n", bytes_read, buffer);
    }
		 
    close(socket_descriptor);
}

