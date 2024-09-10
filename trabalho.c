#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {char nome[30]; int pontuation;} rankReg;
char nomeDeUsuario[30];
rankReg ranking[200];
char dificuldade = 'f'; // f/m/d
time_t startTime;

void boasVindas();
void menu();
void jogar();
void config();
void inst();
void rank();
void ext();
void cls();
void zeraRank();
void menuDificuldade();
void setIniciante();
void setIntermediario();
void setAvancado();
void jogoFacil();
void jogoMedio();
void jogoDificil();
int readRank();
void printMap(int *mapa, int *top, int *side, int size);
int getCoord(char *x, char *y);
void innerEasyGame(int rawMap[][4], int top[], int side[], int reff[][4]);
void registerScore(long long score);

int main(void){ 
  boasVindas();
  menu();
}

void boasVindas(){
  printf("Jogo Apc\n");
  printf("Insira o Nome: ");
  scanf("%s", nomeDeUsuario);
  getchar();
}

void menu(){
  system("clear");
  system("clear");

  printf("1 - Jogar\n");
  printf("2 - Configurações\n");
  printf("3 - Instruções\n");
  printf("4 - Ranking\n");
  printf("5 - Sair\n\n");
  printf("Insira o número: ");

  int opt; 
  scanf("%d", &opt);
  cls();

  switch (opt) {
    case 1:
      jogar();
      break;
    case 2:
      config();
      break;
    case 3:
      inst();
      break;
    case 4:
      rank();
      break;
    case 5:
      ext();
      break;
    default:
      menu();
      break;
  }
}

void jogar(){
  switch (dificuldade) {
    case 'f':
      jogoFacil();
      break;
    case 'm':
      jogoMedio();
      break;
    case 'd':
      jogoDificil();
      break;
    default:
      jogoFacil();
      break;
  }
}

void jogoFacil(){
  // LEITURA DO MAPA
  //-------------------------------------------
  FILE *fd = fopen("iniciante.txt", "r");
  int rawMap[4][4];
  for (int i = 0; i < 4; i++){
    for (int ii = 0; ii < 4; ii++){
      rawMap[i][ii] = fgetc(fd)-'0';
    }
    fgetc(fd);
  }

  int top[4];
  for (int i = 0; i < 4; i++){
    top[i] = (fgetc(fd) - '0') * 10 + fgetc(fd) - '0';
  }
  fgetc(fd);

  int side[4];
  for (int i = 0; i < 4; i++){
    side[i] = (fgetc(fd)-'0')*10 + fgetc(fd)-'0';
  }
  fgetc(fd);

  int reff[4][4];
  for (int i = 0; i < 4; i++){
    for (int ii = 0; ii < 4; ii++){
      reff[i][ii] = fgetc(fd)-'0';
    }
    fgetc(fd);
  }


  fclose(fd);
  //-------------------------------------------  

  //CORPO DO JOGO
  time(&startTime);
  innerEasyGame(rawMap, top, side, reff);

}

void innerEasyGame(int rawMap[][4], int top[], int side[], int reff[][4]){
  
  system("clear");

  char x, y;
  
  printMap(&rawMap[0][0], top, side, 4);

  if (!getCoord(&x, &y)){
    innerEasyGame(rawMap, top, side, reff);
    return;
  }

  getchar();

  int concluido = 1;
  for (int i = 0; i < 4; i++){
    for (int ii = 0; ii < 4; ii++){
      if (reff[i][ii] == 0 && rawMap[i][ii] != 0){concluido = 0;}
    }
  }
  if (concluido){
    printf("terminou\n");
    time_t endtime = time(NULL);
    long long tempoDeJogo = (long long) endtime - (long long) startTime;
    long long score = 100000/tempoDeJogo;
    registerScore(score);
    return;
  }

  if (reff[y][x] == 0){
    rawMap[y][x] = 0;

  }
  else {
    printf("fizestes merda\n");
    getchar();
  };

  innerEasyGame(rawMap, top, side, reff);
}

void jogoMedio(){
  
}
void jogoDificil(){
  
}

void registerScore(long long score){
  int scoreLen = readRank();
  char *a = nomeDeUsuario;
  rankReg newReg;
  newReg.pontuation = score;
  for (int i = 0; i < 30; i++){
    newReg.nome[i] = nomeDeUsuario[i];
  }
  // printf("%d %s\n", newReg.pontuation, newReg.nome);
  int move = 0;
  rankReg tmp, tmpb;

  for (int i = 0; i < scoreLen; i++){
    if (!move && ranking[i].pontuation < score){
      tmp = ranking[i];
      ranking[i] = newReg;
      move = 1;
    }
    else if (move) {
      tmpb = tmp;
      tmp = ranking[i];
      ranking[i] = tmpb;
    }
  }

  for (int i = 0; i < scoreLen+1; i++){
    printf("%d %s\n", ranking[i].pontuation, ranking[i].nome);
  }


}

int getCoord(char *x, char *y){
  char str[100];
  printf("x: ");
  scanf("%[^\n]", str);
  getchar();
  *x = str[0] - '0';
  if (str[1] != '\0'){return 0;}

  printf("y: ");
  scanf("%[^\n]", str);
  getchar();
  *y = str[0] - '0';

  if (str[1] != '\0'){return 0;}
  if (*x > 9 || x < 0){return 0;}
  if (*y > 9 || x < 0){return 0;}
  return 1;
}

void printMap(int *mapa, int *top, int *side, int size){
  int t1, t2, t3;
  printf("  |");
  t1 = 0;
  t2 = size;
  LP3:
    if (t1 >= t2) goto LE3;
      (10 > *(top+t1))? printf("0%d|", *(top+t1)):printf("%d|", *(top+t1));
    t1++;
    goto LP3;
  LE3:
  printf("\n  ▂");
  for (int i = 0; i < size*3; i++) printf("▂");
  printf("\n");
  t1 = 0;
  t3 = 0;
  LP:
    if (t1 >= size*size) goto LE;
    (10 > *(side+t3))? printf("%d ▋", *(side+t3)):printf("%d▋", *(side+t3));
    t3++;
    //----------------------------
    t2 = t1 + size;
    LP2:
      if (t1 >= t2) goto LE2;
      //--------------------------
      printf("%d |", *(mapa+t1));
      //--------------------------
      t1++;
      goto LP2;
    LE2:
    printf("\n");
    //----------------------------
    goto LP;
  LE:
  return;
}

void config(){
  system("clear");
  printf("configuração:\n\n");
  printf("1 - zerar ranking\n");
  printf("2 - modo de dificuldade\n");
  printf("3 - voltar ao menu principal\n\n");
  printf("Insira o número: ");
  int opt;
  scanf("%d", &opt);
  cls();

  switch (opt) {
    case 1:
      zeraRank();
      break;
    case 2:
      menuDificuldade();
      break;
    case 3:
      menu();
      break;
    default:
      config();
      break;
    }
  
}

void menuDificuldade(){
  system("clear");
  printf("dificuldade\n\n");
  printf("1 - iniciante\n");
  printf("2 - intermediário\n");
  printf("3 - avançado\n");
  printf("4 - retornar\n\n");
  printf("Insira o número: ");

  int opt;
  scanf("%d", &opt);
  cls();
  
  switch (opt) {
    case 1:
      setIniciante();
      break;
    case 2:
      setIntermediario();
      break;
    case 3:
      setAvancado();
      break;
    case 4:
      config();
      break;
    default:
      menuDificuldade();
      break;
    }
}

void inst(){printf("instruções\n");}

void rank(){
  system("clear");
  printf("ranking\n\n");
  FILE *rnk = fopen("rank.bin", "r");
  int index = 0;
  while (fread(&ranking[index], sizeof(rankReg), 1, rnk)){
    printf("%d %s\n",  ranking[index].pontuation, ranking[index].nome); 
    index++;
  }
  fclose(rnk);
  getchar();
  menu();
}
void ext(){printf("ext\n");}
void cls(){while (getchar() != '\n');}
void zeraRank(){
  FILE *rnk = fopen("rank.bin", "w");
  fclose(rnk);
  printf("rank zerado [0]\n");
  getchar();
  config();
}
void setIniciante(){
  dificuldade = 'f';
  printf("modo iniciante ok [%c]\n", dificuldade);
  getchar();
  menuDificuldade();
}
void setIntermediario(){  
  dificuldade = 'm';
  printf("modo intermediário ok [%c]\n", dificuldade);
  getchar();
  menuDificuldade();
}
void setAvancado(){
  dificuldade = 'd';
  printf("modo intermediário ok [%c]\n", dificuldade);
  getchar();
  menuDificuldade();
}

int readRank(){
  FILE *rnk = fopen("rank.bin", "r");
  int index = 0;
  while (fread(&ranking[index], sizeof(rankReg), 1, rnk)){index++;}
  fclose(rnk);
  return index;
}
