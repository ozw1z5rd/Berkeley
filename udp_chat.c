/*
 * Programma di chat che utilizza il protocollo UDP per inviare messaggi
 * 
 * Questo programma utilizza la libreria ncurses, è facile che sulla
 * macchina su cui va in funzione sia attivo X e questo permette all'utente
 * di modificare le dimensioni della console come preferisce. Questo software
 * controlla e rileva il numero di righe e di colonne disponibili all'avvio e
 * modificare la dimensione dello schermo dopo che il software è stato mandato in
 * esecuzione potrebbe risultare in piccoli disfunzionamenti estetici.
 *
 * (C) Ing. Alessio Palma 2002
 * Tutti i diritti riservati
 *
 * Questo software è sotto licenza GNU
 *
 */
 

#include<stdio.h>         /* printf, scanf, etc... */
#include<sys/types.h>     /* FD_ etc... */
#include<netinet/in.h>    /* protocols and datatypes */
#include<arpa/inet.h>     /* Host to Network conversions */
#include<netdb.h>         /* gethostbyname */
#include<sys/socket.h>    /* Berkely sockets */ 
#include"esempi.h"        /* definizioni per il progetto */
#include<ncurses.h>       /* magiche librerie per la gestione dei terminali */
#include<ctype.h>         /* isalpha */
#include<fcntl.h>         /* per impostare i socket in modalità non bloccante */


int initialize_socket( char *, unsigned int, unsigned int );
void read_message( void );
int recv_message( void );
void send_message( void );
void ask_usrname( void );
unsigned long int checksum( char *, unsigned int);
int initialize_ncurses( void );
void print_message( void );
int user_exit( void );
void message_delete( void );


/*------------------------------------------------------------------------
 * 
 * Dichiarazioni variabili
 *
 */

int socket_descriptor;		 		 
struct sockaddr_in socket_address, remote_address;

/* attenzione al nuovo tipo di pacchetto */
struct type_pacchetto_v2 pacchetto_r, pacchetto_s;

/* vale 1 se l'utente vuole terminare */
int user_quit = 0;

/* numero di sequenza */
unsigned int uint_seqNum = 0;

/* servono per la gestione delle finestre */
WINDOW *win_output, *win_input, *win_border1, *win_border2;

/* numero di porta al quale il progamma ascolterà i messaggi */
unsigned int uint_listenAt = 0;

/*numero di porta verso cui invierà i messaggi */
unsigned int uint_port = 0;

/* questa stringa conterrà l'indirizzo IP oppure il nome dello'host */
char str_hostName[MAXLEN_HOSTNAME] = "";

/* questa stringa conterrà il nome dell'utente */
char str_userName[MAXLEN_USRNAME] = DFLT_USERNAME;

/* posizione corrente all'interno della stringa messaggio del pacchetto dati */
int int_posizione = 0;

/* messagio pronto? */
int int_composed;

/* numero di sequenza locale */
unsigned int uint_localSeqNum = -1 ;

/*-----------------------------------------------------( initialize_socket )----
 * int initialize_socket( void )
 *
 * inizializza il socket e lo collega alla porta ed indirizzo IP voluti, servono
 * tre parametri : indirizzo IP dell'host remoto, numero di porta della macchina 
 * remota porta ed infine il numero di porta da usare in locale e verso cui gli 
 * altri invieranno i messaggi
 *
 * se tutto procede per il meglio restituisce 0
 *
 *---------------------------------------------------------------------------*/
 
 
int initialize_socket(char *IPaddress, unsigned int uint_port, unsigned int uint_listen )
{

struct	hostent	*host;

     if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
          return -1;
     }

#ifdef __DEBUG     
     printf(MSG_SOCKETOPEN);
#endif
     
     /* Socket non bloccanti */
          
  	 fcntl( socket_descriptor, F_SETFL, O_NONBLOCK);
		 
		 /* socket che il programma alloca ed userà	per ricevere pacchetti */
						
     socket_address.sin_family = AF_INET;
     socket_address.sin_port = htons( uint_listen );

#ifdef __DEBUG     
     printf(MSG_LOCALPORTBIND); 
#endif
		 
		 /* socket ed indirizzo IP verso cui spedire i dati */
			 
		 remote_address.sin_family = AF_INET;
     remote_address.sin_port = htons( uint_port );
     memset(&remote_address.sin_zero, 0x00, sizeof(remote_address)/sizeof(char));
     
     /* 
      * l'host non è un indirizzo IP, si deve usare il DNS e non è detto
      * che tutto si risolva per il meglio.
      */     

    if ( (int)inet_addr(IPaddress) == INADDR_NONE ) 
         if ( (host = gethostbyname( IPaddress )) == NULL ) {
              printf(MSGERR_HOSTNOTFOUND);
              return -1;
         } else remote_address.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr ;
    else inet_aton( IPaddress, &remote_address.sin_addr );


#ifdef __DEBUG
     printf(MSG_REMOTEBIND); 
#endif

     if ( bind( socket_descriptor, (struct sockaddr *)&socket_address, sizeof(socket_address) ) ) {
          return -1; 
     } 
     
     return 0;
}

/*-------------------------------------------------------------( checksum )----
 * unsigned long int checksum( char *, unsigned int )
 *
 * calcola il checksum di un vettore di caratteri
 *-------------------------------------------------------------------------*/
 
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

/*------------------------------------------------------------( send_message )-
 * send_messagge( void )
 *
 * Invia la struttura pacchetto in rete.
 *-------------------------------------------------------------------------*/
 
void send_message( void)
{
     pacchetto_s.ulint_crc = checksum( pacchetto_s.messaggio , MAXLEN_MSG );
		 pacchetto_s.uint_seqNum++;
     sendto( socket_descriptor, (void *)&pacchetto_s, sizeof(pacchetto_s)/sizeof(char),0,(struct sockaddr *)&remote_address,sizeof(remote_address));     
}



/*----------------------------------------------------( initialize_status )----
 * void initialize_status( void )
 *
 * inizializza il programma principale e le librerie ncurses
 * i socket sono inizializzati in questo modulo.
 *-------------------------------------------------------------------------*/
 
int initialize_ncurse( void )
{

  unsigned int uint_screen_rows, uint_screen_cols;
 
		 pacchetto_s.int_versione = 2;
		 pacchetto_s.uint_seqNum = 0;
     strcpy(pacchetto_s.nome, str_userName);
           
     initscr();
     uint_screen_rows = LINES; uint_screen_cols=COLS;
     
     
     /*
      * Disattiva il buffering dei caratteri e lascia che il driver del
      * terminale elabori i caratteri di controllo
      */
      
    cbreak(); 
     
     /*
      * nessun echo sullo schermo dei tasti premuti, è a carico di questo
      * programma aggiornare il video
      */
      
    win_border1   = newwin(uint_screen_rows/1.5, uint_screen_cols,0,0);
    box(win_border1,0,0);
    wprintw(win_border1,"recv");
    wrefresh(win_border1);
                    
    win_border2   = newwin(uint_screen_rows/3, uint_screen_cols,uint_screen_rows/1.5,0);
    box(win_border2,0,0);
    wprintw(win_border2,"send");
    wrefresh(win_border2);

    win_output = newwin(uint_screen_rows/1.5-2, uint_screen_cols-2,1,1);
    scrollok(win_output, TRUE); /* lo scroll è ammesso */
    wrefresh(win_output);
                    
    win_input = newwin(uint_screen_rows/3-2, uint_screen_cols-2,uint_screen_rows/1.5+1,1);
    scrollok(win_input, TRUE);
    notimeout(win_input, FALSE);
    wtimeout(win_input, 10);
    wrefresh(win_input);
 
    return 0;      
}


/*--------------------------------------------------------------( end_program )-
 * void end_program( void )
 *
 * operazioni da compiere prima di terminare il programma
 *
 *-------------------------------------------------------------------------*/
void end_program( void )
{
    /* -----------------------> chiusura ncurses <---*/
    delwin(win_input);
    delwin(win_output);
    delwin(win_border1);
    delwin(win_border2);
    refresh();
    endwin();
    
    /*------------------> fine chiusura ncurses <---*/
    close(socket_descriptor);    
}    
 

/*--------------------------------------------------------( delete_message )---
 * 
 * void delete_message( void )
 *
 * annulla il pacchetto corrente perchè non valido.
 *-------------------------------------------------------------------------*/

void delete_message( void )
{

     strcpy(pacchetto_r.nome,"ERRORE\n");
		 strcpy(pacchetto_r.messaggio,"E' stato ricevuto un pacchetto anomalo!!\n");
		 pacchetto_r.uint_seqNum = 0;
}


/*--------------------------------------------------------( recv_message )---
 * 
 * recv_message()
 *
 * legge il messaggio presente nel socket
 *-------------------------------------------------------------------------*/
 
int recv_message( void )
{
  
static int addr_len;
static int bytes_read=0;
static int int_ok;
   
 		  addr_len = sizeof(remote_address)/sizeof(char);
      bytes_read = recvfrom( socket_descriptor,(void *)&pacchetto_r, sizeof(pacchetto_r)/sizeof(char),0,(struct sockaddr *)&remote_address, &addr_len );
			
			if ( bytes_read != -1 ) {
			
           if ( ( pacchetto_r.int_versione != 2 ) 
			          && ( checksum( pacchetto_r.messaggio, 100 ) != pacchetto_r.ulint_crc )
			          && ( bytes_read != (sizeof(pacchetto_r)/sizeof(char)) )  ) {
							 
			          delete_message();
                int_ok = 0;
          }						
     			else {			
			      			pacchetto_r.messaggio[MAXLEN_MSG-1] = 0x00;
					      	pacchetto_r.nome[MAXLEN_USRNAME-1] = 0x00;
                  int_ok = 1;
                  uint_localSeqNum++;
		     	}
			}
			else {
      
#ifdef __DEBUG 
      printf("(!) recvfrom error\n"); 
#endif

     }					
     
     return ((bytes_read>0) && int_ok);       						
}

/*---------------------------------------------------------( print_message )---
 * 
 * print_message()
 *
 * stampa il messaggio caricato nelle strutture
 *-------------------------------------------------------------------------*/
 
void print_message( void )
{
     if ( uint_localSeqNum == 0 ) uint_localSeqNum = pacchetto_r.uint_seqNum;
     else if ( uint_localSeqNum != pacchetto_r.uint_seqNum) { 
                wprintw(win_output,"(!) Errore numero di sequenza -> Atteso: %d, ricevuto: %d\n",uint_localSeqNum, pacchetto_r.uint_seqNum);
                uint_localSeqNum = pacchetto_r.uint_seqNum;
          }                
          
     wprintw(win_output,">>> seqNum: %4u da %s porta %d mittente : %s\n ", pacchetto_r.uint_seqNum, inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port),pacchetto_r.nome);	
     wprintw(win_output,"%s\n", pacchetto_r.messaggio);

#ifdef __DEBUG     
     wprintw(win_output,"(!) %d, ricevuto: %d\n",uint_localSeqNum, pacchetto_r.uint_seqNum);
#endif
     
     wrefresh(win_output);
          
}

/*-----------------------------------------------------------( user_exit )----
 * 
 * int user_exit()
 *
 * se il programma ha ricevuto QUIT_STRING restuisce
 *--------------------------------------------------------------------------*/
 

int user_exit( void )
{
     return ( strncmp( pacchetto_r.messaggio, QUIT_STRING, 4) == 0 );
}




/*----------------------------------------------------------( parse_args)----
 * 
 * void parse_args( int, **argvv  )
 *
 * elabora i parametri passati al programma e li carica le varibili gloabali 
 *
 *-------------------------------------------------------------------------*/
void parse_args( int int_items, char **ary_item)
{
    while (int_items > 1) {
    
      --int_items;
      
      if ( ! strcmp( ary_item[ int_items ] , OPTION_HOST ) ) {
        /* aumenta la robustezza */
        strncpy( str_hostName, ary_item[int_items+1] , MAXLEN_HOSTNAME);
        str_hostName[ MAXLEN_HOSTNAME -1 ] = 0x00;
      } else if ( ! strcmp( ary_item[ int_items ] , OPTION_PORT ) ) {
                uint_port = atoi( ary_item[int_items+1] );
             } else if ( ! strcmp( ary_item[ int_items ] , OPTION_LISTEN ) ) {
                        uint_listenAt = atoi( ary_item[int_items+1]);
                    } else if ( ! strcmp( ary_item[ int_items ] , OPTION_USERNAME ) ) {
                                strncpy( str_userName, ary_item[int_items+1],MAXLEN_USRNAME );
                                str_userName[MAXLEN_USRNAME-1]= 0x00;
                           }   
    } /* whilend */
    
    /*
     *  se i numeri di porta non sono forniti oppure sono errati imposta quelli
     *  di default. Se si controlla il software sulla stessa macchina è necessario
     *  specificare almeno il numero di porta in uno dei due programmi
     */
     
   uint_port = ( ( uint_port < 1025 ) || ( uint_port > 65535 ) ) ? DFLT_PORT : uint_port; 
   uint_listenAt = ( (uint_listenAt < 1025) || (uint_port > 65535 ) ) ? DFLT_PORT :  uint_listenAt;
   
   if ( strlen(str_hostName ) == 0 ) {
        printf(MSGERR_NOHOSTNAME);
        exit(1);
   }
   
}


/*-----------------------------------------------------( compose_message)----
 * 
 * void compose_message( void )
 *
 * costruisce la stringa che sarà inviata come messaggio. 
 *
 *-------------------------------------------------------------------------*/
 void compose_message( void )
 {
    static char c;
    
    if (int_composed) {
        int_composed = int_posizione = 0;
        wprintw(win_input,"%s>>> Message sent\n\n", pacchetto_s.messaggio);
        memset( pacchetto_s.messaggio, 0x00, MAXLEN_MSG);              
    }
    
    if ( (c = wgetch(win_input)) != ERR ) {
    
        if ( isascii(c) ) pacchetto_s.messaggio[int_posizione++] = c;
        if ( (int_posizione == MAXLEN_MSG) || (c == '\n') ) int_composed = 1;
              
   }
}
 

/* =========================================================================*/

main(int argc, char **argv )
{  

     parse_args( argc, argv);

     if ( initialize_socket( str_hostName, uint_port, uint_listenAt) ) {
          printf(MSGERR_CANNOTINITIALIZE);
          exit(1);
     }
          
		 if ( initialize_ncurse() ) { 
          printf(MSGERR_CANNOTINITIALIZE);
          exit(1);
     };        
		 
		 while( ! user_exit()  ) {
      
		    if ( recv_message() ) print_message();
        compose_message();
        /* int_composed vale 1 se si può spedire il testo immesso */ 
        if ( int_composed ) send_message(); 
		}
					      							
     end_program();                              	
}

