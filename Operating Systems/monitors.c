#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

int u_sobi = 0;   //broj studenata unutra
bool party_u_sobi = false;   //je li partibrejker unutra
int rem = 0;   //koliko studenata još nije odradilo 3 posjeta

pthread_mutex_t m;     //mutex za pristup stanju ulaza/izlaza
pthread_cond_t cs;    //studenti čekaju ovdje
pthread_cond_t cp;    //partibrejker čeka ovdje

int rnd(int min, int max) {
    return min + rand() % (max - min + 1);
}

void *student(void *p) {
    for (int r = 0; r < 3; r++) {
        int id = (int)(long)p;
        usleep(rnd(100, 500) * 1000);

        pthread_mutex_lock(&m);
        while (party_u_sobi)
            pthread_cond_wait(&cs, &m);

        u_sobi++;
        printf("Student %d je usao (u_sobi=%d)\n", id, u_sobi);
        if (u_sobi >= 3)
            pthread_cond_signal(&cp);
        pthread_mutex_unlock(&m);

        usleep(rnd(1000, 2000) * 1000);

        pthread_mutex_lock(&m);
        u_sobi--;
        printf("Student %d je izasao (u_sobi=%d)\n", id, u_sobi);
        if (u_sobi == 0 && party_u_sobi)
            pthread_cond_signal(&cp);
        pthread_mutex_unlock(&m);

        usleep(rnd(1000, 2000) * 1000);
    }

    pthread_mutex_lock(&m); //vise nije aktivan
    rem--;
    if (rem == 0)
        pthread_cond_signal(&cp);
    pthread_mutex_unlock(&m);

    return NULL;
}

void *party(void *arg) {
    (void)arg; //samo mi warninge baca bez ovog
    while (1) {
        usleep(rnd(100, 1000) * 1000);

        pthread_mutex_lock(&m);
        if (rem == 0) {
            pthread_mutex_unlock(&m);
            break;
        }
        while (u_sobi < 3 && rem > 0)
            pthread_cond_wait(&cp, &m);
        if (rem == 0) {
            pthread_mutex_unlock(&m);
            break;
        }

        party_u_sobi = true;
        printf("Partibrejker je usao (u_sobi=%d)\n", u_sobi);
        while (u_sobi > 0)
            pthread_cond_wait(&cp, &m);

        party_u_sobi = false;
        printf("Partibrejker je izasao\n");
        pthread_cond_signal(&cs);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main() {
    int N;
    printf("Unesi broj studenata (>3): ");
    if (scanf("%d", &N) != 1 || N <= 3) {
        printf("Neispravan unos, mora biti >3\n");
        return 1;
    }

    srand(time(NULL));
    rem = N;
    pthread_mutex_init(&m, NULL); //stvaramo sve potrebne monitore
    pthread_cond_init(&cs, NULL);
    pthread_cond_init(&cp, NULL);

    pthread_t t_party; //stvaramo partibrejkera
    pthread_create(&t_party, NULL, party, NULL);

    pthread_t t[N];
    for (int i = 0; i < N; i++) //stvaramo N studenata
        pthread_create(&t[i], NULL, student, (void*)(long)(i + 1));

    for (int i = 0; i < N; i++) //cekamo studente da dovrse posao
        pthread_join(t[i], NULL);

    pthread_mutex_lock(&m); //ako partibrejker ceka, bez ovog moze beskonacno cekat bez da provjeri rem==0
    pthread_cond_signal(&cp);
    pthread_mutex_unlock(&m);

    pthread_join(t_party, NULL); //cekamo partibrejkera

    pthread_mutex_destroy(&m); //brisanje
    pthread_cond_destroy(&cs);
    pthread_cond_destroy(&cp);

    return 0;
}
