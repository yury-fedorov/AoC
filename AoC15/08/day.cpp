#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

string replaceAll( string_view str, string_view oldText, string_view newText ) {
    string s(str);
    const auto oldSize = oldText.length();
    while (true) {
        const auto i = s.find(oldText);
        if ( i == string::npos ) return s;
        s.replace(i, oldSize, newText );
    }
}

string replaceAllRegEx( const string & s, const string & oldRegEx, const string & newText ) {
    regex re(oldRegEx);
    return regex_replace(s,re,newText);
}

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

    cout << "\x1k" << endl;

    auto codeLength = 0;
    auto textLength = 0;

    string line;
    while (getline(f, line)) {
        const auto l = line.length();
        codeLength += l; 

        auto s = line.substr( 1, l - 2 ); // remove first and last
        s = replaceAll(s, "\\\"", "\"" );
        s = replaceAll(s, "\\\\", "\\" );
        s = replaceAllRegEx(s, "(.*)(\\\\x[0-9a-f]{2})(.*)", "$1?$3" ); // does not matter what is the character but only the size
        const auto l1 = s.length();
        textLength += l1;
        cout << "[" << line << "] - {" << s << "} = " << l << " - " << l1 << " = " << (l - l1) <<  endl; 
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << (codeLength - textLength) << "" << endl; // 21026 - too high

    return 0;
}
