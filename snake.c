#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "deck.h"

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

int kbhit(void);
void initialize_field(char field[SIZE][SIZE]);
void print_field(char field[SIZE][SIZE]);
int finish(Deck *d);

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
    ungetc(ch, stdin);
    return 1;
    }
    return 0;
}

void initialize_field(char field[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            field[i][j] = ' ';
        }
    }
}

void print_field(char field[SIZE][SIZE]) {
    int i, j;
    printf("  ");
    for (i = 0; i < SIZE; i++) {
        printf(" #");
    }
    printf("\n");
    for (i = 0; i < SIZE; i++) {
        printf(" # ");
        for (j = 0; j < SIZE; j++) {
            printf("%c ", field[j][i]);
        }
        printf("#\n");
    }
    printf("  ");
    for (i = 0; i < SIZE; i++) {
        printf(" #");
    }
    printf("\n");
}

int finish(Deck *d) {
    Point p = getFront(d);
    if (p.x < 1 || p.x > SIZE-2 || p.y < 1 || p.y > SIZE-2)
        return 1;
    else
        return 0;
}

int main() {
    char field[SIZE][SIZE];
    initialize_field(field);
    int center = SIZE/2;
    int snake_size = 5;
    Deck *d = createDeck();

    /*Inserindo a Snake no centro do campo: */
    int i;
    for (i = center-snake_size/2; i <= center+snake_size/2; i++) {
        d = insertRear(d, (Point){i, center}, field);
    }

    // printf("Tamanho: %d, centro: %d;\n", SIZE, center);
    // Deck *id = d;
    // int ix = 0;
    // for (;id != NULL; ++ix, id = id->next) {
    //     printf("Pos %d: x=%d y=%d\n", ix, id->p.x, id->p.y);
    // }
    // d = reverseDeck(d);
    // printf("\nInvertido:\n");
    // id = d;
    // ix = 0;
    // for (;id != NULL; ++ix, id = id->next) {
    //     printf("Pos %d: x=%d y=%d\n", ix, id->p.x, id->p.y);
    // }
    // return 0;

    char pressionou_prv = ' ';
    char pressionou_act = LEFT;

    while (!finish(d)) {
        while ((!kbhit()) && (!finish(d))) {
            Point p = getFront(d);
            if (pressionou_act == UP) { d = insertFront(d, (Point){p.x, p.y-1}, field); }
            else if (pressionou_act == DOWN) { d = insertFront(d, (Point){p.x, p.y+1}, field); }
            else if (pressionou_act == LEFT) { d = insertFront(d, (Point){p.x-1, p.y}, field); }
            else { d = insertFront(d, (Point){p.x+1, p.y}, field); }
            d = deleteRear(d, field);
            // }
            print_field(field);
            usleep(250000);
            system("clear");
        }
        if (!finish(d)) {
            /*Cada vez que uma tecla é pressionada o controle executa esse trecho: */
            pressionou_prv = pressionou_act;
            pressionou_act = getchar();
            if ((pressionou_act == DOWN) && (pressionou_prv == UP)) { d = reverseDeck(d); }
            else if ((pressionou_act == UP) && (pressionou_prv == DOWN)) { d = reverseDeck(d); }
            else if ((pressionou_act == LEFT) && (pressionou_prv == RIGHT)) { d = reverseDeck(d); }
            else if ((pressionou_act == RIGHT) && (pressionou_prv == LEFT)) { d = reverseDeck(d); }
        }
    }
    printf ("#### Perdeu :(  \n");
    return 0;
}
