#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>

using namespace std;

typedef map<string,string> Passport;

bool isValid( const Passport & passport ) {
    const auto n = passport.size();
    static const set<string> OBLIGATORY_FIELDS = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    if ( n < OBLIGATORY_FIELDS.size() ) return false; // just because not enough fields

    for ( const string & field : OBLIGATORY_FIELDS ) {
        if ( passport.find(field) == passport.end() ) {
            return false;
        }
    }
    return true;
}

bool isValidValues( const Passport & passport ) {

    if ( !isValid(passport) ) return false;

    // byr (Birth Year) - four digits; at least 1920 and at most 2002.
    const auto & byr = passport.at("byr");
    const auto byrNumber = stoi(byr);
    if ( byrNumber < 1920 || byrNumber > 2002 || byr.length() != 4 ) return false;

    // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    const auto & iyr = stoi(passport.at("iyr"));
    if ( iyr < 2010 || iyr > 2020 ) return false;

    // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    const auto & eyr = stoi(passport.at("eyr"));
    if ( eyr < 2020 || eyr > 2030 ) return false;

    smatch what;

    // hgt (Height) - a number followed by either cm or in:
        // If cm, the number must be at least 150 and at most 193.
        // If in, the number must be at least 59 and at most 76.
    const static regex RE_HEIGHT("^(\\d+)(\\w{2})$");
    const auto & hgt = passport.at("hgt");
    if( !regex_match( hgt, what, RE_HEIGHT )) {
        return false;
    } else {
        const int height = stoi(what[1]);    
        const string measure = what[2];
        if ( measure == "cm" ) {
            if ( height < 150 || height > 193 ) return false;
        } else if ( measure == "in" ) {
            if ( height < 59 || height > 76 ) return false;
        } else {
            return false; // wrong measure
        }
    }

    // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    const auto & hcl = passport.at("hcl");
    const static regex RE_COLOR("^#[0-9a-f]{6}$");
    if( !regex_match( hcl, what, RE_COLOR )) {
        return false;
    }

    // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    const static set<string> EYE_COLORS = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    const auto & ecl = passport.at("ecl");
    if ( EYE_COLORS.find( ecl ) == EYE_COLORS.end() ) {
        return false;
    }

    // pid (Passport ID) - a nine-digit number, including leading zeroes.
    const static regex RE_PASS_ID("^[0-9]{9}$");
    const auto & pid = passport.at("pid");
    if( !regex_match( pid, what, RE_PASS_ID )) {
        return false;
    }

    return true;
}

int main() {

    vector<Passport> passports;
    Passport curPassport; // due to multiline case

    ifstream f("input.txt");
    regex re("(\\w+):([^\\s]+)");
    string line;
    while (getline(f, line)) {

        if ( line.empty() ) {
            // the previous password is done
            if (!curPassport.empty()) {
                passports.push_back( curPassport );
                curPassport.clear(); // next password is starting
            }
        } else {
            smatch res;
            string::const_iterator searchStart( line.cbegin() );
            while ( regex_search( searchStart, line.cend(), res, re ) )
            {
                curPassport.insert( make_pair(res[1], res[2]) );
                searchStart = res.suffix().first;
            }
        }
    }
    if (!curPassport.empty()) {
        passports.push_back( curPassport );
    }

    const int validCount = count_if( passports.cbegin(), passports.cend(), isValid );
    cout << "Answer 1: " << validCount << endl; 

    const int validCount2 = count_if( passports.cbegin(), passports.cend(), isValidValues );
    cout << "Answer 2: " << validCount2 << endl;

    return 0;
}
