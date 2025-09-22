#include <iostream>
using namespace std;

int main() {
    int a, b;
    char op;

    cout << "Введите выражение (например: 3 + 4): ";
    cin >> a >> op >> b;

    switch(op) {
        case '+': cout << a + b; break;
        case '-': cout << a - b; break;
        case '*': cout << a * b; break;
        case '/': 
            if(b != 0) cout << (double)a / b;
            else cout << "Ошибка: деление на 0!";
            break;
        default: cout << "Неизвестная операция!";
    }

    return 0;
}
