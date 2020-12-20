#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <regex>
#include <assert.h>

using namespace std;

typedef pair<string, string> Instruction;
typedef vector<Instruction> Program;
typedef map<char,int> Registers;

inline char argsToRegister( const string & args ) { 
    assert( args.length() == 1 );
    return args[0];
}

pair<char, int> argsToRegOffset( const string & args ) {
    static const regex re2arg("^(\\w), (.+)$");
    smatch what;
    if( regex_match( args, what, re2arg )) {
        return { what[1].str()[0], stoi( what[2] ) };
    } 
    cerr << "Unexpected line: " << args << endl;
    assert( false );
}

int main() {
    Program program;
    const bool isFirstAnswer = false;

    ifstream f("input");
    const regex re("^(\\w{3}) (.+)$");

    string line;
    while (getline(f, line)) {
        smatch what;
        if( regex_match( line, what, re )) {
            program.emplace_back( what[1], what[2] );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    const auto n = program.size();
    Registers registers;
    if ( !isFirstAnswer ) registers['a'] = 1;
    for ( int curOp = 0; curOp >= 0 && curOp < n; ) {
        const auto [ cmd, args ] = program[curOp];
        if ( cmd == "hlf" ) {
            registers[argsToRegister(args)] /= 2;
            curOp++;
        } else if ( cmd == "tpl" ) {
            registers[argsToRegister(args)] *= 3;
            curOp++;
        } else if ( cmd == "inc" ) {
            registers[argsToRegister(args)] += 1;
            curOp++;
        } else if ( cmd == "jmp" ) {
            curOp += stoi(args);
        } else if ( cmd == "jie" ) {
            const auto [r, offset] = argsToRegOffset(args);
            curOp += ( registers[r] % 2 == 0 ) ? offset : 1;
        } else if ( cmd == "jio" ) {
            const auto [r, offset] = argsToRegOffset(args);
            curOp += ( registers[r] == 1 ) ? offset : 1;
        } else assert(false);
    }
    const auto answer = registers['b'];
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << answer << endl;
    assert( answer == ( isFirstAnswer ? 184 : 231 ) );
    return 0;
}