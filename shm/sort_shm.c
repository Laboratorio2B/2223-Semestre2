#include "xerrori.h"
#include <time.h>

// algoritmo di sorting con 2 processi e memoria condivisa


// nome della shared memory 
#define Nome "/array"

// funzione di comparazione per qsort
int cmp(const void *a, const void *b)
{
  return *(int*)a - *(int*)b;
}


int main(int argc,char *argv[])
{
  if(argc!=2) {
    fprintf(stderr,"Uso\n\t%s dim_array\n", argv[0]);
    exit(1);
  }
  // conversione input
  int n= atoi(argv[1]);
  if(n<1) termina("dimensione non valida");

  // ---- creazione array memoria condivisa
  int shm_size = n*sizeof(int); // un intero x processo
  int fd = xshm_open(Nome,O_RDWR | O_CREAT, 0660,__LINE__,__FILE__);
  xftruncate(fd, shm_size, __LINE__,__FILE__);
  int *a = simple_mmap(shm_size,fd, __LINE__,__FILE__);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  // xshm_unlink(Nome,__LINE__, __FILE__); // distrugge shm quando finito

  // ---- inizializza array condiviso con interi random
  srand(1); // iniializza numeri casuali con lo stesso seed
  for(int i=0; i<n; i++) 
    a[i] = rand()%100000;


  // ---- crea processo figlio  
  pid_t pid= xfork(__LINE__, __FILE__);
  if (pid != 0) {
  }
  else
  { // processo figlio

    // unmap memoria condivisa perchè ho finito di usarla
    xmunmap(a, shm_size, __LINE__, __FILE__);
    exit(0);
  }
  // genitore aspetta che abbia finito il figlio:
  if(wait(NULL)<0)
    xtermina("Errore waitpid",__LINE__, __FILE__);
    
  
  // unmap memoria condivisa e termina
  xmunmap(a,shm_size,__LINE__, __FILE__);
  return 0;
}



