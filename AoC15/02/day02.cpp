#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    ifstream f("input.txt");

    long paper = 0;
    long ribbon = 0;
    string line;
    while (getline(f, line)) {
        stringstream ss(line); // line in format lxwxh
        int l, w, h;
        char d; // delemiter x
        ss >> l >> d >> w >> d >> h; // parsing the line
        vector v = {l,w,h};
        sort(v.begin(), v.end()); // we need 2 lowest values
        paper += 2*l*w + 2*w*h + 2*h*l + v[0]*v[1];
        ribbon += 2*(v[0] + v[1]) + (l*w*h);
    }
    
    // Question 1
    cout << "Answer 1: " << paper << endl;

    // Question 2
    cout << "Answer 2: " << ribbon << endl;

    return 0;
}
