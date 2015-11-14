/*---------------------------------------------------------------------------
 * Programmazione di rete 
 * 
 * Semplicissimo sender UDP.
 * Questo programma si aspetta in ingresso i parametri indicati qui di seguito:
 *
 * ./send indirizzoIP numero porta
 * 
 * in questa nuova versione viene inviato un pacchetto in un formato differente
 * da quello atteso dal listenet UDP. Grazie ad un particolare tipo di 
 * accorgimento il programma ricevente non si pianta. 
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
/* contiene la struttura comune ai due moduli */
#include"esempi.h"


void initialize_socket( char *, int );
void read_message( void );
void send_message( void );
void ask_usrname( void );
unsigned long int checksum( char *, unsigned int);
void initialize_status( void );

/*------------------------------------------------------------------------
 * 
 * Dichiarazioni variabili
 *
 */

int socket_descriptor;		 		 
struct sockaddr_in socket_address, remote_address;

/* attenzione al nuovo tipo di pacchetto */
struct type_pacchetto_v2 pacchetto;

/* vale 1 se l'utente vuole terminare */
int user_quit = 0;
unsigned int uint_seqNum = 0;

/*----------------------------------------------------------------------
 * Initialize_socket( void )
 *
 * inizializza il socket e lo collega alla porta ed indirizzo IP voluti
 *
 */
 
void initialize_socket(char *IPaddress, int port)
{
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
      */
		 
     remote_address.sin_family = AF_INET;
     remote_address.sin_port = htons( port );
     inet_aton( IPaddress, &remote_address.sin_addr );
     memset(&remote_address.sin_zero, 0x00, 8);


     if ( bind( socket_descriptor, (struct sockaddr *)&socket_address, sizeof(socket_address) ) ) {
          printf("non posso collegare il socket\n");
          exit(1);
     } 

}


/*----------------------------------------------------------------------
 * Initialize_socket( void )
 *
 * carica la struttura pacchetto.
 *
 */
 
void read_message( void ) 
{
     printf("messaggio:");
		 
     fflush(stdin);
     fgets((char *)pacchetto.messaggio, MAXLEN_MSG, stdin );
		 
     user_quit = ( strncmp( pacchetto.messaggio , QUIT_STRING, 4) == 0 ); 
}

/*----------------------------------------------------------------------
 * unsigned long int checksum( char *, unsigned int )
 *
 * calcola il checksum di un vettore di caratteri
 *
 */
 
unsigned long int checksum( char *vettore, unsigned int uint_lunghezza)
{
     unsigned long int ulint_tempChk = 0;
     char *pointer = vettore;
     unsigned int uint_i;

     for (uint_i=0; uint_i<uint_lunghezza; uint_i++) {
		      ulint_tempChk += *(pointer++);
     } 

     return ulint_tempChk;
}

/*----------------------------------------------------------------------
 * send_messagge( void )
 *
 * Invia la struttura pacchetto in rete.
 *
 */
 
void send_message( void)
{
     pacchetto.ulint_crc = checksum( pacchetto.messaggio , MAXLEN_MSG );
     pacchetto.uint_seqNum++;
     sendto( socket_descriptor, (void *)&pacchetto, sizeof(pacchetto)/sizeof(char),0,(struct sockaddr *)&remote_address,sizeof(remote_address));
}


/*----------------------------------------------------------------------
 * void ask_username( void )
 *
 * inizializza la struttura pacchetto con il nome dell'utente
 *
 */
void ask_username()
{
    fflush(stdin);
    printf("Inserisci il tuo nickname:");
    fgets((char *)&pacchetto.nome, MAXLEN_USRNAME, stdin);
    printf("ok\n");
}


/*----------------------------------------------------------------------
 * void initialize_status( void )
 *
 * inizializza le variabili globali
 *
 */
void initialize_status( void )
{
     pacchetto.int_versione = 2;
     pacchetto.uint_seqNum = 0;
}
		 
main(int argc, char **argv )
{
     initialize_socket( argv[1], atoi(argv[2]));
     initialize_status();
		 
     ask_username();
		 
     while( ! user_quit ) {
         read_message();
         send_message();
     }
					      								
     close(socket_descriptor);
}

