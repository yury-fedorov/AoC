#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>

using namespace std;

typedef vector<int> Ticket;
typedef vector<Ticket> Tickets;
typedef pair<int, int> Range;
typedef vector<Range> Rule;
typedef map<string, Rule> Rules;
typedef vector<set<string>> ValidFields;

smatch parse(const string &line, const regex &re) {
  smatch what;
  if (regex_match(line, what, re)) {
    return what;
  }
  cerr << "Unexpected line: " << line << endl;
  throw domain_error("Unexpected format");
}

vector<int> parseList(const string &line, const regex &re) {
  vector<int> result;
  smatch res;
  string::const_iterator searchStart(line.cbegin());
  while (regex_search(searchStart, line.cend(), res, re)) {
    const auto n = stoi(res[1]);
    result.push_back(n);
    searchStart = res.suffix().first;
  }
  return result;
}

bool matchRule(const int fieldValue, const Rule &rule) {
  for (const auto &r : rule) {
    if (fieldValue >= r.first && fieldValue <= r.second) {
      return true;
    }
  }
  return false;
}

bool removeFieldFromOptions(const string field, size_t validField,
                            ValidFields &fields) {
  bool isErased = false;
  for (size_t fi = 0; fi < fields.size(); fi++) {
    if (fi != validField) {
      isErased |= fields[fi].erase(field) > 0;
      // could happen that at this point this set is only one again
      // afterFieldRemoved( fi, fields ); --> recursion
    }
  }
  return isErased;
}

bool afterFieldRemoved(int fieldIndex, ValidFields &fields) {
  const int n = fields[fieldIndex].size();
  assert(n > 0);
  return (n == 1) ? removeFieldFromOptions(*fields[fieldIndex].begin(),
                                           fieldIndex, fields)
                  : false;
}

bool clean(ValidFields &fields) {
  bool isErased = false;
  for (size_t f = 0; f < fields.size(); f++) {
    const int n = fields[f].size();
    assert(n > 0);
    if (n == 1)
      isErased |= removeFieldFromOptions(*fields[f].begin(), f, fields);
  }
  return isErased;
}

TEST_CASE("Day16", "[16]") {
  Rules rules;

  ifstream f("16/input");
  string line;

  const regex reRule("^([a-z ]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
  while (getline(f, line) && !line.empty()) {
    auto &&m = parse(line, reRule);
    const Rule rule = {{stoi(m[2]), stoi(m[3])}, {stoi(m[4]), stoi(m[5])}};
    rules.insert(make_pair(m[1], rule));
  }
  getline(f, line);
  assert(line == "your ticket:");
  const regex reTicket("(\\d+)");
  getline(f, line);
  const Ticket myTicket = parseList(line, reTicket);

  getline(f, line);
  assert(line.empty());

  getline(f, line);
  assert(line == "nearby tickets:");
  Tickets tickets;
  while (getline(f, line)) {
    const Ticket &&ticket = parseList(line, reTicket);
    tickets.push_back(ticket);
  }

  Tickets goodTickets;
  long ticketScanningErrorRate{0};
  for (const auto &t : tickets) {
    bool isGoodTicket = true;
    for (const auto &f : t) {
      bool isGoodField = false;
      for (const auto &[name, rule] : rules) {
        isGoodField = matchRule(f, rule);
        if (isGoodField)
          break;
      }
      if (!isGoodField) {
        ticketScanningErrorRate += f;
        isGoodTicket = false;
      }
    }
    if (isGoodTicket)
      goodTickets.push_back(t);
  }

  REQUIRE(ticketScanningErrorRate == 24980);

  // part 2
  const int fieldCount = myTicket.size();
  set<string> fullSet;
  for (const auto &[name, rule] : rules) {
    fullSet.insert(name);
  }

  ValidFields valid(fieldCount, fullSet);
  Tickets allGood(goodTickets);
  allGood.push_back(myTicket);
  for (const auto &t : allGood) {
    for (int f = 0; f < fieldCount; f++) {

      if (valid[f].size() == 1) {
        // we need to erase this field from all other fields
        if (removeFieldFromOptions(*valid[f].begin(), f, valid)) {
          while (clean(valid)) {
          }
        }
      }

      for (const auto &[name, rule] : rules) {
        if (!matchRule(t.at(f), rule)) {
          valid[f].erase(name);
          if (afterFieldRemoved(f, valid)) {
            while (clean(valid)) {
            }
          }
        }
      }
    }
  }

  long long answer2 = 1;
  for (int i = 0; i < fieldCount; i++) {
    const auto &s = valid[i];
    assert(s.size() == 1);
    const string &name = *s.begin();
    if (name.find("departure") == 0) {
      // cout << name << " = " << myTicket[i] << endl;
      answer2 *= myTicket[i];
    }
  }
  REQUIRE(answer2 == 809376774329);
}