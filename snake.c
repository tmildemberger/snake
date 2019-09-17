#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "deck.h"

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#define FOOD '@'

typedef struct fi {
    int width;
    int height;
    char **field;
} Field;

typedef struct snake {
    Deck *d;
    Field *field;
    int size;
    char dir;
} Snake;

int kbhit(void);
Field* create_field(int width, int height);
void initialize_field(Field *field);
void destroy_field(Field *field);
void gen_food(Field *field);
void print_field(Field *field);

int alive_snake(Snake *s);
Snake* create_snake(int size, Field *field);
void reverse_snake(Snake *s);
void dir_snake(Snake *s, char dir);
void move_snake(Snake *s);
void destroy_snake(Snake *s);

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
    for (i = 0; i < field->height; i++) {
        for (j = 0; j < field->width; j++) {
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

void gen_food(Field *field) {
    int x = (rand() % (field->width - 1)) + 1;
    int y = (rand() % (field->height - 1)) + 1;
    while (field->field[y][x] != ' ') {
        x = (rand() % (field->width - 1)) + 1;
        y = (rand() % (field->height - 1)) + 1;
    }
    field->field[y][x] = FOOD;
}

void print_field(Field *field) {
    int i, j;
    printf("  ");
    for (i = 1; i < field->width-1; i++) {
        printf(" #");
    }
    printf("\n");
    for (i = 1; i < field->height-1; i++) {
        printf(" # ");
        for (j = 1; j < field->width-1; j++) {
            printf("%c ", field->field[i][j]);
        }
        printf("#\n");
    }
    printf("  ");
    for (i = 1; i < field->width-1; i++) {
        printf(" #");
    }
    printf("\n");
}

int alive_snake(Snake *s) {
    Point p = getFront(s->d);
    if (p.x < 1 || p.x > s->field->width - 2 ||
        p.y < 1 || p.y > s->field->height- 2)
        return 0;
    else
        return 1;
}

Snake* create_snake(int size, Field *field) {
    Snake *new_snake = (Snake*) malloc(sizeof (Snake));
    new_snake->field = field;
    new_snake->d = createDeck();
    new_snake->size = size;
    new_snake->dir = LEFT;

    /*Inserindo a Snake no centro do campo: */
    int i;
    int center = field->height / 2;
    for (i = (field->width - size) / 2; i <= (field->width + size) / 2; i++) {
        new_snake->d = insertRear(new_snake->d, (Point){i, center});
        field->field[center][i] = '*'; /*Ponto inserido faz parte do corpo da Snake!*/
    }
    return new_snake;
}

void reverse_snake(Snake *s) {
    s->d = reverseDeck(s->d);
}

void dir_snake(Snake *s, char dir) {
    s->dir = dir;
}

void move_snake(Snake *s) {
    Point p = getFront(s->d);
    if (s->dir == UP) --p.y;
    else if (s->dir == DOWN) ++p.y;
    else if (s->dir == LEFT) --p.x;
    else ++p.x;

    printf("x:%d y:%d\n", p.x, p.y);
    if (s->field->field[p.y][p.x] == FOOD) {
        gen_food(s->field);
        s->d = insertFront(s->d, p);
        s->field->field[p.y][p.x] = '*';
        ++s->size;
        return;
    }
    s->d = insertFront(s->d, p);
    s->field->field[p.y][p.x] = '*';

    p = getRear(s->d);
    s->field->field[p.y][p.x] = ' ';
    s->d = deleteRear(s->d);
}

void destroy_snake(Snake *s) {
    destroyDeck(s->d);
    free(s);
}

#define SIZE 20

int main() {
    srand(time(NULL));

    Field *field = create_field(SIZE*2, SIZE);
    initialize_field(field);
    int snake_size = 5;
    Snake *snake = create_snake(snake_size, field);

    gen_food(field);

    char pressionou_prv = ' ';
    char pressionou_act = LEFT;

    while (1) {
        if (kbhit()) {
            /*Cada vez que uma tecla é pressionada o controle executa esse trecho: */
            pressionou_prv = pressionou_act;
            pressionou_act = getchar();
            dir_snake(snake, pressionou_act);
            if ((pressionou_act == DOWN) && (pressionou_prv == UP)) { reverse_snake(snake); }
            else if ((pressionou_act == UP) && (pressionou_prv == DOWN)) { reverse_snake(snake); }
            else if ((pressionou_act == LEFT) && (pressionou_prv == RIGHT)) { reverse_snake(snake); }
            else if ((pressionou_act == RIGHT) && (pressionou_prv == LEFT)) { reverse_snake(snake); }
        }
        move_snake(snake);
        if (!alive_snake(snake)) break;
        printf("ola\n");

        print_field(field);
        usleep(250000);
        system("clear");
    }
    print_field(field);
    printf ("#### Perdeu :(\n");
    printf ("#### Pontuacao final (tamanho da cobrinha): %d\n", snake->size);
    if (snake->size > 16) {
        printf ("#### Obs: :D\n");
    }

    destroy_snake(snake);
    destroy_field(field);
    return 0;
}
