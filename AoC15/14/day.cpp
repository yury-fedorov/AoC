#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <regex>
#include <numeric>

using namespace std;

typedef tuple<int,int,int> Data;

struct Deer {
    const Data data;
    int distance;
    int points;
    Deer( const Data & data_ ) : data(data_), distance(0), points(0) {}
};

auto distance( const Data & d, const int t ) {
    const auto speed = get<0>(d);
    const auto fly = get<1>(d);
    const auto rest = get<2>(d);
    const auto fullInternval = fly + rest;
    const auto n = t / fullInternval;
    const auto fraction = t % fullInternval;
    const auto distanceInterval = speed * fly;
    return ( ( n * fly ) + min(fraction, fly) ) * speed;
}

int main() {
    map<string, Data> deers;

    ifstream f("input.txt");
    // Dancer can fly 27 km/s for 5 seconds, but then must rest for 132 seconds.
    regex re("(\\w+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\w+) seconds.");
    string line;
    while (getline(f, line)) {       
        smatch what;
        if( regex_match( line, what, re )) {
            deers.insert( make_pair( what[1], make_tuple( stoi(what[2]), stoi(what[3]), stoi(what[4])) ) );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    vector<shared_ptr<Deer>> running; // for the second question

    const auto T = 2503;
    auto dmax = 0;
    for ( auto p : deers ) {
        auto d = distance( p.second, T );
        dmax = max(dmax, d);
        running.push_back( make_shared<Deer>(p.second) ); // for the second question (initialization)
    }

    cout << "Answer 1: " << dmax  << endl;

    const auto delta = [](const Data & d, int t ) { return distance(d, t) - distance(d, t-1); };
    auto curMax = 0;
    set<shared_ptr<Deer>> leaders;
    for ( auto t = 1; t <= T; t++ ) {
        for ( auto & d : running ) {
            const auto dx = delta(d->data, t);
            d->distance += dx;
            if ( curMax < d->distance ) {
                // new leader
                curMax = d->distance;
                leaders.clear();
                leaders.insert(d);
            } else if ( curMax == d->distance ) {
                // yet another leader
                leaders.insert(d);
            }
        }
        // now we must update points for all leaders
        for ( auto & l : leaders ) {
            l->points++;
        }
    }
    const auto maxByPoints = max_element( running.cbegin(), running.cend(), 
        []( const shared_ptr<Deer> & a, const shared_ptr<Deer> & b ) { return a->points < b->points; } );
    cout << "Answer 2: " << (*maxByPoints)->points << endl;

    return 0;
}
