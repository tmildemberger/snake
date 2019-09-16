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

Deck* insertFront(Deck *d, Point p) {
    Deck *novo = (Deck*) malloc(sizeof (Deck));
    novo->p = p;
    novo->prev = NULL;
    novo->next = d;
    if (!emptyDeck(d)) {
        d->prev = novo;
    }
    return novo;
}

Deck* insertRear(Deck *d, Point p) {
    Deck *novo = (Deck*) malloc(sizeof (Deck));
    novo->p = p;
    novo->next = NULL;
    novo->prev = NULL;

    if (!emptyDeck(d)) {
        Deck *aux = d;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = novo;
        novo->prev = aux;
        return d;
    }
    return novo;
}

Deck* deleteFront(Deck *d) {
    if (emptyDeck(d)) {
        printf("Deck vazio\n");
        exit(1);
    }
    Deck *aux = d->next;
    if (aux != NULL) {
        aux->prev = NULL;
    }
    free(d);

    return aux;
}

Deck* deleteRear(Deck *d) {
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
        Deck *temp = aux->next;
        aux->next = aux->prev;
        aux->prev = temp;
        aux = aux->next;
    } while (aux != NULL);
    return d;
}
