#include "xerrori.h"

// programma per il conteggio di numero dei primi in un
// file di testo. 
// Usa shared memory e semafori per comunicare i risultati al
// genitore

#define Sommamem "/sommaprimi"
#define Sommasem "/sommaprimi"
#define Sommasem2 "/sommaprimi2"


// restituisce true/false a seconda che n sia primo o composto
bool primo(int n)
{
  if(n<2) return false;
  if(n%2==0) {
    if(n==2)  return true;
    else return false; }
  for (int i=3; i*i<=n; i += 2) 
      if(n%i==0) return false;
  return true;
}


int main(int argc, char *argv[]) {
  if(argc!=2) {
    printf("Uso:\n\t%s nomefile\n",argv[0]);
    exit(1);
  }
  fprintf(stderr, "Processo %d (figlio di %d) terminato\n",getpid(),getppid());
  return 0;
}

