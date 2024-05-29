#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura nod al Arbore Timp
struct Timp {
    int ora;
    int minut;
    int secunda;
    struct Timp* left;
    struct Timp* right;
};

// Structura nod lista dublu inlantuita
struct Nod {
    char nr_tel[15];
    int apel_primit;
    int apel_efectuat;
    struct Timp* apel; // Pointer la arborele de timp pentru momentul apelului
    struct Nod* urmator;
    struct Nod* precedent;
};

// Structura pentru coada care este de tip lista dublu inlantuita
struct Coada {
    struct Nod* front;
    struct Nod* rear;
};

// creare nod nou pentru arborele de timp
struct Timp* creareTimp(int ora, int minut, int secunda) {
    struct Timp* nod_nou = (struct Timp*)malloc(sizeof(struct Timp));
    nod_nou->ora = ora;
    nod_nou->minut = minut;
    nod_nou->secunda = secunda;
    nod_nou->left = NULL;
    nod_nou->right = NULL;
    return nod_nou;
}

// creare nod nou pentru lista de apeluri
struct Nod* creareNod(char nr_tel[], int apel_primit, int apel_efectuat, int ora, int minut, int secunda) {
    struct Nod* nod_nou = (struct Nod*)malloc(sizeof(struct Nod));
    strncpy(nod_nou->nr_tel, nr_tel, sizeof(nod_nou->nr_tel) - 1);
    nod_nou->nr_tel[sizeof(nod_nou->nr_tel) - 1] = '\0';
    nod_nou->apel_primit = apel_primit;
    nod_nou->apel_efectuat = apel_efectuat;
    nod_nou->apel = creareTimp(ora, minut, secunda);
    nod_nou->urmator = NULL;
    nod_nou->precedent = NULL;
    return nod_nou;
}

// Creare coada noua
struct Coada* creare_Coada() {
    struct Coada* Coada_noua = (struct Coada*)malloc(sizeof(struct Coada));
    Coada_noua->front = NULL;
    Coada_noua->rear = NULL;
    return Coada_noua;
}

// Funcție pentru a adăuga un apel la sfârșitul cozii
void add_final_Coada(struct Coada* coada, char nr_tel[], int apel_primit, int apel_efectuat, int ora, int minut, int secunda) {
    struct Nod* nod_nou = creareNod(nr_tel, apel_primit, apel_efectuat, ora, minut, secunda);
    if (coada->rear == NULL) {
        // Coada este goală
        coada->front = nod_nou;
        coada->rear = nod_nou;
    } else {
        // Coada nu este goală
        coada->rear->urmator = nod_nou;
        nod_nou->precedent = coada->rear;
        coada->rear = nod_nou;
    }
}

// Funcție pentru a elibera memoria ocupată de arborele de timp
void freeArbore(struct Timp* radacina) {
    if (radacina != NULL) {
        freeArbore(radacina->left);
        freeArbore(radacina->right);
        free(radacina);
    }
}

// Funcție pentru a extrage un apel de la începutul cozii
void del_final_coada(struct Coada* coada) {
    if (coada->front == NULL) {
        // Coada este goală
        printf("Coada este goala.\n");
        exit(EXIT_FAILURE);
    }
    struct Nod* frontNode = coada->front;
    coada->front = frontNode->urmator;
    if (coada->front == NULL) {
        // Ultimul element a fost extras, coada este acum goală
        coada->rear = NULL;
    } else {
        // Elimină legătura înapoi pentru frontNode
        coada->front->precedent = NULL;
    }
    // Eliberează memoria ocupată de arborele de timp
    freeArbore(frontNode->apel);
    free(frontNode);
}

// Funcție pentru a afișa apelurile din coadă
void afisare_coada(struct Coada* coada) {
    if (coada->front == NULL) {
        printf("Coada este goala.\n");
    } else {
        struct Nod* nod_curent = coada->front;
        while (nod_curent != NULL) {
            printf("Numar de telefon: %s, Apeluri primite: %d, Apeluri efectuate: %d, Timp apel: %02d:%02d:%02d\n",
                   nod_curent->nr_tel, nod_curent->apel_primit, nod_curent->apel_efectuat,
                   nod_curent->apel->ora, nod_curent->apel->minut, nod_curent->apel->secunda);
            nod_curent = nod_curent->urmator;
        }
    }
}

// Funcție pentru a elibera memoria ocupată de coadă
void freeCoada(struct Coada* coada) {
    while (coada->front != NULL) {
        struct Nod* temp = coada->front;
        coada->front = temp->urmator;
        // Eliberează memoria ocupată de arborele de timp
        freeArbore(temp->apel);
        free(temp);
    }
    free(coada);
}

int main() {
    struct Coada* coada1 = creare_Coada();
    int i, k, h, min, sec;
    char s[15]; // Am modificat dimensiunea bufferului pentru a se potrivi cu dimensiunea câmpului nr_tel din structura Nod

    printf("Intrare:\n");
    do {
        printf("Introducere nr telefon: ");
        scanf("%s", s);
        printf("Ora: ");
        scanf("%d", &h);
        printf("Minut: ");
        scanf("%d", &min);
        printf("Secunda: ");
        scanf("%d", &sec);
        printf("Apel primit(apasa tasta 0) sau efectuat(apasa tasta 1)? ");
        scanf("%d", &k);
        if (k == 0) {
            add_final_Coada(coada1, s, 1, 0, h, min, sec);
        } else {
            add_final_Coada(coada1, s, 0, 1, h, min, sec);
        }
        printf("Doresti sa continui?(apasa tasta 0 pentru a iesi) ");
        scanf("%d", &i);
    } while (i != 0);

    printf("\nIesire:\n");
    afisare_coada(coada1);
    freeCoada(coada1);
    return 0;
}
