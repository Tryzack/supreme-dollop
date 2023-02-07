//NOTA CAMBIAR TAMAÑO DEL CMD PARA PODER VERLO COMPLETO
//PARA ELLO, ABRIR EL JUEGO HACER CLICK DERECHO EN LA VENTANA DE CMD -> PROPIEDADES
//EN WINDOWS SIZE O TAMAÑO DE VENTANA UTILIZAR: ANCHO/WEITH = 80 Y ALTO/HEIGHT = 40

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

//FUNCION GOTOXY GENERAL
void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);

}

//FUNCION OCULTAR CURSOR DE LA CONSOLA
void ocultarcursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;

    SetConsoleCursorInfo(hCon, &cci);
}

//FUNCION PARA DIBUJAR LIMITES DEL MAPA
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

//CLASE DE LA NAVE
class nave{
    int x, y;
    int vida, vidas;
    public:
    //CONSTRUCTOR
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

//FUNCION PARA DIBUJAR LA NAVE
void nave::pintar(){

    gotoxy(x,y); printf("  %c", 30);
    gotoxy(x, y+1); printf(" %c%c%c", 40, 207, 41);
    gotoxy(x, y+2); printf("%c%c %c%c", 30, 219, 219, 30);

}
//FUNCION PARA BORRAR LA NAVE
void nave::borrar(){
    gotoxy(x, y); printf("      ");
    gotoxy(x, y+1); printf("      ");
    gotoxy(x, y+2); printf("      ");
}
//FUNCION PARA MOVER LA NAVE
void nave::mover(){

        if(kbhit()){
            char tecla = getch();
            gotoxy(x,y);
            borrar();
            if(tecla == IZQUIERDA && x>3) x--;
            if(tecla == DERECHA && x+6<77) x++;
            if(tecla == ARRIBA &&y>4) y--;
            if(tecla == ABAJO &&y+3<33) y++;
            pintar();
            pintar_vida();
        }

}
//FUNCION PARA DIBUJAR LAS VIDAS RESTANTES
void nave::pintar_vida(){

    gotoxy(50,2); printf("VIDAS: %d", vidas);
    gotoxy (64, 2); printf("VIDAS");
    gotoxy (70,2); printf("        ");
    for(int i=0; i<vida; i++){
        gotoxy(70+i,2); printf("%c", 3);
    }
}
//FUNCION DE MUERTE DE LA NAVE
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
//FUNCION PARA DEVOLVER VIDAS RESTANTES
int nave::preguntar_vida(){
    return vidas;
}
//FUNCION PARA DISMINUIR VIDA
void nave::disminuir_vida(){
    vida--;
}


//CLASE DE TIROS O BALAS
class tiro{
    int x, y;
public:
    //CONSTRUCTOR DE LOS TIROS
    tiro(int _x, int _y): x(_x), y(_y){}
    void mover();
    bool fuera();
    int X(){return x;}
    int Y(){return y;}
    int colisionstatus();
    void cambiarcs();

};

bool tiro::fuera(){
    if(y==4){
        return true;
    }
    return false;
}
//FUNCION PARA MOVER LOS TIROS
void tiro::mover(){

            gotoxy(x,y); printf(" ");
            y--;
            gotoxy(x,y); printf("*");

}

//CLASE DE LOS ASTEROIDES
class asteroide{
    int x, y;
public:
    //CONSTRUCTOR DEL ASTEROIDE
    asteroide(int _x, int _y): x(_x), y(_y){}
    void pintar();
    void mover();
    void colision(nave &n);
    void colision2(tiro &t);
    int X(){return x;}
    int Y(){return y;}

};
//FUNCION PARA DIBUJAR EL ASTEROIDE
void asteroide::pintar(){
    gotoxy(x,y); printf("x");
}
//FUNCION PARA MOVER AL ASTEROIDE
void asteroide::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y>32){

        x = rand()%71+4;
        y=4;

    }
    pintar();
}
//FUNCION PARA LA COLISION ENTRE ASTEROIDE Y NAVE
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

void derrota(){
    
}

//CAMBIAR DIFICULTAD
int cambiardificultad(int dificultad){
    if(dificultad==3) {
        dificultad=1;
    } else if(dificultad==1 || dificultad==2) dificultad++;
    return dificultad;
}

//MAIN
int main (){
    
    ocultarcursor();
    //DIBUJAR LIMITES DEL MAPA
    pintarlimites();
    nave n(38,25, 3, 3);
    bool fin = false;
    int puntuacion=0, sel=15, nast=5, speed=30;
    bool menu=true;
    int dificultad=2; //la dificultad afecta la cantidad de asteroides y la velocidad del juego
    char dificultad1[8] = "NORMAL ";
    while(menu==true){
        gotoxy(35,15); printf("INICIO");
        gotoxy(35,16); printf("DIFICULTAD: %s        ", dificultad1);
        gotoxy(35,17); printf("SALIR");
        gotoxy(30,sel); printf(">>>");
        char tecla = getch();
            if (tecla == ARRIBA && sel>15) {
                gotoxy(30,sel); printf("   ");
                sel=sel-1;
            }
            if (tecla == ABAJO && sel<17) {
                gotoxy(30,sel); printf("   ");
                sel=sel+1;
            }
            if (tecla=='a' && sel==15) menu=false;
            if (tecla=='a' && sel==16) {
                dificultad = cambiardificultad(dificultad);
                switch(dificultad)  {
                    case 1:
                        dificultad1[0] = 'F';
                        dificultad1[1] = 'A';
                        dificultad1[2] = 'C';
                        dificultad1[3] = 'I';
                        dificultad1[4] = 'L';
                        dificultad1[5] = ' ';
                        dificultad1[6] = ' ';
                        dificultad1[7] = ' ';
                        dificultad1[8] = ' ';
                        nast=4;
                        speed=40;
                        break;
                    case 2:
                        dificultad1[0] = 'N';
                        dificultad1[1] = 'O';
                        dificultad1[2] = 'R';
                        dificultad1[3] = 'M';
                        dificultad1[4] = 'A';
                        dificultad1[5] = 'L';
                        dificultad1[6] = ' ';
                        dificultad1[7] = ' ';
                        dificultad1[8] = ' ';
                        nast=5;
                        speed=30;
                        break;
                    case 3:
                        dificultad1[0] = 'D';
                        dificultad1[1] = 'I';
                        dificultad1[2] = 'F';
                        dificultad1[3] = 'I';
                        dificultad1[4] = 'C';
                        dificultad1[5] = 'I';
                        dificultad1[6] = 'L';
                        dificultad1[7] = ' ';
                        dificultad1[8] = ' ';
                        nast=7;
                        speed=15;
                        break;
                    default:
                        dificultad1[0] = 'N';
                        dificultad1[1] = 'O';
                        dificultad1[2] = 'R';
                        dificultad1[3] = 'M';
                        dificultad1[4] = 'A';
                        dificultad1[5] = 'L';
                        dificultad1[6] = ' ';
                        dificultad1[7] = ' ';
                        dificultad1[8] = ' ';
                        nast=5;
                        speed=30;
                        break;

                }
            }
            if (tecla=='a' && sel==17) return 0;
    }

    gotoxy(35,15); printf("                       ");
    gotoxy(35,16); printf("                       ");
    gotoxy(35,17); printf("                       ");
    gotoxy(30,sel); printf("                       ");

    //DIBUJAR NAVE
    n.pintar();
    n.pintar_vida();

    //LISTA ASTEROIDES
    list<asteroide*> ast;
    list<asteroide*>::iterator ita;
    for(int i=0; i<nast; i++){
        ast.push_back(new asteroide(rand() %74+3 , rand() %5+4 ));
    }

    //LISTA TIROS
    list<tiro*> b;
    list<tiro*>::iterator it;
    
    //INICIO JUEGO
    while(!fin){

        //CREACION DE LOS TIROS
        if(kbhit()){

            char tecla = getch();
            if (tecla == 'a')
            b.push_back(new tiro(n.X()+2, n.Y()-1));

        }

        for(it = b.begin(); it != b.end(); it++){

            (*it) -> mover();
            if((*it)->fuera()){
                gotoxy((*it)->X(),(*it)->Y()); printf(" ");
                delete(*it);
                it = b.erase(it);

            }
       }

        ocultarcursor();
        //CREACION DE LOS ATEROIDES
        for(ita = ast.begin(); ita != ast.end(); ita++){
            (*ita)->mover();
            (*ita)->colision(n);
        }

        //COLISION DE ASTEROIDES Y TIROS

        for(ita = ast.begin(); ita != ast.end(); ita++){
            for(it=b.begin(); it !=b.end(); it++){
                if((*ita)->X()==(*it)->X() && ((*ita)->Y() +1 == (*it)-> Y() || (*ita)->Y() == (*it)-> Y() )){

                    gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                    delete(*it);
                    it = b.erase(it);

                    ast.push_back(new asteroide(rand()%74+3 , 4));
                    gotoxy((*ita)->X(), (*ita)->Y()); printf(" ");
                    delete(*ita);
                    ita = ast.erase(ita);

                    puntuacion+=100;

                }
            }
        }

        //CREACION DE LA NAVE
        n.muerte();
        n.mover();

        //VELOCIDAD DEL JUEGO EN MILISEGUNDOS
        Sleep(speed);

        if(n.preguntar_vida()==0){
            Sleep(1500);
            n.borrar();
            gotoxy(35, 13); printf("             ");
            gotoxy(35, 14); printf("             ");
            gotoxy(35, 15); printf("             ");
            gotoxy(35, 15); printf("GAME OVER");
            gotoxy(35, 16); printf("             ");
            gotoxy(35, 17); printf("             ");
            Sleep(2500);
            gotoxy(35, 17); printf("PUNTUACION: %d", puntuacion);
            Sleep(2500);
            fin=true;
        }

        gotoxy(4,2); printf("PUNTUACION: %d", puntuacion);
    }
    

    return 0;
}
