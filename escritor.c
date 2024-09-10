#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {int pontuation; char nome[20];} rankReg;

int main(void){
  rankReg a = {20, "rafael"};
  rankReg b = {19, "joao"};
  rankReg c = {18, "batata"};
  rankReg d = {17, "dinheiro"};
  rankReg e = {16, "batata de novo"};
  rankReg f = {15, "ola"};

  rankReg lista[] = {a, b, c, d, e, f};

  FILE *fp = fopen("ranking.bin", "wb");

  int res = fwrite(lista, sizeof(lista), 1, fp);
  fclose(fp);
  printf("%d\n", res);
}
