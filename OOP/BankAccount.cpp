#include <iostream>
using namespace std;

class BankAccount {
private:
    string owner;
    double balance;
public:
    BankAccount(string o, double b) {
        owner = o;
        balance = b;
    }
    void deposit(double amount) {
        balance += amount;
    }
    void withdraw(double amount) {
        if(amount <= balance) balance -= amount;
        else cout << "Недостаточно средств!\n";
    }
    void show() {
        cout << "Владелец: " << owner << ", Баланс: " << balance << endl;
    }
};

int main() {
    BankAccount acc("Иван", 1000);
    acc.show();
    acc.deposit(500);
    acc.withdraw(200);
    acc.show();
    return 0;
}
