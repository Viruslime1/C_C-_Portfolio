
---

# 📌 Новый большой проект: **Bank Management System (Банковская система)**

⚡ Это будет консольное приложение, где можно:  
- создавать клиентов,  
- открывать счета,  
- вносить и снимать деньги,  
- переводить между счетами,  
- сохранять всё в файл.  

---

## 📄 Projects/BankSystem.cpp

```cpp
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Account {
    int id;
    string owner;
    double balance;

    Account(int i, string o, double b = 0.0)
        : id(i), owner(o), balance(b) {}
};

class Bank {
private:
    vector<Account> accounts;
    int nextId = 1;
public:
    void loadFromFile(const string &filename) {
        ifstream in(filename);
        if(!in.is_open()) return;

        accounts.clear();
        int id;
        string owner;
        double balance;

        while(in >> id) {
            in.ignore();
            getline(in, owner);
            in >> balance;
            accounts.push_back(Account(id, owner, balance));
            if(id >= nextId) nextId = id + 1;
        }
        in.close();
    }

    void saveToFile(const string &filename) {
        ofstream out(filename);
        for(auto &a : accounts) {
            out << a.id << "\n" << a.owner << "\n" << a.balance << "\n";
        }
        out.close();
    }

    void createAccount(string owner, double balance = 0.0) {
        accounts.push_back(Account(nextId++, owner, balance));
        cout << "Счёт создан!\n";
    }

    void listAccounts() {
        cout << "Список счетов:\n";
        for(auto &a : accounts) {
            cout << a.id << ". " << a.owner << " — Баланс: " << a.balance << endl;
        }
    }

    Account* findAccount(int id) {
        for(auto &a : accounts) {
            if(a.id == id) return &a;
        }
        return nullptr;
    }

    void deposit(int id, double amount) {
        Account* acc = findAccount(id);
        if(acc) {
            acc->balance += amount;
            cout << "Внесено: " << amount << ". Баланс: " << acc->balance << endl;
        } else cout << "Счёт не найден!\n";
    }

    void withdraw(int id, double amount) {
        Account* acc = findAccount(id);
        if(acc) {
            if(acc->balance >= amount) {
                acc->balance -= amount;
                cout << "Снято: " << amount << ". Баланс: " << acc->balance << endl;
            } else cout << "Недостаточно средств!\n";
        } else cout << "Счёт не найден!\n";
    }

    void transfer(int fromId, int toId, double amount) {
        Account* from = findAccount(fromId);
        Account* to = findAccount(toId);
        if(from && to) {
            if(from->balance >= amount) {
                from->balance -= amount;
                to->balance += amount;
                cout << "Переведено " << amount << " от " << from->owner << " к " << to->owner << endl;
            } else cout << "Недостаточно средств!\n";
        } else cout << "Счёт не найден!\n";
    }
};

int main() {
    Bank bank;
    string filename = "bank.txt";
    bank.loadFromFile(filename);

    int choice;
    string owner;
    double amount;
    int id, id2;

    while(true) {
        cout << "\n--- Банковская система ---\n";
        cout << "1. Создать счёт\n2. Список счетов\n3. Пополнить\n4. Снять\n5. Перевести\n6. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch(choice) {
            case 1:
                cin.ignore();
                cout << "Введите имя владельца: ";
                getline(cin, owner);
                bank.createAccount(owner);
                break;
            case 2:
                bank.listAccounts();
                break;
            case 3:
                cout << "Введите ID: ";
                cin >> id;
                cout << "Введите сумму: ";
                cin >> amount;
                bank.deposit(id, amount);
                break;
            case 4:
                cout << "Введите ID: ";
                cin >> id;
                cout << "Введите сумму: ";
                cin >> amount;
                bank.withdraw(id, amount);
                break;
            case 5:
                cout << "Введите ID отправителя: ";
                cin >> id;
                cout << "Введите ID получателя: ";
                cin >> id2;
                cout << "Введите сумму: ";
                cin >> amount;
                bank.transfer(id, id2, amount);
                break;
            case 6:
                bank.saveToFile(filename);
                cout << "Выход...\n";
                return 0;
            default:
                cout << "Неверный выбор!\n";
        }
    }
}
