#include <iostream>
using namespace std;


//tablero de 5x5, 32 bits
uint32_t empty=0x0;
uint32_t full=0x1f1f1f;

uint32_t firstRow=0xf0000000;
uint32_t lastRow=0x000f0000;
uint32_t borde=0xf99f0000;
uint32_t firstCol=0x88880000;
uint32_t lastCol=0x1111000;



struct GameState
{
    uint32_t FullBoard=0x0;

    uint32_t Player1=0x1;
    uint32_t Player2=0x0;
    uint32_t Floor1=0x0;
    uint32_t Floor2=0x0;
    uint32_t Floor3=0x0;
    uint32_t Floor4=0x0;
};




void print(uint32_t board){
    uint32_t one=0x80000000;
    for(int i=0;i<25;i++){
        if(one&board){
            std::cout<<"o";
            
        }else{
            std::cout<<"-";
        }
        if(i%5==4){
            std::cout<<std::endl;
        }
        one >>= 1;
    }
    std::cout<<std::endl;
}

uint32_t vecinos(uint32_t board){
    uint32_t vecinoDerecha=(board&~lastCol)>>1;
    uint32_t vecinoIzquierda=(board&~firstCol)<<1;
    uint32_t vecinoArriba=(board&~firstRow)<<5;
    uint32_t vecinoAbajo=(board&~lastRow)>>5;
    return vecinoDerecha|vecinoIzquierda|vecinoArriba|vecinoAbajo;

}
int main(){

    GameState game;

    bool Turno1 = true;
    bool Colocacion = true;

    //Colocacion de Jugadores
    
    bool pawn1 = true;
    while (Colocacion)
    {
        if(Turno1){

            std::cout<<"Ingrese coordenadas Jugador 1: ";
            int i,j;
            std::cin>>i>>j;
            game.Player1 |= 0x80000000>>(j*5+i);
            print(game.Player1);

            Turno1 = false;
        }
        
        else{
            
            std::cout<<"Ingrese coordenadas Jugador 2: ";
            int i,j;
            std::cin>>i>>j;
            game.Player2 |= 0x80000000>>(j*5+i);
            print(game.Player2);


            Turno1 = true;

            if (pawn1){
                pawn1 = false;
            }

            else{
                Colocacion = false;
            }
        }

    }
    

    //print(firstRow);
    //print(lastRow);

//Imprimendo Columnas
    // uint32_t col1=0x84210842;
    // print(col1);

    // uint32_t col2=0x42108421;
    // print(col2);

    // uint32_t col3=0x21084210;
    // print(col3);

    // uint32_t col4=0x10842108;
    // print(col4);

    // uint32_t col5=0x08421084;
    // print(col5);


//Imprimendo Filas
    // uint32_t row1=0xf8000000;
    // print(row1);

    // uint32_t row2=0x07c00000;
    // print(row2);

    // uint32_t row3=0x003e0000;
    // print(row3);

    // uint32_t row4=0x0001f000;
    // print(row4);

    // uint32_t row5=0x00000f80;
    // print(row5);

    //auto veci=vecinos(tab);
    // print(veci);
    // print(vecinos(0x0400));

    //print(full&~0x0400);

    //auto veci=vecinos(0x0400);
    
    // std::cout<<"inicial:"<<std::endl;
    // print(veci);

    // while(veci){
    //     auto i=16-__builtin_ffs(veci);
    //     uint16_t v = 0x8000>>i;
    //     std::cout<<"vecino:"<<std::endl;
    //     print(v);
    //     veci^=v;

    // }


    //print(veci);
    //std::cout<<"Tiene "<<__builtin_popcount(veci)<<" piezas"<<std::endl;



    return 0;

}