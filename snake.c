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

#define SIZE 20

typedef struct fi {
    int width;
    int height;
    char **field;
} Field;

int kbhit(void);
Field* create_field(int width, int height);
void initialize_field(Field *field);
void destroy_field(Field *field);
void print_field(Field *field);
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
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

Field* create_field(int width, int height) {
    Field *new_field = (Field*) malloc(sizeof (Field));
    new_field->width = width;
    new_field->height = height;
    new_field->field = (char**) malloc(sizeof (char*) * height);
    int i;
    for (i = 0; i < height; ++i) {
        new_field->field[i] = (char*) malloc(sizeof (char) * width);
    }
    return new_field;
}

void initialize_field(Field *field) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            field->field[i][j] = ' ';
        }
    }
}

void destroy_field(Field *field) {
    int i;
    for (i = 0; i < field->height; ++i) {
        free(field->field[i]);
    }
    free(field->field);
    free(field);
}

void print_field(Field *field) {
    int i, j;
    printf("  ");
    for (i = 1; i < SIZE-1; i++) {
        printf(" #");
    }
    printf("\n");
    for (i = 1; i < SIZE-1; i++) {
        printf(" # ");
        for (j = 1; j < SIZE-1; j++) {
            printf("%c ", field->field[j][i]);
        }
        printf("#\n");
    }
    printf("  ");
    for (i = 1; i < SIZE-1; i++) {
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
    Field *field = create_field(SIZE, SIZE);
    initialize_field(field);
    int center = SIZE/2;
    int snake_size = 5;
    Deck *d = createDeck();

    /*Inserindo a Snake no centro do campo: */
    int i;
    for (i = center-snake_size/2; i <= center+snake_size/2; i++) {
        d = insertRear(d, (Point){i, center});
        field->field[i][center] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/
    }

    char pressionou_prv = ' ';
    char pressionou_act = LEFT;

    while (1) {
        if (kbhit()) {
            /*Cada vez que uma tecla é pressionada o controle executa esse trecho: */
            pressionou_prv = pressionou_act;
            pressionou_act = getchar();
            if ((pressionou_act == DOWN) && (pressionou_prv == UP)) { d = reverseDeck(d); }
            else if ((pressionou_act == UP) && (pressionou_prv == DOWN)) { d = reverseDeck(d); }
            else if ((pressionou_act == LEFT) && (pressionou_prv == RIGHT)) { d = reverseDeck(d); }
            else if ((pressionou_act == RIGHT) && (pressionou_prv == LEFT)) { d = reverseDeck(d); }
        }
        Point p = getFront(d);
        if (pressionou_act == UP) { d = insertFront(d, (Point){p.x, p.y-1});
        field->field[p.x][p.y-1] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/ }
        else if (pressionou_act == DOWN) { d = insertFront(d, (Point){p.x, p.y+1});
        field->field[p.x][p.y+1] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/ }
        else if (pressionou_act == LEFT) { d = insertFront(d, (Point){p.x-1, p.y});
        field->field[p.x-1][p.y] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/ }
        else { d = insertFront(d, (Point){p.x+1, p.y});
        field->field[p.x+1][p.y] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/ }

        if (finish(d)) break;
        p = getRear(d);
        field->field[p.x][p.y] = ' '; /*Ponto eliminado agora é marcado como vazio!*/
        d = deleteRear(d);


        print_field(field);
        usleep(250000);
        system("clear");
    }
    printf ("#### Perdeu :(  \n");
    return 0;
}
