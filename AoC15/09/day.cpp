#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

    int codeLength = 0;
    int textLength = 0;

    string line;
    while (getline(f, line)) {
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << (codeLength - textLength) << endl;

    return 0;
}
