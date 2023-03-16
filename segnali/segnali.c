#include "xerrori.h"
#define QUI __LINE__,__FILE__

// esempio base gestione segnali con handler
// ogni processo ha un unico thread


// variabili globali utilizzate da main signal handler 
int tot_segnali = 0;
// il perche' della keyword volatile lo vediamo a lezione
volatile bool continua = true;

// funzione che viene invocata quando viene ricevuto 
// un segnale USR1 USR2 o INT (Control-C)
void handler(int s)
{
  tot_segnali++;
  printf("Segnale %d ricevuto dal processo %d\n", s, getpid());
  if(s==SIGUSR2) {
    // dopo aver fatto girare il programma la prima volta
    // s-commentate l'istruzione kill
    // kill(getpid(),SIGUSR1);
    continua = false;
  } 
}

int main(int argc, char *argv[])
{
  // definisce signal handler 
  struct sigaction sa;
  sa.sa_handler = &handler;
  // setta sa.sa_mask che è la maschera di segnali da bloccare
  // durante l'esecuzione di handler. Blocca tutti i segnali
  // tranne SIGUSR1
  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask,SIGUSR1);     
  sa.sa_flags = SA_RESTART;     // restart system calls 
  sigaction(SIGUSR1,&sa,NULL);  // handler per USR1
  sigaction(SIGUSR2,&sa,NULL);  // stesso handler per USR2
  sigaction(SIGINT,&sa,NULL);   // stesso handler per Control-C

  // fork();
  
  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());
  
  // entra in orribile busy waiting attendendo i segnali
  continua = true;
  do { // loop apparentemente senza uscita
    sleep(1000);   
    puts("Mi sono svegliato");            
  } while(continua); 
  printf("Ricevuti: %d segnali\n", tot_segnali);   
  return 0;
}

