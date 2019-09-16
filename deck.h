#ifndef DECK_H
#define DECK_H

#define SIZE 20

typedef struct {
    int x;
    int y;
} Point;

typedef struct deck {
    Point p;
    struct deck *next;
    struct deck *prev;
} Deck;

/*Cria um deck vazio.*/
Deck* createDeck(void);

/*Adiciona um item na cabeça da lista.*/
Deck* insertFront(Deck *d, Point p);

/*Adiciona um item na cauda da lista.*/
Deck* insertRear(Deck *d, Point p);

/*Remove um item da cabeça da lista.*/
Deck* deleteFront(Deck *d);

/*Remove um item da cauda da lista.*/
Deck* deleteRear(Deck *d);

/*Retorna sem remover o item na cabeça da lista.*/
Point getFront(Deck *d);

/*Retorna sem remover o item na cauda da lista.*/
Point getRear(Deck *d);

/*Retorna 1 se o deck está vazio*/
int emptyDeck(Deck *d);

/*Retorna o mesmo deck mas todo ao contrário*/
Deck* reverseDeck(Deck *d);

#endif // DECK_H
