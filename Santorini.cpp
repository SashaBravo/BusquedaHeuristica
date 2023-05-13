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

//tablero de 5x5, 32 bits
uint32_t empty=0x0;
uint32_t full=0x1f1f1f;

uint32_t firstRow=0xf8000000;
uint32_t lastRow=0x00000f80;
uint32_t borde=0xfc631f80;
uint32_t firstCol=0x84210842;
uint32_t lastCol=0x08421084;

bool Jugando = true;

uint32_t var1;
uint32_t var2;

bool boolean = true;

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

//Funciones 

void get_bestMove(vector<uint32_t>& Player_board, GameState& game_, bool Turno, int depth, bool auxFicha, bool isMove);


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

            if(!vecinos){
                if(one&game.Player1){
                    std::cout<<BLUE<<"o"<< RESET << BG_BLACK;
                }

                 if(one&game.Player2){
                    std::cout<<BLUE<<"x"<< RESET << BG_BLACK;
                }

                //else { std::cout<<BLUE<<" " << RESET << BG_BLACK;}
            }
            else{
                if(one&~game.FullBoard){
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

uint32_t vecinos2(uint32_t Floor, GameState game){

    uint32_t Pisos = 0x0;

    if (Floor == game.Floor3){
        //cout<<"Ganaste";
        Pisos = (game.Floor3|game.Floor2|game.Floor1|game.Floor0)&~game.Floor4;
        //return 0x0;
    }
    else if (Floor == game.Floor2){
        Pisos = (game.Floor3|game.Floor2|game.Floor1|game.Floor0)&~game.Floor4;
    }
    else if (Floor == game.Floor1){
        Pisos = (game.Floor2|game.Floor1|game.Floor0)&~(game.Floor3|game.Floor4);
    }
    else{
        Pisos = (game.Floor1|game.Floor0)&~(game.Floor2|game.Floor3|game.Floor4);
    }

    return Pisos;
}

uint32_t ActualFloor(uint32_t player, GameState game){

    if(player&game.Floor3){
        return game.Floor3;
    }
    if(player&game.Floor2){
        return game.Floor2;
    }
    if(player&game.Floor1){
        return game.Floor1;
    }

    return game.Floor0;
}

vector<uint32_t> SelectToken(uint32_t Player_board, GameState game_, bool isMove)
{
    vector<uint32_t> auxVector;
    uint32_t aux_veci1 = 0x0;
    uint32_t aux_veci2 = 0x0;

    

    //Para Moves
    if(isMove)
    {   var1 = 0x0;
        var2 = 0x0;
        uint32_t TurnPlayer = Player_board;

        auto token = 32-__builtin_ffs(TurnPlayer);
        uint32_t t1 = 0x80000000>>token;
        var1 = t1;

        uint32_t floor1 = ActualFloor(t1, game_);
        uint32_t veci1_1 = vecinos(t1, game_);      //Vecinos donde no hay jugadores, borde o piso 4
        uint32_t veci1_2 = vecinos2(floor1, game_);      //Vecinos de piso+1 e inferiores
        aux_veci1 = veci1_1&veci1_2;
        auxVector.push_back(aux_veci1);
        
        TurnPlayer ^= t1;
        
        auto token2 = 32-__builtin_ffs(TurnPlayer);
        uint32_t t2 = 0x80000000>>token2;
        var2 = t2;
        uint32_t floor2 = ActualFloor(t2, game_);
        uint32_t veci2_1 = vecinos(t2, game_);      //Vecinos donde no hay jugadores, borde o piso 4
        uint32_t veci2_2 = vecinos2(floor2, game_);      //Vecinos de piso+1 e inferiores
        aux_veci2 = veci2_1&veci2_2;
        auxVector.push_back(aux_veci2);
    }
    

    //Para Builds
    else{
        var1 = 0x0;
        uint32_t TurnPlayer = Player_board;

        auto token = 32-__builtin_ffs(TurnPlayer);
        uint32_t t1 = 0x80000000>>token;
        var1 = t1;

        uint32_t veci1_1 = vecinos(t1, game_);      //Vecinos donde no hay jugadores, borde o piso 4
        auxVector.push_back(veci1_1);
    }
    
    return auxVector;
}

void Building(GameState& game_, uint32_t next_M){
    uint32_t buildP = next_M;

    if(buildP&game_.Floor1)
    {
        if(buildP&game_.Floor2)
        {
            if(buildP&game_.Floor3)
            {
                game_.Floor4|= buildP;
                game_.Floor3^= buildP;
            }
            game_.Floor3|= buildP;
            game_.Floor2^= buildP;
        }
        game_.Floor2|= buildP;
        game_.Floor1^= buildP;
    }
    game_.Floor1|= buildP;
    game_.Floor0^= buildP;
    
}

void Move(uint32_t& Player_board, GameState& game_, uint32_t next_M, bool auxFicha, bool Turno1, int MaxDepth)
{       
    uint32_t moveP = next_M;

    if(moveP&game_.Floor3){
        Jugando = false;
    }

    if(auxFicha)
    {
        Player_board^=var1;
        game_.FullBoard^=var1;
    }
    else
    {
        Player_board^=var2;
        game_.FullBoard^=var2;
    }

    Player_board|= moveP;
    game_.FullBoard|= Player_board;

    print(game_.FullBoard, game_, false); 

    auto token = SelectToken(next_M, game_, false);
    get_bestMove(token, game_, Turno1, MaxDepth, auxFicha, false);
}

GameState Apply_Move(uint32_t Player_board, GameState game_, uint32_t next_M, bool aaa)
{           
    if(aaa)
    {
        Player_board^=var1;
        game_.FullBoard^=var1;
    }
    else
    {
        Player_board^=var2;
        game_.FullBoard^=var2;
    }
    Player_board|= next_M;
    game_.FullBoard|= Player_board;
    
    return game_;
}

vector<uint32_t> generate_moves(uint32_t possible_mov) {
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

int evaluate(const GameState state, uint32_t player) {
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
        uint32_t position = 0x80000000 >> i;
        if (state.Floor0 & position) {
            if (state.Player1 & position) {
                heights_score += 100;
            } else if (state.Player2 & position) {  
                heights_score -= 100;
            }
        } else if (state.Floor1 & position) {
            if (state.Player1 & position) {
                heights_score += 200;
            } else if (state.Player2 & position) {
                heights_score -= 200;
            }
        } else if (state.Floor2 & position) {
            if (state.Player1 & position) {
                heights_score += 300;
            } else if (state.Player2 & position) {
                heights_score -= 300;
            }
        } else if (state.Floor3 & position) {
            if (state.Player1 & position) {
                heights_score += 400;
            } else if (state.Player2 & position) {
                heights_score -= 400;
            }
        }
    }
    int total_score = win_score + moves_score + heights_score;
    return player == state.Player1 ? total_score : -total_score;
}

int eva_Build(const GameState state, uint32_t player) {
    const int floorDiffWeight = 1;
    const int proximityWeight = 2;
    const int availabilityWeight = 1;
    const int blockingWeight = 10;

    int playerFloor = __builtin_popcount(state.FullBoard & state.Player1);
    int rivalFloor = __builtin_popcount(state.FullBoard & state.Player2);

    int score = 0;

    int floorDiff = playerFloor - rivalFloor;
    score += floorDiff * floorDiffWeight;

    int playerPosition = __builtin_ctz(state.Player1);
    int proximityScore = 0;
    if (playerPosition == 0 || playerPosition == 4 || playerPosition == 20 || playerPosition == 24) {
        proximityScore++;
    }
    score += proximityScore * proximityWeight;

    int availabilityScore = 0;
    if (playerFloor < 3) {
        availabilityScore++;
    }
    score += availabilityScore * availabilityWeight;

    if (playerFloor >= 3 && rivalFloor < 3 && (state.FullBoard & state.Floor4 & state.Player1)) {
        score += blockingWeight;
    }

    return player == 1 ? score : -score;
}


int negamax(GameState state, int depth, int alpha, int beta, bool player, bool isBuild) {
    if (depth == 0) {
        if(isBuild){
            return eva_Build(state, player);
        }
        else{
            return evaluate(state, player);
        }
    }

    int score;
    int max_Score = std::numeric_limits<int>::min();
    uint32_t ActualPlayer = 0x0;
    bool boolean2 = true;

    if(player){
        ActualPlayer = state.Player1;
    }
    else{
        ActualPlayer = state.Player2;
    }

    if(isBuild)
    {
        auto token = SelectToken(ActualPlayer, state, false);

        for (auto tokenAux : token)
        {
            auto movess = generate_moves(tokenAux);
        
            for (auto moveAux : movess) {
                
                GameState newMove = Apply_Move(ActualPlayer, state, moveAux, boolean2);

                score = -negamax(newMove, depth - 1, -beta, -alpha, !player, true);

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
            
            boolean2 = false;
        }
    }
    
    else
    {
        auto token = SelectToken(ActualPlayer, state, true);
        for (auto tokenAux : token)
        {
            auto movess = generate_moves(tokenAux);
        
            for (auto moveAux : movess) {
                
                GameState newMove = Apply_Move(ActualPlayer, state, moveAux, boolean2);

                score = negamax(newMove, depth - 1, alpha, beta, player, false);

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
            
            boolean2 = false;
        }
    }


    return max_Score;
}

void get_bestMove(vector<uint32_t>& Player_board, GameState& game_, bool Turno, int depth, bool auxFicha, bool isMove)
{
    int score;
    uint32_t best_move = 0x0;
    int best_score = std::numeric_limits<int>::min();
    boolean = true;

    uint32_t ActualPlayer = 0x0;

    if(Turno){
        ActualPlayer = game_.Player1;
    }
    else{
        ActualPlayer = game_.Player2;
    }
    if(isMove){

        for (auto token : Player_board)
            {
            auto tokenMove = generate_moves(token);
            for (auto moveAux : tokenMove) {

                GameState newMove = Apply_Move(ActualPlayer, game_, moveAux, boolean);

                score = negamax(newMove, depth - 1 , std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), Turno, true);

                if (score > best_score) {
                    best_move = moveAux;
                    auxFicha = boolean;
                    best_score = score;
                }
            }
            boolean = false;
        }
        if(Turno){
            Move(game_.Player1, game_, best_move, auxFicha, Turno, depth);
        }
        else{
            Move(game_.Player2, game_, best_move, auxFicha, Turno, depth);
        }
        
    }
    
    
    else
    {
        for (auto token : Player_board)
        {
            auto tokenMove = generate_moves(token);
            for (auto moveAux : tokenMove) {

                GameState newMove = Apply_Move(ActualPlayer, game_, moveAux, boolean);

                score = -negamax(newMove, depth - 1 , std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), Turno, false);

                if (score > best_score) {
                    best_move = moveAux;
                    auxFicha = boolean;
                    best_score = score;
                }
            }
            boolean = false;
        }

        Building(game_, best_move);
    }
    
}

int main(){

    GameState game;
    srand((unsigned) time(NULL));

    bool Turno1 = true;
    bool Colocacion = true;

    int MaxDepth = 6;

    //Colocacion de Jugadores
    
    bool pawn1 = true;
    while (Colocacion)
    {
        if(Turno1){
            while (true)
            {
                int i = rand() % 5;
                int j = rand() % 5;
            
                if (game.FullBoard != (game.FullBoard |= 0x80000000>>(j*5+i)))
                {
                    game.Player1 |= 0x80000000>>(j*5+i);
                    game.FullBoard |= game.Player1;
                    break;
                }
            }
            Turno1 = false;
        }
        
        else{
            while (true)
            {
                int i = rand() % 5;
                int j = rand() % 5;
            
                if (game.FullBoard != (game.FullBoard |= 0x80000000>>(j*5+i)))
                {
                    game.Player2 |= 0x80000000>>(j*5+i);
                    game.FullBoard |= game.Player2;
                    break;
                }
            }

            Turno1 = true;

            if (pawn1) { pawn1 = false; }
            else { Colocacion = false; }
        }
    }

    print(game.FullBoard, game, false);

    //Juego
    while (Jugando)
    {
        //Jugador 1
        if(Turno1)
        {
            std::cout<<"Jugador 1."<<endl;
            
            bool auxFicha = true;

            auto token = SelectToken(game.Player1, game, true);
            get_bestMove(token, game, Turno1, MaxDepth, auxFicha, true);

            Turno1 = false;
            //break;
        }

        else
        {
            std::cout<<"Jugador 2."<<endl;

            bool auxFicha = true;
            
            auto token = SelectToken(game.Player2, game, true);
            get_bestMove(token, game, Turno1, MaxDepth, auxFicha, true);

            Turno1 = true;
            //break;
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
    print(game.FullBoard, game, false);
}