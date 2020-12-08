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

    int accumulator {0};
    int next = 0;
    set<int> executed;

    while ( executed.find( next ) == executed.end() ) {
        executed.insert(next);
        const Instruction & i = code[next];
        switch( i.first ) {
            case NOP: next++; break;
            case ACC: accumulator += i.second; next++; break;
            case JMP: next += i.second; break;
            default: 
                cerr << next << ": " << i.first << " " << i.second << endl;
                assert(false);
        }
    }
     cout << accumulator << endl;

    const bool isFirstAnswer = true;
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
