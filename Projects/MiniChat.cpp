#include <iostream>
#include <vector>
using namespace std;

struct Message {
    string user;
    string text;
};

int main() {
    vector<Message> chat;
    string user, text;
    cout << "Введите имя пользователя: ";
    cin >> user;

    while(true) {
        cout << user << ": ";
        cin.ignore();
        getline(cin, text);

        if(text == "/exit") break;

        chat.push_back({user, text});

        cout << "\n--- Чат ---\n";
        for(auto &m : chat) {
            cout << m.user << ": " << m.text << endl;
        }
        cout << "------------\n";
    }
    return 0;
}
