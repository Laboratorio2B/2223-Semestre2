#include "xerrori.h"

// calcola il numero e la somma dei primi contenuti
// alcuni file di testo 
// utilizza dei processi ausiliari per il conteggio
// la comunicazione avviene attraverso 
// shared memory e con il controllo di alcuni semafori 

#define Sommamem "/sommaprimi"
#define Sommasem "/sommaprimi"
#define Sommasem2 "/sommaprimi2"




int main(int argc, char *argv[])
{ 
  if(argc<2) {
    printf("Uso:\n\t%s file1 [file2 file3 ...] \n",argv[0]);
    exit(1);
  }

  // ---- creazione array memoria condivisa
  int shm_size = sizeof(int)+sizeof(long); // uso solo 4+8=12 byte di memoria condivisa (a[0])
  int fd = xshm_open(Sommamem,O_RDWR | O_CREAT, 0660,__LINE__,__FILE__);
  xftruncate(fd, shm_size, __LINE__,__FILE__);
  // int *nump = simple_mmap(shm_size,fd, __LINE__,__FILE__);
  // long *sommap = (long *) (nump + 1);
  // versione alternativa
  char *tmp = simple_mmap(shm_size,fd, __LINE__,__FILE__);
  int *nump = (int *) (tmp);
  long *sommap = (long *) (tmp + 4);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
    
  // ---- creo i semafori
  sem_t *sem_valori = xsem_open(Sommasem,O_CREAT|O_EXCL,0666,1,
                   __LINE__, __FILE__);
  sem_t *sem_apertura = xsem_open(Sommasem2,O_CREAT|O_EXCL,0666,0,
                   __LINE__, __FILE__);



  // faccio partire i processi ausiliari
  int aux = argc-1;            // numero processi ausiliari
  for(int i=0;i<aux;i++) {
    if(xfork(__LINE__,__FILE__)==0) {
      if(execl("sommaprimi_aux.out", "sommaprimi_aux.out", argv[i+1], (char *) NULL)==-1)
        xtermina("exec fallita",__LINE__,__FILE__);
    }
  }

  // quando sono sicuto che i processi ausiliari hanno
  // aperto shm e sem procedo a prenotare la cancellazione
  xshm_unlink(Sommamem,__LINE__, __FILE__); // distrugge shm quando finito
  xsem_unlink(Sommasem,__LINE__, __FILE__); // distrugge sem quando finito  
  xsem_unlink(Sommasem2,__LINE__, __FILE__); // distrugge sem quando finito



  fprintf(stderr, "Io %d ho finito.\n",getpid());
  return 0;
}



