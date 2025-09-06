#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>

sem_t *sem;

#define IDX_TRGOVAC 0
#define IDX_KRUH 1
#define IDX_SUNKA 2  
#define IDX_SIR 3

void Trgovac();
void Kupac(int p);

int main() {
    int ID;
    pid_t pid;

    ID = shmget(IPC_PRIVATE, sizeof(sem_t) * 4, 0600); //dijeljena memorija
    sem = shmat(ID, NULL, 0);
    shmctl(ID, IPC_RMID, NULL);

    sem_init(&sem[IDX_TRGOVAC], 1, 0); //semafor u nizu sem, 1 za procese, 0 je pocetna vrijednost
    sem_init(&sem[IDX_KRUH], 1, 0);
    sem_init(&sem[IDX_SUNKA], 1, 0);
    sem_init(&sem[IDX_SIR], 1, 0);

    printf("Kupac Sendviča 1: ima kruh\n");
    printf("Kupac Sendviča 2: ima šunku\n");
    printf("Kupac Sendviča 3: ima sir\n\n");

    pid = fork(); //stvaramo trgovca
    if (pid == 0) {
        Trgovac();
        exit(0);
    }

    for (int p = 0; p < 3; p++) { //stvaramo prva 3 kupca
        pid = fork();
        if (pid == 0) {
            Kupac(p);
            exit(0);
        }
    }

    while (wait(NULL) > 0) { } //loop, cekamo zavrsetak child procesa

    sem_destroy(&sem[IDX_TRGOVAC]); //brisemo semafore
    sem_destroy(&sem[IDX_KRUH]);
    sem_destroy(&sem[IDX_SUNKA]);
    sem_destroy(&sem[IDX_SIR]);
    shmdt(sem);

    return 0;
}

void Trgovac() {
    const char *sastojci[3] = {"kruh", "šunku", "sir"};
    srand(time(NULL));
    do {
        int i = rand() % 3;
        int j = (i + 1 + rand() % 2) % 3;
        int k = 3 - (i + j);

        printf("Trgovac stavlja: %s i %s\n", sastojci[i], sastojci[j]);

        switch (k) { //postavljanje semafora na osnovu preostalog sastojka
            case 0: sem_post(&sem[IDX_KRUH]);  break;
            case 1: sem_post(&sem[IDX_SUNKA]); break;
            case 2: sem_post(&sem[IDX_SIR]);   break;
        }

        sem_wait(&sem[IDX_TRGOVAC]);
        sleep(1);
    } while (1);
}

void Kupac(int p) {
    sem_wait(&sem[p + 1]); //p+1 zbog 0-indeksiranja
    printf("Kupac Sendviča %d: uzima sastojke, izlazi iz trgovine, slaže si sendvič i jede\n\n", p+1);
    pid_t pid = fork();
    if (pid == 0) { //nacin kako sam ja ovo rjesio je tako da stvorimo novog kupca s istim proizvodom, a starog samo exitamo, to rezultira stvaranjem kupca svake sekunde, jer trgovac svake sekunde  stavlja proizvode
        Kupac(p);
        exit(0);
    }
    sem_post(&sem[IDX_TRGOVAC]); //slobodan ulaz sljedeceg
    return;
}

