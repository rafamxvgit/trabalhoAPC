#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int diff = 4; //dificuldade do jogo
int eMap = 0;//em qual mapa facil (easy)
int iMap = 0;//em qual mapa intermediário
int aMap = 0; //em qual mapa avançado
int zerou = 0;
char playerName[20];
char nomes[500][20]; //lista dos nomes das pessoas
int points[500]; //lista da pontuação de cada pessoa

void supremeGetChar(){while (getchar() != '\n');}

void welcomeMessage(){
    system("cls");
    printf("'tudo isso que está entre aspas é o título do jogo.'\n pressione <enter> ");
    supremeGetChar();
}

char menu(){
    system("cls");
    printf("1 - jogar\n");
    printf("2 - configurações\n");
    printf("3 - instruções\n");
    printf("4 - ranking\n");
    printf("5 - sair\n\n");   
    printf("digite numero ");
    char ch = getchar();
    supremeGetChar();
    return ch;
}

char config(){
    system("cls");
    printf("1 - zerar ranking\n");
    printf("2 - modo de dificuldade\n");
    printf("3 - voltar ao menu principal\n\n");
    printf("digite numero ");
    char ch = getchar();
    supremeGetChar();
    return ch;
}

void zeraRank(){
    FILE *fp = fopen("ranking.bin", "w");
    fclose(fp);
};

char setDiff(){
    system("cls");
    printf("1 - iniciante\n");
    printf("2 - intermediário\n");
    printf("3 - avançado\n\n");
    printf("digite numero ");
    char ch = getchar();
    supremeGetChar();
    return ch;
};

void instruc(){
    system("cls");
    printf("Sério que você precisa mesmo de instruções? Patético...\n");
    printf("A única coisa que importa é a seguinte: a ordem de entrada das cordenadas é coluna primeiro e depois linha.\n");
    printf("Já falei mais do que eu devia. Agora joga.\n");
    supremeGetChar();
}


void dprint(int x) { (x < 10) ? printf("0%d", x) : printf("%d", x); }

int readRank(){
    FILE *fp = fopen("ranking.bin", "rb");
    if (fp == NULL){
        fp = fopen("ranking.bin", "a+");
        fclose(fp);
        return 0;
    } 
    int index = 0;
    struct pessoa {int points; char name[20];} pes;
    while(fread(&pes, sizeof(pes), 1, fp)){
        strcpy(nomes[index], pes.name);
        points[index] = pes.points;
        index++;
    }
    fclose(fp);
    return index;
}

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

int getCoord(int *x, int *y){
    char strx[200];
    getX:
        printf("x: ");
        scanf("%[^\n]", strx);
        getchar();

        if (strx[0] == 'q') return 0;
        if (strx[1] != '\0') goto getX;
        if (strx[0]-'0' > 0 && strx[0]-'0' < 10) *x = strx[0]-'1';
        else {goto getX;};

    char stry[200];
    getY:
        printf("y: ");
        scanf("%[^\n]", stry);
        getchar();

        if (stry[0] == 'q') return 0;
        if (stry[1] != '\0') goto getY;
        if (stry[0]-'0' > 0 && stry[0]-'0' < 10) *y = stry[0]-'1';
        else {goto getY;}
        return 1;
}

int writeRank(int rankSize){
    FILE *fp = fopen("ranking.bin", "wb"); 
    struct person {int pts; char nome[20];};
    struct person per;
    for (int i = 0; i < rankSize; i++){
        strcpy(per.nome, nomes[i]); per.pts = points[i];
        fwrite(&per, sizeof(per), 1, fp);
    }
    fclose(fp);
    return 0;
}

void sortRank(int rankSize){
    int sorted = 1;
    do {
        sorted = 1;
        for (int i = 1; i < rankSize; i++){
            if (points[i] > points[i-1]){
                int tmp;
                char ctmp[20];
                //swap points
                tmp = points[i];
                points[i] = points[i-1];
                points[i-1] = tmp;

                //swap names
                strcpy(ctmp, nomes[i]);
                strcpy(nomes[i], nomes[i-1]);
                strcpy(nomes[i-1], ctmp);
                sorted = 0;
            }
        }
    } while (!sorted);
}

void salvarPlacar(int size){
    int ptsEarned;
    switch (size){
        case 4:
            ptsEarned = 50;break;
        case 6:
            ptsEarned = 100;break;
        case 7:
            ptsEarned = 150;break;            
    }
    
    int rSize = readRank();
    int achou = -1;
    for (int i = 0; i < rSize; i++){
        if(!strcmp(nomes[i], playerName)){
            achou = i;
            points[i] += ptsEarned;
            break;
        }
    }
    if (achou == -1){
        strcpy(nomes[rSize], playerName);
        points[rSize] = ptsEarned;
        sortRank(rSize+1); //TODO
        writeRank(rSize+1);
    }
    else{
        sortRank(rSize); //TODO
        writeRank(rSize);
    }

}

void jogo(int size){    
    int body[size][size];
    int reff[size][size];
    int header[size];
    int side[size];
    int nMap;
    switch (size) {
        case 4:
            nMap = eMap; break;
        case 6:
            nMap = iMap; break;
        case 7: 
            nMap = aMap; break;
    }
    getMap(&body[0][0], header, side, &reff[0][0], nMap, size);

    int hp = 5;
    int x;
    int y;
    start:
        system("cls");
        printMap(&body[0][0], header, side, size);

        if (!getCoord(&x, &y)){return;}

        if (x > (size-1) || y > (size-1)){
            printf("ERR 01: coordenada inválida\n");
            getchar();
            goto start;
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
            system("cls");
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
                switch (diff) {
                    case 4:
                        eMap++;
                        if (eMap >= 4){eMap = 0; diff = 6;}
                        break;
                    case 6:
                        iMap++;
                        if (iMap >= 4){iMap = 0; diff = 7;}
                        break;
                    case 7:
                        aMap++;
                        if (aMap >= 4){aMap = 0; zerou = 1;}
                        break;
                }
                supremeGetChar();
                salvarPlacar(size);
                return;
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
                return;
            }
            supremeGetChar();
        }

    goto start;

    printf("");
}

void rank(){    
    system("cls");
    int rSize = readRank();

    for (int i = 0; i < rSize; i++){
        printf("%d %s\n", points[i], nomes[i]);
    }
    supremeGetChar();
};

void initialInput(){
    printf("nome: ");
    char entry[500];
    scanf("%[^\n]", entry);
    for (int i = 0; i < 19; i++){
        playerName[i] = entry[i];   
    }
    playerName[19] = '\0';
    getchar();
};

int main(void)
{
    welcomeMessage();
    initialInput();

    menuStart:
    switch (menu()) {
        //↓↓↓↓↓↓↓↓↓↓↓↓↓
        case '1':
            jogo(diff);
            if (zerou){printf("zeraste. Sai\n"); supremeGetChar(); exit(0);}
            goto menuStart;
            break;
        //↑↑↑↑↑↑↑↑↑↑↑↑↑

        //↓↓↓↓↓↓↓↓↓↓↓↓↓
        case '2':
            confStart:
            switch (config()) {
                case '1':
                    zeraRank();
                    break;

                case '2':
                    sDiffStart:
                    switch (setDiff()) {
                        case '1':
                            diff = 4; printf("[d-4] ok");
                            supremeGetChar(); break;
                        case '2':
                            diff = 6; printf("[d-6] ok");
                            supremeGetChar(); break;
                        case '3':
                            diff = 7; printf("[d-7] ok");
                            supremeGetChar(); break;
                        default:
                            goto sDiffStart;
                    }
                    break;

                case '3':
                    goto menuStart;
            }
            goto confStart;
        //↑↑↑↑↑↑↑↑↑↑↑↑↑

        //↓↓↓↓↓↓↓↓↓↓↓↓↓
        case '3':
            instruc();
            break;
        //↑↑↑↑↑↑↑↑↑↑↑↑↑

        //↓↓↓↓↓↓↓↓↓↓↓↓↓
        case '4':
            rank();
            break;

        //↑↑↑↑↑↑↑↑↑↑↑↑↑

        //↓↓↓↓↓↓↓↓↓↓↓↓↓
        case '5':
            return 0;
        //↑↑↑↑↑↑↑↑↑↑↑↑↑
    }
    goto menuStart;
}
