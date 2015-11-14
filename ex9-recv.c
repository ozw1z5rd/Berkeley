/*---------------------------------------------------------------------------
 * Programmazione di rete
 *
 * Listener UDP
 *
 * Questo programma termina quanto riceve un pacchetto con il messaggio
 * pari a QUIT_STRING.
 * Riceve in multicast e peremtte a pi� processi di usare lo stesso numero di porta.
 *
 * avviare con:  ./recv indirizzoIP numero_porta
 *
 * Ing. Alessio Palma
 * (C) Know How & Technologies s.a.s. (khet.net)
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
struct type_pacchetto_v2 pacchetto;
int reuse = 1;
struct ip_mreq ipmreq;

/*------------------------------------------------------------------------
 *
 * prototipi
 */

void intialize_socket(char *,  int );
void print_message( void );
void read_message( void );
int user_exit( void );
unsigned long int checksum( char *, unsigned int);
void message_delete( void );
extern int errno;
/*------------------------------------------------------------------------
 *
 * initialize_socket( numero di porta )
 *
 * inizializza per prepara il socket collegandolo alla porta desiderata
 */

void initialize_socket( char *IPaddress, int port)
{

 // Controlla che l'indirizzo sia di classe D

    if(!IN_MULTICAST(htonl(inet_addr(IPaddress)))) {
   	  printf("L'Indirizzo IP fornito deve appartenere alla classe MULTICAST\n");
 	  exit(1);
    }

// Apre un socket

     if ( ( socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
          printf("Errore: non posso aprire il socket\n");
          exit(1);
     }

// imposta l'indirizzo del socket

     socket_address.sin_family = AF_INET;
     socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
     socket_address.sin_port = htons( port );

// imposta il gruppo da cui si intende ricevere UDP in multicast

     inet_aton(IPaddress, &ipmreq.imr_multiaddr);

// imposta  l'interfaccia sulla quale sar� avviata la trasmissione multicast
// ovvero la prima disponibile

     ipmreq.imr_interface.s_addr = htonl(INADDR_ANY);

// Permette di collegare il processo allo stesso numero di porta anche se gi�
// in uso, da impostare prima di eseguire il bind()
     if ( setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEPORT,
                     &reuse, sizeof(reuse)) < 0 ) {
          printf("Errore: non posso impostare il riuso del numero di porta\n");
	   printf("setsockopt::%d\n", errno);
       exit(1);
     }

// Fa richiesta di adesione al gruppo voluto.
     if ( setsockopt(socket_descriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                     &ipmreq, sizeof(ipmreq)) < 0 ) {
       printf("Errore: non posso aderire al gruppo %s \n", IPaddress);
	   printf("setsockopt::%d\n", errno);
       exit(1);
     }

// finalmente il socket � collegato
     if ( bind( socket_descriptor, (struct sockaddr *) &socket_address,
                sizeof(socket_address) ) < 0 ) {
          printf("Errore: non posso collegare il socket!\n");
	  printf("bind::%d\n", errno);
          exit(1);
     }
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

     for (uint_i=0; uint_i<uint_lunghezza; uint_i++)
	      ulint_tempChk += *(pointer++);

     return ulint_tempChk;
}

/*------------------------------------------------------------------------
 *
 * void delete_message( void )
 *
 * annulla il pacchetto corrente perch� non valido.
 */

void delete_message( void )
{

     strcpy(pacchetto.nome,"ERRORE\n");
     strcpy(pacchetto.messaggio,"E' stato ricevuto un pacchetto anomalo!!\n");
     pacchetto.uint_seqNum = 0;
}


/*------------------------------------------------------------------------
 *
 * read_message()
 *
 * legge il messaggio presente nel buffer
 */

void read_message()
{
     addr_len = sizeof(remote_address)/sizeof(char);
     bytes_read = recvfrom( socket_descriptor,(void *)&pacchetto,
                            sizeof(pacchetto)/sizeof(char),0,
                            (struct sockaddr *)&remote_address, &addr_len );

     if ( bytes_read != -1 ) {
         if ( ( pacchetto.int_versione != 2 )
	           && ( checksum( pacchetto.messaggio,100)!= pacchetto.ulint_crc )
	           && ( bytes_read != (sizeof(pacchetto)/sizeof(char)) )
	      ) delete_message();
     	   else {
	           pacchetto.messaggio[MAXLEN_MSG-1] = 0x00;
		       pacchetto.nome[MAXLEN_USRNAME-1] = 0x00;
		 }
	 } else { printf("(!) recvfrom error\n"); }
}

/*------------------------------------------------------------------------
 *
 * print_message()
 *
 * stampa il messaggio caricato nelle strutture
 */

void print_message( void )
{
     printf("seqNum: %4u da %s porta %d\n", pacchetto.uint_seqNum,
            inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port));
     printf("Mittente: %s ", pacchetto.nome);
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
     return ( strncmp( pacchetto.messaggio, QUIT_STRING, 4) == 0 );
}



main(int argc, char **argv )
{

    initialize_socket( argv[1], atoi(argv[2]) );


     while( ! user_exit() ) {
          read_message();
	      print_message();
     }

     close(socket_descriptor);
}