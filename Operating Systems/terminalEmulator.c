#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <termios.h>

#define MAXARGS 10
#define VEL_BUF 300
#define MAX_HIST 100
#define MAX_BG 100

typedef struct { pid_t pid; char naredba[VEL_BUF]; } PozProc;

char *povijest[MAX_HIST]; // polje za povijest naredbi
int broj_povijesti = 0;

PozProc pozadinski_proc[MAX_BG]; // polje za spremljene procese u pozadini
int broj_poz_proc = 0;

struct termios postavke_terminala; // spremamo izvorne postavke terminala
struct sigaction stari_sigint; // spremamo stare postavke za SIGINT

void dodaj_u_povijest(const char *n) { // dodaje naredbu u povijest
    povijest[broj_povijesti++] = strdup(n);
}

void obradi_sigint(int sig) { // obrada SIGINT signala (Ctrl+C)
    printf("\n[signal SIGINT] proces %d primio signal %d\n", getpid(), sig);
}

void obradi_sigchld(int sig) { // obrada SIGCHLD signala
    pid_t z; int status;
    while ((z = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < broj_poz_proc; i++) {
            if (pozadinski_proc[i].pid == z) {
                printf("\n[roditelj %d - SIGCHLD + waitpid] dijete %d zavrsilo s radom\n", getpid(), z);
                for (int j = i; j < broj_poz_proc - 1; j++)
                    pozadinski_proc[j] = pozadinski_proc[j+1];
                broj_poz_proc--;
                break;
            }
        }
    }
}

pid_t pokreni_program(char *naredba[], int u_pozadini, const char *cijela_naredba) {
    pid_t pid_novi;
    if ((pid_novi = fork()) == 0) { // dijete proces
        sigaction(SIGINT, &stari_sigint, NULL); // postavlja obradu signala SIGINT na staru vrijednost
        setpgid(0, 0); // postavlja grupu procesa djeteta
        if (!u_pozadini)
            tcsetpgrp(STDIN_FILENO, getpgid(0)); // ako nije u pozadini, dijete preuzima terminal
        execvp(naredba[0], naredba); // pokreće program
        perror("Nisam pokrenuo program!");
        exit(1);
    } else {
        printf("[roditelj] stvoreno dijete s PID-om: %d\n", pid_novi); // ispisuje PID djeteta
        if (u_pozadini) { // ako je u pozadini, sprema u listu pozadinskih procesa
            pozadinski_proc[broj_poz_proc].pid = pid_novi;
            strncpy(pozadinski_proc[broj_poz_proc++].naredba, cijela_naredba, VEL_BUF - 1);
        }
    }
    return pid_novi;
}

int pokreni_ugradenu(char **argv) {
    if (!strcmp(argv[0], "cd")) // ugrađena naredba "cd"
        chdir(argv[1]);
    else if (!strcmp(argv[0], "ps")) { // ugrađena naredba "ps"
        printf("PID    ime\n");
        for (int i = 0; i < broj_poz_proc; i++)
            printf("%d  %s\n", pozadinski_proc[i].pid, pozadinski_proc[i].naredba);
    } else if (!strcmp(argv[0], "kill")) { // ugrađena naredba "kill"
        pid_t pid = atoi(argv[1]); int sig = atoi(argv[2]);
        for (int i = 0; i < broj_poz_proc; i++)
            if (pozadinski_proc[i].pid == pid) kill(pid, sig);
    } else if (!strcmp(argv[0], "history")) { // ugrađena naredba "history"
        for (int i = 0; i < broj_povijesti; i++)
            printf("    %d  %s\n", i+1, povijest[i]);
    } else return 0;
    return 1;
}

int provjeri_ponavljanje(char *buffer, char **novi_buffer) {
    if (buffer[0] == '!') {
        int broj = atoi(buffer+1);
        *novi_buffer = strdup(povijest[broj-1]);
        printf("%s\n", *novi_buffer);
        return 1;
    }
    return 0;
}

int main() {
    struct sigaction act;
    pid_t pid_novi;
    char buffer[VEL_BUF], *argv[MAXARGS];
    int argc;

    printf("[roditelj %d] krenuo s radom\n", getpid()); // početna poruka

    act.sa_handler = obradi_sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, &stari_sigint); // postavljanje obrade signala SIGINT

    act.sa_handler = obradi_sigchld;
    act.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act, NULL); // postavljanje obrade signala SIGCHLD

    act.sa_handler = SIG_IGN;
    sigaction(SIGTTOU, &act, NULL); // ignorira signal SIGTTOU

    tcgetattr(STDIN_FILENO, &postavke_terminala); // spremamo izvorne postavke terminala

    do {
        printf("[roditelj] unesi naredbu: "); // ispis u konzolu
        fflush(stdout);
        if (!fgets(buffer, VEL_BUF, stdin)) continue; // učitavanje naredbe
        buffer[strcspn(buffer, "\n")] = '\0'; // uklanja znak novog reda

        char *naredba_iz_pov = NULL;
        if (provjeri_ponavljanje(buffer, &naredba_iz_pov)) {
            strncpy(buffer, naredba_iz_pov, VEL_BUF-1);
            free(naredba_iz_pov);
        }

        dodaj_u_povijest(buffer); // dodaje naredbu u povijest

        argc = 0;
        argv[argc] = strtok(buffer, " \t");
        while (argv[argc] && argc < MAXARGS-1)
            argv[++argc] = strtok(NULL, " \t");
        argv[argc] = NULL;

        if (!strcmp(argv[0], "exit")) { // ako korisnik unese "exit"
            for (int i = 0; i < broj_poz_proc; i++)
                kill(pozadinski_proc[i].pid, SIGKILL); // ubija sve procese u pozadini
            break;
        }

        if (pokreni_ugradenu(argv)) continue;

        int u_pozadini = 0;
        if (!strcmp(argv[argc-1], "&")) {
            u_pozadini = 1;
            argv[--argc] = NULL;
        }

        printf("[roditelj] pokrecem program\n"); // poruka da se pokreće program
        pid_novi = pokreni_program(argv, u_pozadini, buffer);

        if (!u_pozadini) {
            printf("[roditelj] cekam da zavrsi\n"); // roditelj čeka dijete
            waitpid(pid_novi, NULL, 0);
            printf("[roditelj] dijete %d završilo s radom\n", pid_novi); // ispis da je dijete završilo
            tcsetpgrp(STDIN_FILENO, getpgid(0));
            tcsetattr(STDIN_FILENO, 0, &postavke_terminala);
        }

    } while (1);

    for (int i = 0; i < broj_povijesti; i++) // oslobađa memoriju
        free(povijest[i]);

    return 0;
}
