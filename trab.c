#include <stdio.h>
#include <stdlib.h>

void dprint(int x) { (x < 10) ? printf("0%d", x) : printf("%d", x); }

void printMap(int *body, int *header, int *side, int size)
{
    // impressão do cabeçalho do mapa
    printf("    ");
    for (int i = 0; i < size; i++)
    {

        int sum = 0;
        for (int ii = 0; ii < size; ii++){sum += body[i + (ii * size)];}

        if (sum != header[i]){
            dprint(header[i]);
            printf(" ");
        }
        else{
            printf("   ");
        }
    }
    printf("\n");

    // impressão da linha de separação
    printf("   ");
    for (int i = 0; i < (size * 3); i++){
        printf("-");
    }
    printf("\n");

    // impressão do corpo e do lado do mapa
    int counter = 0;
    for (int i = 0; i < size; i++){

        int sum = 0;
        for (int ii = 0; ii < size; ii++){sum += body[(i * size) + ii];}

        if (sum != side[i]){
            dprint(side[i]);
            printf(" | ");
        }
        else{
            printf("   | ");
        }

        for (int ii = 0; ii < size; ii++){
            (body[counter] != 0) ? printf("%d  ", body[counter]) : printf("   ");
            counter++;
        }
        printf("\n");
    }
}

void getMap(int *body, int *header, int *side, int *reff, int nMap, int size){
    FILE *fp;

    switch (size) {
        case 4:
            fp = fopen("iniciante.txt", "r");
            break;
        case 6:
            fp = fopen("intermediario.txt", "r");
            break;
        case 7:
            fp = fopen("avancado.txt", "r");
            break;
    }

    while (nMap){
        if (fgetc(fp) == '*') {nMap--; fgetc(fp);}
    }

    for (int i = 0; i < size; i++){
        for (int ii = 0; ii < size; ii++){
            body[(size*i)+ii] = fgetc(fp)-'0';
        }
        fgetc(fp);
    }

    for (int i = 0; i < size; i++){
        int res = 0;
        res += (fgetc(fp)-'0')*10;
        res += fgetc(fp)-'0';
        header[i] = res;
    }
    fgetc(fp);

    for (int i = 0; i < size; i++){
        int res = 0;
        res += (fgetc(fp)-'0')*10;
        res += fgetc(fp)-'0';
        side[i] = res;
    }
    fgetc(fp);

    for (int i = 0; i < size; i++){
        for (int ii = 0; ii < size; ii++){
            reff[(size*i)+ii] = fgetc(fp)-'0';
        }
        fgetc(fp);
    }

}

void getCoord(int *x, int *y){
    char strx[200];
    getX:
        printf("x: ");
        scanf("%[^\n]", strx);
        getchar();

        if (strx[1] != '\0') goto getX;
        if (strx[0]-'0' > 0 && strx[0]-'0' < 10) *x = strx[0]-'1';
        else {goto getX;};

    char stry[200];
    getY:
        printf("y: ");
        scanf("%[^\n]", stry);
        getchar();

        if (stry[1] != '\0') goto getY;
        if (stry[0]-'0' > 0 && stry[0]-'0' < 10) *y = stry[0]-'1';
        else {goto getY;}
}

void supremeGetChar(){while (getchar() != '\n');}

void salvarPlacar(){
    printf("TODO: salvar placar\n");
}

void jogo(int size){    
    int body[size][size];
    int reff[size][size];
    int header[size];
    int side[size];
    getMap(&body[0][0], header, side, &reff[0][0], 1, size);

    int hp = 5;
    int x;
    int y;
    start:
        system("clear");
        printMap(&body[0][0], header, side, size);

        getCoord(&x, &y);

        if (x > (size-1) || y > (size-1)){
            printf("ERR 01: coordenada inválida\n");
            getchar(); 
            jogo(size);
            return;
        }

        if (reff[y][x] == 0){
            //movimento certo
            body[y][x] = 0;

            int sumLine = 0;
            int sumCol = 0;
            for (int i = 0; i < size; i++){
                sumLine += body[y][i];
                sumCol += body[i][x];
            }
            system("clear");
            printMap(&body[0][0], header, side, size); printf("\nacertaste\n");
            if (sumLine == side[y]){printf("fechaste a linha %d\n", y+1);}
            if (sumCol == header[x]){printf("fechaste a coluna %d\n", x+1);}

            int fim = 1;
            for (int i = 0; i < size; i++){
                for (int ii = 0; ii < size; ii++){
                    if (reff[i][ii] == 0 && body[i][ii] != 0){
                        fim = 0;
                    }
                }
            }
            if (fim){
                //fim de jogo
                printf("fim de jogo\n");
                salvarPlacar();
            }
            supremeGetChar();
        }        
        else {
            //movimento errado
            printf("errado\n");
            hp--;
            printf("hp: %d\n", hp);
            if (hp == 0){
                printf("perdeste\n");
                printf("TODO: retornar ao menu\n");
                supremeGetChar();
            }
            supremeGetChar();
        }

    goto start;

    printf("");
}

int main(void)
{
    jogo(6);
}
