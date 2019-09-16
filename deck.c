#include "deck.h"
#include <stdio.h>
#include <stdlib.h>

Deck* createDeck(void) {
    return NULL;
}

Point getFront(Deck *d) {
    if (emptyDeck(d)) {
        printf("Deck vazio\n");
        exit(1);
    }
    return d->p;
}

Point getRear(Deck *d) {
    if (emptyDeck(d)) {
        printf("Deck vazio\n");
        exit(1);
    }
    while (d->next != NULL) {
        d = d->next;
    }
    return d->p;
}

Deck* insertFront(Deck *d, Point p, char field[][SIZE]) {
    field[p.x][p.y] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/

    Deck *novo = (Deck*) malloc(sizeof (Deck));
    novo->p = p;
    novo->prev = NULL;
    novo->next = d;
    if (!emptyDeck(d)) {
        d->prev = novo;
    }
    return novo;
}

Deck* insertRear(Deck *d, Point p, char field[][SIZE]) {
    field[p.x][p.y] = '*';  /*Ponto inserido faz parte do corpo da Snake!*/

    Deck *novo = (Deck*) malloc(sizeof (Deck));
    novo->p = p;
    novo->next = NULL;

    Deck *aux = d;
    if (!emptyDeck(d)) {
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = novo;
    } else {
        d = novo;
    }
    novo->prev = aux;
    return d;
}

Deck* deleteFront(Deck *d, char field[][SIZE]) {
    if (emptyDeck(d)) {
        printf("Deck vazio\n");
        exit(1);
    }
    Deck *aux = d->next;
    aux->prev = NULL;

    field[d->p.x][d->p.y] = ' '; /*Ponto eliminado agora é marcado como vazio!*/
    free(d);

    return aux;
}

Deck* deleteRear(Deck *d, char field[][SIZE]) {
    if (emptyDeck(d)) {
        printf("Deck vazio\n");
        exit(1);
    }
    Deck *aux = d;
    while (aux->next != NULL) {
        aux = aux->next;
    }
    if (aux == d) {
        d = NULL;
    }
    field[aux->p.x][aux->p.y] = ' '; /*Ponto eliminado agora é marcado como vazio!*/
    aux->prev->next = NULL;
    free(aux);
    return d;
}

int emptyDeck(Deck *d) {
    return d == NULL;
}

Deck* reverseDeck(Deck *d) {
    if (emptyDeck(d) || d->next == NULL) {
        return d;
    }
    while (d->next != NULL) {
        d = d->next;
    }
    Deck *aux = d;
    do {
        aux->next = aux->prev;
        aux->prev = aux->next;
        aux = aux->next;
    } while (aux != NULL);
    return d;
}
