#include <stdio.h>
#include <ncurses.h>

int wy, wx;
char nome[50];

void input(WINDOW *win, int y, int x, char str[], int maxLen);

void inputBox(WINDOW *win, int y, int x, char str[], int maxLen);

void Menu();

void BoasVindas();

int max(int a, int b);

int main(void){
  initscr();
  getmaxyx(stdscr, wy, wx);
  //---------------------------

  BoasVindas(nome);
  Menu();
  endwin();
  return 0;
}


//-------------------------------------------------------------------

void BoasVindas(){
  noecho();
  werase(stdscr);
  getmaxyx(stdscr, wy, wx);
  box(stdscr, 0, 0);
  int ch;
  WINDOW *inpwin = newwin(3, 50, 2, 2);

  mvprintw(0, max((wx/2)-16, 0), "BEM VINDO AO 'TRABALHO DE APC'!!!");
  mvprintw(1, 3, "DIGITE SEU NOME:");
  refresh();
  inputBox(inpwin, 1, 1, nome, 50);
}

void input(WINDOW *win, int y, int x, char str[], int maxLen){
  noecho();

  int ch;
  int index = 0;
  while (index < maxLen-1){
    ch = wgetch(win);

    if (ch == '\n') {str[index] = '\0'; break;}
    else if (ch == 127) {
      if (ch != 0) {str[index-1] = '\0'; index -= 2;}
      else {index = -1; str[0] = '\0';}
    }
    else {str[index] = ch; str[index+1] = '\0';}

    werase(win);
    mvwprintw(win, y, x, "%s", str);


    refresh();
    index++;
  }
  echo();
}

void inputBox(WINDOW *win, int y, int x, char str[], int maxLen){
  noecho();
  box(win, 0, 0);

  int ch;
  int index = 0;
  while (index < maxLen-2){
    ch = wgetch(win);



    if (ch == '\n') {str[index] = '\0'; break;}
    else if (ch == 127) {
      if (ch != 0) {str[index-1] = '\0'; index -= 2;}
      else {index = -1; str[0] = '\0';}
    }
    else {str[index] = ch; str[index+1] = '\0';}

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, y, x, "%s", str);
    refresh();
    index++;
  }
  echo();
}

void Menu(){
  
}

int max(int a, int b){
  if (a > b) return a;
  return b;
}
