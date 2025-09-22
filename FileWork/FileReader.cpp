#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream out("example.txt");
    out << "Привет, мир!" << endl;
    out.close();

    ifstream in("example.txt");
    string line;
    while(getline(in, line)) {
        cout << "Считано из файла: " << line << endl;
    }
    in.close();
    return 0;
}
