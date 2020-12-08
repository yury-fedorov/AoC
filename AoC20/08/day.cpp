#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <numeric>
#include <assert.h>
#include <climits>

using namespace std;

const vector<string> operations { "nop", "acc", "jmp" };
enum Operation { NOP, ACC, JMP };
typedef pair<Operation, int> Instruction;

int main() {
    ifstream f("input.txt");
    regex re("^(\\w{3}) ([-+]\\d+)$");
    smatch what;
    string line;
    vector<Instruction> code;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            const string op = what[1];
            const int arg = stoi(what[2]);
            const auto i = find( operations.cbegin(), operations.cend(), op);
            assert( i != operations.end() );
            code.emplace_back( (Operation)(int)(i - operations.cbegin()), arg );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    for ( auto e : code ) {
        cout << e.first << " " << e.second << endl;
    }

    const bool isFirstAnswer = true;
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
