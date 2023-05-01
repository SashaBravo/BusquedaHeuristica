#include <iostream>
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

//tablero de 5x5, 32 bits
uint32_t empty=0x0;
uint32_t full=0x1f1f1f;

uint32_t firstRow=0xf8000000;
uint32_t lastRow=0x00000f80;
uint32_t borde=0xfc631f80;
uint32_t firstCol=0x84210842;
uint32_t lastCol=0x08421084;


bool Jugando = true;

struct GameState
{
    uint32_t FullBoard=0x0;

    uint32_t Player1=0x1;
    uint32_t Player2=0x0;
    uint32_t Floor0=0xffffffff;
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
        cout << BG_BLACK;
        if(one&game.Floor1){
        cout<<BG_MAGENTA;
        }
        if(one&game.Floor2){
        cout<<BG_YELLOW;
        }
        if(one&game.Floor3){
        cout<<BG_CYAN;
        }
        if(one&game.Floor4){
        cout<<BG_WHITE;
        }
        cout << "[";
        if(one&board){

            if(!vecinos)
            {
                if(one&game.Player1)
                {
                    std::cout<<BLUE<<"o"<< RESET << BG_BLACK;
                }

                 if(one&game.Player2)
                {
                    std::cout<<BLUE<<"x"<< RESET << BG_BLACK;
                }

                //else { std::cout<<BLUE<<" " << RESET << BG_BLACK;}
            }
            else
            {
                if(one&~game.FullBoard)
                {
                    std::cout<<GREEN<<"v"<< RESET << BG_BLACK;
                }
                else{
                    std::cout<< RED<<"f"<< RESET << BG_BLACK;
                }
                
            }

            
        }
        else{
            std::cout<<" "<< RESET << BG_BLACK;
        }

        if(one&game.Floor1){
        cout<<BG_MAGENTA;
        }
        if(one&game.Floor2){
        cout<<BG_YELLOW;
        }
        if(one&game.Floor3){
        cout<<BG_CYAN;
        }
        if(one&game.Floor4){
        cout<<BG_WHITE;
        }

        cout << "]";
        if(i%5==4){
            std::cout<<RESET<<std::endl;
        }
        one >>= 1;
        
    }
    std::cout<<std::endl;
}

uint32_t vecinos(uint32_t board, GameState game){
    uint32_t vecinoDerecha=(board&~lastCol&~game.Floor4)>>1;
    uint32_t vecinoIzquierda=(board&~firstCol&~game.Floor4)<<1;
    
    uint32_t vecinoArriba=(board&~firstRow&~game.Floor4)<<5;
    uint32_t vecinoArribaD=(board&~firstRow&~lastCol&~game.Floor4)<<4;
    uint32_t vecinoArribaI=(board&~firstRow&~firstCol&~game.Floor4)<<6;
    
    uint32_t vecinoAbajo=(board&~lastRow&~game.Floor4)>>5;
    uint32_t vecinoAbajoD=(board&~lastRow&~lastCol&~game.Floor4)>>6;
    uint32_t vecinoAbajoI=(board&~lastRow&~firstCol&~game.Floor4)>>4;
    return (vecinoDerecha|vecinoIzquierda|vecinoArriba|vecinoArribaD|vecinoArribaI|vecinoAbajo|vecinoAbajoD|vecinoAbajoI)&~game.FullBoard;

}

uint32_t vecinos2(uint32_t Floor, GameState game){
    
    if (Floor == game.Floor3)
    {
        cout<<"Ganaste";
        return game.Floor3|game.Floor2|game.Floor1|game.Floor0;
    }
    else if (Floor == game.Floor2)
    {
        return game.Floor3|game.Floor2|game.Floor1|game.Floor0;
    }
    else if (Floor == game.Floor1)
    {
        return (game.Floor2|game.Floor1|game.Floor0)&~game.Floor3;
    }
    else
    {
        return (game.Floor1|game.Floor0)&~game.Floor2&~game.Floor3;
    }


}

uint32_t ActualFloor(uint32_t player, GameState game){

    if(player&game.Floor3){
        cout<<"Ganaste";
        return game.Floor3;
    }
    if(player&game.Floor2){
        cout<<"Estas en el piso 2"<<endl;
        return game.Floor2;
    }
    if(player&game.Floor1){
        cout<<"Estas en el piso 1" <<endl;
        return game.Floor1;
    }

    cout<<"Estas en el piso 0" <<endl;

    return game.Floor0;
}

void Building(uint32_t& Player_board, GameState& game_){

    uint32_t veci = 0x0;

    veci = vecinos(Player_board, game_);
    print(veci, game_, true);
    

    std::cout<<"Seleccione donde construir: ";
    int i,j;
    std::cin>>i>>j;
    uint32_t buildP = 0x80000000>>(j*5+i);
    if(buildP&veci)
    {
        if(buildP&game_.Floor1)
        {
            if(buildP&game_.Floor2)
            {
                if(buildP&game_.Floor3)
                {
                    game_.Floor4|= buildP;
                }
                game_.Floor3|= buildP;
            }
            game_.Floor2|= buildP;
        }
        game_.Floor1|= buildP;
    }

    
    print(game_.FullBoard, game_, false);
}

void Move(uint32_t& Player_board, GameState& game_, uint32_t& veci)
{

    while(true){
            
        std::cout<<"Seleccione un movimiento: ";
        int i,j;
        std::cin>>i>>j;

        uint32_t moveP = 0x80000000>>(j*5+i);

        //Ver vecinos
        //print(game_.Player1, game_, false);
        //print(game_.Player2, game_, false);

        if (moveP&veci)
        {
            std::cout<<"Estos son tus posibles movimientos: ";
            std::cout<<std::endl;
            
            Player_board|= moveP;
            game_.FullBoard|= Player_board;

            print(game_.FullBoard, game_, false); 

            if(moveP&game_.Floor3){
                Jugando = false;
                break;
            }

            cout << "Build" << endl;
            Building(moveP,game_);
            break;

        }

        else{
            std::cout<<"Moviemiento no valido";
            std::cout<<std::endl;
        }
                
    }
}

void SelectToken(uint32_t& Player_board, GameState& game_)
{
    uint32_t veci = 0x0;
    uint32_t veci2 = 0x0;
    uint32_t floor = 0x0;
    while (true)
    {
        std::cout<<"Escoga una de las fichas del Jugador: ";
        int i,j;
        std::cin>>i>>j;

        uint32_t bitMap = 0x80000000>>(j*5+i);

        //Ver vecinos

        if (Player_board&bitMap)
        {
            std::cout<<"Estos son tus posibles movimientos: ";
            std::cout<<std::endl;

            floor = ActualFloor(bitMap, game_);
            veci = vecinos(bitMap, game_);      //Vecinos donde no hay jugadores, borde o piso 4

            veci2 = vecinos2(floor, game_);      //Vecinos de piso+1 e inferiores

            uint32_t aux_veci = veci&veci2;

            print(veci&veci2, game_, true);

            Player_board^=bitMap;
            game_.FullBoard^=bitMap;
            Move(Player_board, game_, aux_veci);
            break;
        }

        else{
            std::cout<<"No existe una ficha del Jugador ahi.";
            std::cout<<std::endl;
        }
    }
}

int main(){

    GameState game;
    //GameState* game_ = &game;

    bool Turno1 = true;
    bool Colocacion = true;

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
            std::cout<<"Jugador 1.";
        SelectToken(game.Player1, game);
        Turno1 = false;
        }else{
            std::cout<<"Jugador 2.";
        SelectToken(game.Player2, game);
        Turno1 = true;
        }
    }

    cout<<"Ganador: Jugador ";
    if (!Turno1)
    {
        cout<<"1" <<endl;
    }
    else
    {
        cout<<"2" <<endl;
    }

}