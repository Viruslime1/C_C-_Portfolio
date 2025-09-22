#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<string> tasks;
    int choice;
    string task;

    while(true) {
        cout << "\n1. Добавить задачу\n2. Показать задачи\n3. Удалить задачу\n4. Выход\nВыберите: ";
        cin >> choice;

        switch(choice) {
            case 1:
                cout << "Введите задачу: ";
                cin.ignore();
                getline(cin, task);
                tasks.push_back(task);
                break;
            case 2:
                cout << "Ваши задачи:\n";
                for(size_t i=0; i<tasks.size(); i++) {
                    cout << i+1 << ". " << tasks[i] << endl;
                }
                break;
            case 3:
                int num;
                cout << "Введите номер задачи для удаления: ";
                cin >> num;
                if(num > 0 && num <= (int)tasks.size())
                    tasks.erase(tasks.begin()+num-1);
                else
                    cout << "Неверный номер!" << endl;
                break;
            case 4:
                return 0;
            default:
                cout << "Неверный выбор!" << endl;
        }
    }
}
