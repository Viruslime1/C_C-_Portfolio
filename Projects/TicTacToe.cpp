#include <iostream>
using namespace std;

char board[3][3] = {{'1','2','3'},
                    {'4','5','6'},
                    {'7','8','9'}};

void drawBoard() {
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool checkWin() {
    for(int i=0;i<3;i++) {
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2]) return true;
        if(board[0][i]==board[1][i] && board[1][i]==board[2][i]) return true;
    }
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2]) return true;
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0]) return true;
    return false;
}

int main() {
    int move;
    char turn = 'X';
    int moves = 0;
    while(true) {
        drawBoard();
        cout << "Ход игрока " << turn << ": ";
        cin >> move;

        int row = (move-1)/3;
        int col = (move-1)%3;

        if(board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = turn;
            moves++;
            if(checkWin()) {
                drawBoard();
                cout << "Игрок " << turn << " победил!" << endl;
                break;
            }
            if(moves==9) {
                drawBoard();
                cout << "Ничья!" << endl;
                break;
            }
            turn = (turn=='X') ? 'O' : 'X';
        } else {
            cout << "Клетка занята, попробуйте снова.\n";
        }
    }
    return 0;
}
