/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Listener UDP
 *
 * Questo programma termina quanto riceve un pacchetto con il messaggio
 * pari a QUIT_STRING.
 * Il fatto che invia strutture dati lo rende particolarmente fragile. 
 * Leggi l'articolo.
 *
 * avviare con:  ./recv numero_porta
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
#include"esempi.h"


/*------------------------------------------------------------------------
 * 
 * Dichiarazioni variabili
 */
 
int socket_descriptor, i, bytes_read;
unsigned int counter;
struct sockaddr_in socket_address, remote_address;
socklen_t addr_len;
struct type_pacchetto pacchetto;	 

/*------------------------------------------------------------------------
 * 
 * prototipi
 */
 
void intialize_socket( int );
void print_message( void );
void read_message( void );
int user_exit( void );

/*------------------------------------------------------------------------
 * 
 * initialize_socket( numero di porta )
 *
 * inizializza per prepara il socket collegandolo alla porta desiderata
 */
	 
void initialize_socket( int port )
{
     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
          printf("Errore: non posso aprire il socket\n");
          exit(1);
     }
		 
     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( port );

     if ( bind( socket_descriptor, 
		            (struct sockaddr *) &socket_address, sizeof(socket_address) ) 
				) {
          printf("Errore: non posso collegare il socket\n");
          exit(1);
     }

}


/*------------------------------------------------------------------------
 * 
 * read_message()
 *
 * legge il messaggio presente nel buffer
 */
 
void read_message()
{
      bytes_read = recvfrom( socket_descriptor,(void *)&pacchetto, sizeof(pacchetto)/sizeof(char),0,(struct sockaddr *)&remote_address, &addr_len );
}

/*------------------------------------------------------------------------
 * 
 * print_message()
 *
 * stampa il messaggio caricato nelle strutture
 */
 
void print_message( void )
{
     printf("Mittente:%s", pacchetto.nome);	
     printf("Testo: %s", pacchetto.messaggio);
		 printf("------------------------------------------------------------\n");
}

/*------------------------------------------------------------------------
 * 
 * int user_exit()
 *
 * se il programma ha ricevuto QUIT_STRING restuisce 1
 */
 

int user_exit( void )
{
     return ( strncmp( pacchetto.messaggio, QUIT_STRING, 4) == 0 ) ;
}



main(int argc, char **argv )
{
		 initialize_socket( atoi(argv[1]) );
		 		 
     while( ! user_exit() ) {
          read_message();
					print_message();
     }

     close(socket_descriptor);
}

