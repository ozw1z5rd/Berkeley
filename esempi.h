/*
 * esempi.h 
 *
 * Ing. Alessio Palma 2002
 *
 * strutture dati comuni ai vari moduli
 */
 
#define MAXLEN_MSG       255
#define MAXLEN_USRNAME   80 
#define QUIT_STRING      "quit"

typedef struct type_pacchetto {      
			 char nome[MAXLEN_USRNAME];
			 char messaggio[MAXLEN_MSG];
} type_pacchetto;

/*
 * nuovo formato di pacchetto.
 *
 */

typedef struct type_pacchetto_v2 {      
   char nome[MAXLEN_USRNAME];
   char messaggio[MAXLEN_MSG];
   int int_versione;
   unsigned long int ulint_crc;
   unsigned int uint_seqNum;			  
} type_pacchetto_v2;
