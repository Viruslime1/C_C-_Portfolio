#include <iostream>
using namespace std;

#define MAX 100

class Stack {
    int top;
    int arr[MAX];
public:
    Stack() { top = -1; }
    void push(int x) {
        if(top >= MAX-1) cout << "Стек переполнен\n";
        else arr[++top] = x;
    }
    void pop() {
        if(top < 0) cout << "Стек пуст\n";
        else top--;
    }
    int peek() {
        if(top < 0) return -1;
        return arr[top];
    }
    bool isEmpty() { return top < 0; }
};

int main() {
    Stack s;
    s.push(10);
    s.push(20);
    cout << "Верхний элемент: " << s.peek() << endl;
    s.pop();
    cout << "Верхний элемент после pop: " << s.peek() << endl;
    return 0;
}
