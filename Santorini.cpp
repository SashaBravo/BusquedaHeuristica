#include <iostream>
using namespace std;


//tablero de 5x5, 32 bits
uint32_t empty=0x0;
uint32_t full=0x1f1f1f;

uint32_t firstRow=0xf8000000;
uint32_t lastRow=0x00000f80;
uint32_t borde=0xfc631f80;
uint32_t firstCol=0x84210842;
uint32_t lastCol=0x08421084;



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

/*
//Colores
std::string red = "\033[1;31m";     //Piso0
std::string green = "\033[1;32m";   //Piso1
std::string cyan = "\033[1;36m";    //Piso2

std::string cyan_b = "\033[46m";    //Piso3
std::string white_b = "\033[47m";   //Piso4

std::string clear_color = "\033[0m";

*/


void print(uint32_t board, GameState game, bool vecinos){
    uint32_t one=0x80000000;
    for(int i=0;i<25;i++){
        if(one&board){

            if(!vecinos)
            {
                if(one&game.Player1)
                {
                    std::cout<<"o";
                }

                if(one&game.Player2)
                {
                    std::cout<<"x";
                }
            }
            else
            {
                if(one&~game.FullBoard)
                {
                    std::cout<<"v";
                }
                else{
                    std::cout<<"f";
                }
                
            }
            
        }
        else{
            std::cout<<"-";
        }
        if(i%5==4){
            std::cout<<std::endl;
        }
        one >>= 1;
    }
    std::cout<<std::endl;
}

uint32_t vecinos(uint32_t board, GameState game){
    uint32_t vecinoDerecha=(board&~lastCol)>>1;
    uint32_t vecinoIzquierda=(board&~firstCol)<<1;
    
    uint32_t vecinoArriba=(board&~firstRow)<<5;
    uint32_t vecinoArribaD=(board&~firstRow&~lastCol)<<4;
    uint32_t vecinoArribaI=(board&~firstRow&~firstCol)<<6;
    
    uint32_t vecinoAbajo=(board&~lastRow)>>5;
    uint32_t vecinoAbajoD=(board&~lastRow&~lastCol)>>6;
    uint32_t vecinoAbajoI=(board&~lastRow&~firstCol)>>4;
    return (vecinoDerecha|vecinoIzquierda|vecinoArriba|vecinoArribaD|vecinoArribaI|vecinoAbajo|vecinoAbajoD|vecinoAbajoI)&~game.FullBoard;

}



int main(){

    GameState game;

    bool Turno1 = true;
    bool Colocacion = true;
    bool Jugando = true;

    //Colocacion de Jugadores
    
    bool pawn1 = true;
    while (Colocacion)
    {
        if(Turno1){

            

            while (true)
            {
                std::cout<<"Ingrese coordenadas Jugador 1: ";
                int i,j;
                std::cin>>i>>j;
            
                if (game.FullBoard != (game.FullBoard |= 0x80000000>>(j*5+i)))
                {
                    game.Player1 |= 0x80000000>>(j*5+i);

                    game.FullBoard |= game.Player1;

                    break;
                }

                else{
                    std::cout<<"ya hay una ficha ahi";
                    std::cout<<std::endl;
                }
                
            }

            Turno1 = false;
        }
        
        else{
            

            while (true)
            {
                std::cout<<"Ingrese coordenadas Jugador 2: ";
                int i,j;
                std::cin>>i>>j;
                if (game.FullBoard != (game.FullBoard |= 0x80000000>>(j*5+i)))
                {
                    game.Player2 |= 0x80000000>>(j*5+i);

                    game.FullBoard |= game.Player2;

                    break;
                }

                else{
                    std::cout<<"ya hay una ficha ahi";
                    std::cout<<std::endl;
                }
            }
            

            


            Turno1 = true;

            if (pawn1){
                pawn1 = false;
            }

            else{
                Colocacion = false;
            }
        }

        print(game.FullBoard, game, false);
    }



    //Juego

    while (Jugando)
    {
        //Jugador 1
        if(Turno1){

            auto veci = 0x0;

            //Escoger Ficha
            while (true)
            {
                std::cout<<"Escoga una de las fichas del Jugador 1: ";
                int i,j;
                std::cin>>i>>j;

                uint32_t bitMap1 = 0x80000000>>(j*5+i);

                //Ver vecinos

                if (game.Player1&bitMap1)
                {
                    std::cout<<"Estos son tus posibles movimientos: ";
                    std::cout<<std::endl;

                    veci = vecinos(bitMap1, game);

                    print(veci, game, true);

                    game.Player1^=bitMap1;
                    game.FullBoard^=bitMap1;

                    break;

                }

                else{
                    std::cout<<"No existe una ficha del Jugador 1 ahi.";
                    std::cout<<std::endl;
                }
            }

            //Mover Ficha 
            while (true)
            {
                std::cout<<"Seleccione un movimiento: ";
                int i,j;
                std::cin>>i>>j;

                uint32_t moveP1 = 0x80000000>>(j*5+i);

                //Ver vecinos

                if (moveP1&veci)
                {
                    std::cout<<"Estos son tus posibles movimientos: ";
                    std::cout<<std::endl;

                    game.Player1|= moveP1;
                    game.FullBoard|= game.Player1;

                    print(game.FullBoard, game, false); 

                    break;

                }

                else{
                    std::cout<<"Moviemiento no valido";
                    std::cout<<std::endl;
                }
            }

            


            //Poner Piso



        }


        //Jugador 2
        else{
            //Mover Ficha 



            //Poner Piso



        }
    }
    
    




    //print(firstRow);
    //print(lastRow);
/*
//Imprimendo Columnas
    // uint32_t col1=0x84210842;
    // print(col1);

    // uint32_t col2=0x42108421;
    // print(col2);

    // uint32_t col3=0x21084210;
*/
}