#include"test_in_c.h"

unsigned short int a,b,e;
short int k1,k2; 
unsigned long int f,g,h;
char c,d;

main()
{
  f=(a=65000);
  g=(b=65000);
  
  e = (long)a*b;
  h = f*g;
  
  
  k1 = 0xfffc;
  k2 = 0x8001;
  
 
  printf("dimensione intero short: %d\n", sizeof(a)/sizeof(char));
  printf("dimensione intero long: %d\n", sizeof(f)/sizeof(char));
 
  printf("k1 è senza segno=%u con segno = %d\n",k1,k1);
  printf("k2 è senza segno=%u con segno = %d\n",k2,k2);
  
  printf("Hello world\n");
  printf(" %c %x %c %x \n",e,e,h,h);

  
  a=0;
  while ( a <= 10) {
    printf("valore di a %d",a);
    a++;
  }
  
  
  if ( a<11) {
    printf("il valore di a è minore di 11");
  }
  else { 
    printf("opk"); 
  }
  
  
  switch ( a ) {
  
    case 10 : printf("switch case 10\n");
              break;
              
    default : {}
    
  }
  
  
  printf("\n");
  scanf("%d",&a);
  
  printf("a = %d",a);
 
}


/*

specifichi a priori che si tratta di un variabile con segno. 

a = 100 (2)    

_ _ _ _ _ _ _ _ _
1 0 0 0 0 0 1 0 0

-4

senza segno

complemento

_ _ _ _ _ _ _ _ _
1 1 1 1 1 1 0 1 1 + 1

1 1 1 1 1 1 1 0 0

0 0 0 0 0 0 0 0 0 - 0 1 1 1 1 1 1 1  = numero positivo 0 <-> 127

1 0 0 0 0 0 0 0 0 - 1 1 1 1 1 1 1 1  = numero negativo. -128 <-> -1 


0 0 0 0 0 0 0 0 1 -> 1 1 1 1 1 1 1 1 0 + 1 -> 1 1 1 1 1 1 1 1 1

1 0 0 0 0 0 0 0 0 - 1 -> 0 1 1 1 1 1 1 1 -> 1 0 0 0 0 0 0 0   
 
*/
