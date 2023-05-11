#include <iostream>
#include <stddef.h>
#include <limits>
#include <cstdlib>
#include <vector>

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

    uint32_t Player1=0x0;
    uint32_t Player2=0x0;
    uint32_t Floor0=0xffffffff;
    uint32_t Floor1=0x0;
    uint32_t Floor2=0x0;
    uint32_t Floor3=0x0;
    uint32_t Floor4=0x0;
};


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
    
    if (Floor == game.Floor3){
        cout<<"Ganaste";
        return game.Floor3|game.Floor2|game.Floor1|game.Floor0;
    }
    else if (Floor == game.Floor2){
        return game.Floor3|game.Floor2|game.Floor1|game.Floor0;
    }
    else if (Floor == game.Floor1){
        return (game.Floor2|game.Floor1|game.Floor0)&~game.Floor3;
    }
    else{
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

    //cout<<"Estas en el piso 0" <<endl;

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

void Move(uint32_t& Player_board, GameState& game_, uint32_t next_M, uint32_t token)
{       
    cout<<"Seleccione un movimiento: " << endl;
/*
    Player_board^=token;
    game_.FullBoard^=token;*/

    uint32_t moveP = next_M;
    
    Player_board|= moveP;
    game_.FullBoard|= Player_board;


    print(game_.FullBoard, game_, false); 

    if(moveP&game_.Floor3){
        Jugando = false;
    }

    // sacarlo
    //cout << "Build" << endl;
    // Building(moveP,game_);


}

GameState Apply_Move(uint32_t Player_board, GameState game_, uint32_t next_M)
{           
    //cout<<"Seleccione un movimiento: " << endl;

    uint32_t moveP = next_M;
    
    Player_board|= moveP;
    game_.FullBoard|= Player_board;

    //print(game_.FullBoard, game_, false); 

    if(moveP&game_.Floor3){
        Jugando = false;
    }

    return game_;
}

uint32_t SelectToken(uint32_t& Player_board, GameState& game_)
{
    uint32_t veci = 0x0;
    uint32_t veci2 = 0x0;
    uint32_t floor = 0x0;

        std::cout<<"Escoga una de las fichas del Jugador: " << endl;

        // dejarlo bonito

        uint32_t TurnPlayer = Player_board;
        auto token = 32-__builtin_ffs(TurnPlayer);
        uint32_t t1 = 0x80000000>>token;
        TurnPlayer ^= t1;
        auto token2 = 32-__builtin_ffs(TurnPlayer);
        uint32_t t2 =   0x80000000>>token2;

        int a = rand() % 2;
        uint32_t bitMap;
        if(a==0){ bitMap = t1;}
        else{ bitMap = t2;}
        
        //uint32_t bitMap = 0x80000000>>token;
        //uint32_t 
        print(bitMap, game_, false); 

    
        std::cout<<std::endl;
        //Ver vecinos


            std::cout<<"Estos son tus posibles movimientos: ";
            std::cout<<std::endl;

            floor = ActualFloor(bitMap, game_);
            veci = vecinos(bitMap, game_);      //Vecinos donde no hay jugadores, borde o piso 4

            veci2 = vecinos2(floor, game_);      //Vecinos de piso+1 e inferiores

            uint32_t aux_veci = veci&veci2;

            print(veci&veci2, game_, true);

            Player_board^=bitMap;
            game_.FullBoard^=bitMap;
            // Move(Player_board, game_, aux_veci);
            // break;

    return aux_veci;
}

vector<uint32_t> generate_moves(uint32_t& possible_mov) {
    vector<uint32_t> moves;
    
    while(possible_mov){
        
        auto move = 32-__builtin_ffs(possible_mov);
        uint32_t aux_move = 0x80000000>>move;
        possible_mov ^= aux_move;
        moves.push_back(aux_move);

    }
    return moves;
}

int win_positions(GameState game, uint32_t player) {
    int count = 0;
    uint32_t playerMask = player == game.Player1 ? game.Player1 : game.Player2;
    uint32_t otherPlayerMask = player == game.Player1 ? game.Player2 : game.Player1;
    
    // Comprobamos las posiciones ganadoras de las dos fichas del jugador
    for (int i = 0; i < 2; i++) {

        auto aux = 32-__builtin_ffs(playerMask);
        uint32_t firstBit = 0x80000000>>aux;
        playerMask ^= firstBit;

        if((firstBit>>1&game.Floor3)&~firstCol) count++;
        if((firstBit<<1&game.Floor3)&~lastCol) count++;
        if((firstBit<<4&game.Floor3)&~firstRow&~lastCol) count++;
        if((firstBit>>4&game.Floor3)&~lastRow&~firstCol) count++;
        if((firstBit<<5&game.Floor3)&~firstRow) count++;
        if((firstBit>>5&game.Floor3)&~lastRow) count++;
        if((firstBit<<6&game.Floor3)&~firstRow&~firstCol) count++;
        if((firstBit>>6&game.Floor3)&~lastRow&~lastCol) count++;        
        
        auto Op_aux = 32-__builtin_ffs(otherPlayerMask);
        uint32_t Op_firstBit = 0x80000000>>Op_aux;
        otherPlayerMask ^= Op_firstBit;

        if((Op_firstBit>>1&game.Floor3)&~firstCol) count--;
        if((Op_firstBit<<1&game.Floor3)&~lastCol) count--;
        if((Op_firstBit<<4&game.Floor3)&~firstRow&~lastCol) count--;
        if((Op_firstBit>>4&game.Floor3)&~lastRow&~firstCol) count--;
        if((Op_firstBit<<5&game.Floor3)&~firstRow) count--;
        if((Op_firstBit>>5&game.Floor3)&~lastRow) count--;
        if((Op_firstBit<<6&game.Floor3)&~firstRow&~firstCol) count--;
        if((Op_firstBit>>6&game.Floor3)&~lastRow&~lastCol) count--;   
    }
    return count;
}

int count_set_bits(uint32_t moves)
{
    int count = 0;
    while (moves != 0) {
        moves &= (moves - 1);
        count++;
    }
    return count;
}

int evaluate(const GameState& state, uint32_t player) {
    uint32_t opponent = player == state.Player1 ? state.Player2 : state.Player1;

    // Count the number of winning positions for the player and opponent
    int player_win = win_positions(state, player);
    int opponent_win = win_positions(state, opponent);

    // Evaluate the difference in number of winning positions
    int win_score = (player_win - opponent_win) * 1000;

    // Evaluate the difference in number of movable positions for each player
    int moves_score = 0;
    uint32_t player_moves = vecinos(player, state)& vecinos2(ActualFloor(player, state), state);
    uint32_t opponent_moves = vecinos(opponent, state)& vecinos2(ActualFloor(opponent, state), state);
    int p_moves_count = count_set_bits(player_moves);
    int o_moves_count = count_set_bits(opponent_moves);
    if (p_moves_count > o_moves_count) {
        moves_score = (p_moves_count - o_moves_count) * 10;
    } else if (p_moves_count < o_moves_count) {
        moves_score = (p_moves_count - o_moves_count) * -10;
    }

    // Evaluate the difference in the heights of all positions for each player
    int heights_score = 0;
    for (int i = 0; i < 25; i++) {
        uint32_t position = 0x1 << i;
        if (state.Floor0 & position) {
            if (state.Player1 & position) {
                heights_score += 5;
            } else if (state.Player2 & position) {
                heights_score -= 5;
            }
        } else if (state.Floor1 & position) {
            if (state.Player1 & position) {
                heights_score += 4;
            } else if (state.Player2 & position) {
                heights_score -= 4;
            }
        } else if (state.Floor2 & position) {
            if (state.Player1 & position) {
                heights_score += 3;
            } else if (state.Player2 & position) {
                heights_score -= 3;
            }
        } else if (state.Floor3 & position) {
            if (state.Player1 & position) {
                heights_score += 2;
            } else if (state.Player2 & position) {
                heights_score -= 2;
            }
        } else if (state.Floor4 & position) {
            if (state.Player1 & position) {
                heights_score += 1;
            } else if (state.Player2 & position) {
                heights_score -= 1;
            }
        }
    }

    int total_score = win_score + moves_score + heights_score;
    return player == state.Player1 ? total_score : -total_score;
}

int negamax(GameState state, int depth, int alpha, int beta, bool player) {
    if (depth == 0) {
        return evaluate(state, player);
    }

    int max_Score = std::numeric_limits<int>::min();

    uint32_t aux = 32-__builtin_ffs(player);
    uint32_t vec = vecinos(aux, state)&vecinos2(aux, state);

    auto movess = generate_moves(vec);

    uint32_t ActualPlayer = 0x0;

    if(player){
        ActualPlayer = state.Player1;
    }
    else{
        ActualPlayer = state.Player2;
    }

    for (auto moveAux : movess) {
        
        GameState newMove = Apply_Move(ActualPlayer, state, moveAux);

        int score = -negamax(newMove, depth - 1, -beta, -alpha, !player);

        if (score >= beta) {
            return score;
        }

        if (score > max_Score) {
            max_Score = score;
            alpha = std::max(alpha, score);
        }

        if (alpha >= beta) {
            break;
        }
        
    }   

    cout<<max_Score<<endl;

    return max_Score;
}

uint32_t get_bestMove(uint32_t& Player_board, GameState& game_, bool Turno, int depth)
{
    uint32_t best_move = 0x0;
    int best_score = std::numeric_limits<int>::min();

    //uint32_t token = SelectToken(game_.Player1, game_);
    auto tokenMove = generate_moves(Player_board);

    for (auto moveAux : tokenMove) {
            
        GameState newMove = Apply_Move(Player_board, game_, moveAux);
        
        int score = -negamax(newMove, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), Turno);

        if (score > best_score) {
            best_move = moveAux;
            best_score = score;
        }
        
    }
    return best_move;
}

int main(){

    GameState game;
    //GameState* game_ = &game;
    srand((unsigned) time(NULL));

    bool Turno1 = true;
    bool Colocacion = true;

    int MaxDepth = 4;

    //Colocacion de Jugadores
    
    bool pawn1 = true;
    while (Colocacion)
    {
        if(Turno1){
            while (true)
            {
                std::cout<<"Ingrese coordenadas Jugador 1: " << endl;
                int i = rand() % 5;
                int j = rand() % 5;
                //std::cin>>i>>j;
            
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
                std::cout<<"Ingrese coordenadas Jugador 2: " << endl;
                int i = rand() % 5;
                int j = rand() % 5;
                //std::cin>>i>>j;
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

        
    }

    print(game.FullBoard, game, false);

    //Juego
       // negamax(game, 2, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), game.Player1);

    while (Jugando)
    {
        //Jugador 1

        if(Turno1)
        {
            std::cout<<"Jugador 1.";
            uint32_t token = SelectToken(game.Player1, game);
            
            auto bestMove = get_bestMove(token, game, Turno1, MaxDepth);
            /*
            uint32_t a = SelectToken(game.Player1, game);
            auto b = generate_moves(a);
            
            for (auto x : b)
            {
                print(x, game, true);
                cout << endl;
            }
            break;*/

            

            Move(game.Player1, game, bestMove, token);

           
            //SelectToken(game.Player1, game);
            Turno1 = false;
        }

        else
        {
            std::cout<<"Jugador 2.";

            uint32_t token = SelectToken(game.Player2, game);
            auto bestMove = get_bestMove(token, game, Turno1, MaxDepth);

            /*
            uint32_t m = SelectToken(game.Player2, game);
            auto n = generate_moves(m);
            */
            Move(game.Player2, game, bestMove, token);
            //SelectToken(game.Player2, game);
            Turno1 = true;
            break;
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