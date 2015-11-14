/*
 * esempi.h 
 *
 * Ing. Alessio Palma 2002
 *
 * strutture dati comuni ai vari moduli
 */
 
#define MAXLEN_MSG       255
#define MAXLEN_USRNAME   80
#define MAXLEN_HOSTNAME  80 
#define QUIT_STRING      "quit"
#define DFLT_PORT        49152
#define DFLT_USERNAME    "Anonymous"

#define OPTION_HOST      "-h"
#define OPTION_PORT      "-p"
#define OPTION_LISTEN    "-l"
#define OPTION_USERNAME  "-u"

#define MSGERR_NOHOSTNAME  "Hostname o indirizzo IP non fornito. Impossibile continuare\n" 
#define MSGERR_CANNOTINITIALIZE "Inizializzazione del software fallita\n"
#define MSGERR_HOSTNOTFOUND "HOST non trovato\n"


#define MSG_LOCALPORTBIND "Porta collegata\n"
#define MSG_REMOTEBIND "Indirizzo IP remoto colletato\n"
#define MSG_SOCKETOPEN "Socket Aperto\n"


typedef struct type_pacchetto {      
			 char nome[MAXLEN_USRNAME];
			 char messaggio[MAXLEN_MSG];
} type_pacchetto;

/*
 * nuovo formato di pacchetto.
 *
 */

typedef struct type_pacchetto_v2 {      
			 char nome[MAXLEN_USRNAME];    /* nome mittente */
			 char messaggio[MAXLEN_MSG];   /* messaggio */
			 
			 int int_versione;              /* versione del pacchetto */
			 unsigned long int ulint_crc;   /* CRC */
       unsigned long int uint_seqNum; /* numero di sequenza */			  
} type_pacchetto_v2;
