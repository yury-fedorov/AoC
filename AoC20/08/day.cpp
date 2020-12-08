#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <assert.h>

using namespace std;

const vector<string> operations { "nop", "acc", "jmp" };
enum Operation { NOP, ACC, JMP };
typedef pair<Operation, int> Instruction;

pair<bool,int> run( const vector<Instruction> & code ) {
    int accumulator {0};
    int next {0};
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
        if ( next == code.size() ) return make_pair(true, accumulator); // proper termination
    }
    return make_pair(false,accumulator);
}

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

    const auto r = run(code);
    cout << "Answer 1: " << r.second << endl;

    for ( auto & i : code ) {
        const Operation originalOp = i.first;
        typedef optional<Operation> OptOp;
        const auto newOp = ( originalOp == NOP ) ? OptOp(JMP) : ( originalOp == JMP ) ? OptOp(NOP) : OptOp();
        if ( newOp.has_value() ) {
            i.first = newOp.value();
            const auto r2 = run(code);
            if (r2.first) {
                cout << "Answer 2: " << r2.second << endl;
                break;
            }
            i.first = originalOp;
        }
    }

    return 0;
}
