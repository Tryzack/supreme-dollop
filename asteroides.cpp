#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80


void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);

}

void ocultarcursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;

    SetConsoleCursorInfo(hCon, &cci);
}

void pintarlimites(){
    for(int i=2; i<78; i++){
        gotoxy(i, 3); printf("%c", 205);
        gotoxy(i, 33); printf("%c", 205);
    }

    for(int i=4; i<33; i++){
        gotoxy(2, i); printf("%c", 186);
        gotoxy(77, i); printf("%c", 186);
    }
    gotoxy(2,3); printf("%c", 201);
    gotoxy(2,33); printf("%c", 200);
    gotoxy(77,3); printf("%c", 187);
    gotoxy(77,33); printf("%c", 188);
}

class nave{
    int x, y;
    int vida, vidas;
    public:
    nave(int _x, int _y, int _vida, int _vidas): x(_x), y(_y), vida(_vida), vidas(_vidas){};

    void pintar();
    void borrar();
    void mover();
    void pintar_vida();
    void muerte();
    int preguntar_vida();
    int X() {return x;}
    int Y() {return y;}
    void disminuir_vida();

};

void nave::pintar(){
    gotoxy(x,y); printf("  %c", 30);
    gotoxy(x, y+1); printf(" %c%c%c", 40, 207, 41);
    gotoxy(x, y+2); printf("%c%c %c%c", 30, 219, 219, 30);
}
void nave::borrar(){
    gotoxy(x, y); printf("      ");
    gotoxy(x, y+1); printf("      ");
    gotoxy(x, y+2); printf("      ");
}
void nave::mover(){

        if(kbhit()){
            char tecla = getch();
            gotoxy(x,y);
            borrar();
            if(tecla == IZQUIERDA && x>3) x=x--;
            if(tecla == DERECHA && x+6<77) x=x++;
            if(tecla == ARRIBA &&y>4) y--;
            if(tecla == ABAJO &&y+3<33) y++;
            pintar();
            pintar_vida();
        }

}
void nave::pintar_vida(){

    gotoxy(50,2); printf("VIDAS: %d", vidas);
    gotoxy (64, 2); printf("VIDAS");
    gotoxy (70,2); printf("        ");
    for(int i=0; i<vida; i++){
        gotoxy(70+i,2); printf("%c", 3);
    }
}
void nave::muerte(){
    if(vida==0){
        borrar();
        gotoxy(x,y);   printf("   * ");
        gotoxy(x,y+1); printf("  ***");
        gotoxy(x,y+2); printf("   * ");
        Sleep(1000);

        gotoxy(x,y);   printf("  ***  ");
        gotoxy(x,y+1); printf(" *   * ");
        gotoxy(x,y+2); printf("  ***  ");
        Sleep(1000);
        borrar();
        vidas--;
        vida=3;
        pintar_vida();
        x=38;
        y=25;
        pintar();

        }


}
int nave::preguntar_vida(){
    return vidas;
}
void nave::disminuir_vida(){
vida--;
}

class asteroide{
    int x, y;
public:
    asteroide(int _x, int _y): x(_x), y(_y){}
    void pintar();
    void mover();
    void colision(nave &n);

};

void asteroide::pintar(){
    gotoxy(x,y); printf("x");
}
void asteroide::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y>32){
        x = rand()%71+4;
        y=4;
    }
    pintar();
}
void asteroide::colision(nave &n){

    if(x >= n.X() && x <= n.X()+5 && y>=n.Y() && y<=n.Y()+2){

    n.disminuir_vida();
    n.borrar();
    n.pintar();
    n.pintar_vida();
    x = rand()%71+4;
    y=4;

    }

}

class tiro{
    int x, y;
public:
    tiro(int _x, int _y): x(_x), y(_y){}
    void mover();
    bool fuera() {if(y==4) return true; return false;}
    int X(){return x;}
    int Y(){return y;}
};

void tiro::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("*");
}

int main (){

    pintarlimites();
    nave n(38,25, 3, 3);
    n.pintar();
    n.pintar_vida();

    asteroide a1(10, 4), a2(10, 8), a3(10, 12), a4(10, 16), a5(10, 20), a6(10, 24);

    list<tiro*> b;
    list<tiro*>::iterator it;

    bool fin = false;

    while(!fin){

        if(kbhit()){
            char tecla = getch();
            if (tecla == 'a')
            b.push_back(new tiro(n.X()+2, n.Y()-1));
        }

        for(it = b.begin(); it != b.end(); it++){

            (*it) -> mover();
            if((*it) -> fuera){
                gotoxy((*it)->X(),(*it)->Y()); printf(" ");
                delete(*it);
                it = b.erase(it);
            }
        }


        ocultarcursor();
        a1.mover(); a1.colision(n);
        a2.mover(); a2.colision(n);
        a3.mover(); a3.colision(n);
        a4.mover(); a4.colision(n);
        a5.mover(); a5.colision(n);
        a6.mover(); a6.colision(n);

        n.muerte();
        n.mover();

        Sleep(30);
        if(n.preguntar_vida()==0){
            Sleep(3000);
            n.borrar();
            gotoxy(35, 13); printf("             ");
            gotoxy(35, 14); printf("             ");
            gotoxy(35, 15); printf("             ");
            gotoxy(35, 15); printf("GAME OVER");
            gotoxy(35, 16); printf("             ");
            gotoxy(35, 17); printf("             ");
            Sleep(5000);
            fin=true;
        }
        }

    return 0;
}