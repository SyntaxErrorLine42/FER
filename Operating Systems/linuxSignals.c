#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 5

volatile int TEKUCI_PRIORITET = 0;
int OZNAKA_CEKANJA[N+1] = {0};
int KON[N+1] = {0};

void prekidni_potprogram(int sig) {
    int u_prioritet;
    printf("\nPrioritet prekida (1-%d): ", N);
    scanf("%d", &u_prioritet);

    OZNAKA_CEKANJA[u_prioritet] = 1;
    printf("Registriran prekid prioriteta %d.\n", u_prioritet);

    /*for (int j = 1; j <= 3; j++) {
        printf("PP:%d-3\n", j);
        sleep(1);
    }*/

    int i, max_prioritet;
    while (1) {
        max_prioritet = 0;
        for (i = 1; i <= N; i++) {
            if (OZNAKA_CEKANJA[i] && i > max_prioritet)
                max_prioritet = i;
        }
        if (max_prioritet > TEKUCI_PRIORITET) {
            printf("ZASTAVICE %d%d%d%d%d\n", OZNAKA_CEKANJA[1], OZNAKA_CEKANJA[2], OZNAKA_CEKANJA[3], OZNAKA_CEKANJA[4], OZNAKA_CEKANJA[5]);

            OZNAKA_CEKANJA[max_prioritet] = 0;
            KON[max_prioritet] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = max_prioritet;
            printf("KON[1]=%d\nKON[2]=%d\nKON[3]=%d\nKON[4]=%d\nKON[5]=%d\n", KON[1], KON[2], KON[3], KON[4], KON[5]);

            printf("Obrada prekida prioriteta %d:\n", max_prioritet);
            for (i = 1; i <= 5; i++) {
                printf("%d/5\n", i);
                sleep(1);
            }
            TEKUCI_PRIORITET = KON[max_prioritet];
            printf("Završen prekid prioriteta %d, TEKUCI_PRIORITET=%d.\n", max_prioritet, TEKUCI_PRIORITET);
            KON[max_prioritet]=0;
            /*for (int j = 1; j <= 3; j++) {
                printf("PIP:%d-3\n", j);
                sleep(1);
            }*/
        } else {

            if (u_prioritet <= TEKUCI_PRIORITET)
                printf("Prekid prioriteta %d odgođen, trenutni prioritet je %d.\n", u_prioritet, TEKUCI_PRIORITET);
            break;
        }
    }
    printf("KON[1]=%d\nKON[2]=%d\nKON[3]=%d\nKON[4]=%d\nKON[5]=%d\n", KON[1], KON[2], KON[3], KON[4], KON[5]);
   printf("ZASTAVICE %d%d%d%d%d\n", OZNAKA_CEKANJA[1], OZNAKA_CEKANJA[2], OZNAKA_CEKANJA[3], OZNAKA_CEKANJA[4], OZNAKA_CEKANJA[5]);

}

int main() {
    int iter = 0;
    struct sigaction act;
    act.sa_handler = prekidni_potprogram;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;
    sigaction(SIGINT, &act, NULL);
    printf("Glavni program pokrenut. TEKUCI_PRIORITET=%d.\n", TEKUCI_PRIORITET);

    while (1) {
        iter++;
        printf("Glavni program: iter %d, TEKUCI_PRIORITET=%d.\n", iter, TEKUCI_PRIORITET);
        sleep(1);
    }
    
    return 0;
}
