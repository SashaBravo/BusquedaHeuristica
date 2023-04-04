#include <iostream>
using namespace std;

// Clase para representar una pieza
class Piece {
private:
    int height;
public:
    Piece(int height) {
        this->height = height;
    }
    int getHeight() {
        return height;
    }
};

// Clase para representar el tablero
class Board {
private:
    Piece* grid[5][5];
public:
    Board() {
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                grid[x][y] = nullptr;
            }
        }
    }
    Piece* getPiece(int x, int y) {
        return grid[x][y];
    }
    void setPiece(int x, int y, Piece* piece) {
        grid[x][y] = piece;
    }
};

// Clase para representar un jugador
class Player {
private:
    int id;
public:
    Player(int id) {
        this->id = id;
    }
    int getId() {
        return id;
    }
};

// Clase para representar el juego
class Game {
private:
    Board board;
    Player* players[2];
    Player* currentPlayer;
public:
    Game() {
        players[0] = new Player(1);
        players[1] = new Player(2);
        currentPlayer = players[0];
    }
    void printBoard() {
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                Piece* piece = board.getPiece(x, y);
                if (piece == nullptr) {
                    cout << ". ";
                } else {
                    cout << piece->getHeight() << " ";
                }
            }
            cout << endl;
        }
    }
    void makeMove(int x, int y, int height) {
        Piece* piece = new Piece(height);
        board.setPiece(x, y, piece);
    }
    bool checkVictory() {
        // Implementa la lógica para verificar si un jugador ha ganado el juego
        return false;
    }
    void switchPlayer() {
        if (currentPlayer == players[0]) {
            currentPlayer = players[1];
        } else {
        currentPlayer = players[0];
    }
}
void play() {
    while (!checkVictory()) {
        cout << "Turno del jugador " << currentPlayer->getId() << endl;
        printBoard();
        int x, y, height;
        cout << "Ingrese las coordenadas x, y y la altura de la pieza separadas por espacios: ";
        cin >> x >> y >> height;
        makeMove(x, y, height);
        switchPlayer();
    }
    cout << "El jugador " << currentPlayer->getId() << " ha ganado!" << endl;
}
};
int main() {
Game game;
game.play();
return 0;
}