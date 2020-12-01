#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

long long answer1( const vector<int> & numbers ) {
    const auto n = numbers.size();
    for ( auto i = 0; i < n; i++ ) {
        const auto a = numbers[i];
        for ( auto j = 0; j < i; j++ ) {
            const auto b = numbers[j];
            if ( ( a + b ) == 2020 ) {
                return a * b;
            }
        }
    }
    throw domain_error( "answer is not found" );
}

long long answer2( const vector<int> & numbers ) {
    const auto n = numbers.size();
    for ( auto i = 0; i < n; i++ ) {
        const auto a = numbers[i];
        for ( auto j = 0; j < i; j++ ) {
            const auto b = numbers[j];
            for ( auto k = 0; k < j; k++ ) {
                const auto c = numbers[k];
                if ( ( a + b + c ) == 2020 ) {
                    return a * b * c;
                }
            }
        }
    }
    throw domain_error( "answer is not found" );
}

int main() {
    const bool isFirstAnswer = true;

    ifstream f("input.txt");
    vector<int> numbers;
    string line;
    while (getline(f, line)) {
        numbers.push_back( stoi(line) );
    }

    const auto answer = isFirstAnswer ? answer1(numbers) : answer2(numbers);
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " <<  answer << endl;
    return 0;
}
