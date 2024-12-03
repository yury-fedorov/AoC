#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <set>
#include <vector>

using namespace std;

typedef size_t Card;
typedef list<int> Deck;
typedef vector<Deck> Decks;
typedef set<Decks> History;
enum Player { FIRST, SECOND };

Card next(Deck &d) {
  const Card c = d.front();
  d.pop_front();
  return c;
}

void put(Deck &d, Card c1, Card c2) {
  d.push_back(c1);
  d.push_back(c2);
}

void resize(Deck &d, size_t n) {
  assert(n > 0 && n <= d.size());
  if (n == d.size())
    return;
  auto end = next(d.begin(), n);
  d = list<int>(d.begin(), end);
}

Player recursiveCombat(Decks &decks) {
  History history;
  Deck &d1 = decks[FIRST];
  Deck &d2 = decks[SECOND];

  while (!d1.empty() && !d2.empty()) {
    const auto [pos, isNew] = history.insert(decks);
    if (!isNew)
      return FIRST;

    Card c1 = next(d1);
    Card c2 = next(d2);

    const bool d1l = d1.size() >= c1;
    const bool d2l = d2.size() >= c2;

    Player winner;
    if (d1l && d2l) {
      Decks copy{decks};
      resize(copy[FIRST], c1);
      resize(copy[SECOND], c2);
      winner = recursiveCombat(copy);
    } else {
      winner = (c1 > c2 ? FIRST : SECOND);
    }
    if (winner == SECOND)
      swap(c1, c2); // first card of the winner
    put(winner == FIRST ? d1 : d2, c1, c2);
  }
  return d1.empty() ? SECOND : FIRST;
}

long answer(const Deck &d) {
  long result = 0;
  int n = d.size();
  for (const Card c : d) {
    result += n-- * c;
  }
  return result;
}

auto day22(const bool isFirstAnswer) {
  Decks decks;
  ifstream f("22/input.txt");
  const regex rePlayer("^Player (\\d+):$");
  string line;
  smatch what;
  while (getline(f, line)) {
    if (regex_match(line, what, rePlayer)) {
      const int playerN = stoi(what[1]);
      assert(playerN == 1 || playerN == 2);

      Deck deck;
      while (getline(f, line) && !line.empty()) {
        deck.push_back(stoi(line));
      }
      decks.push_back(deck);
    } else {
      cerr << "Unexpected line: " << line << endl;
      FAIL();
    }
  }
  Deck &d1 = decks[FIRST];
  Deck &d2 = decks[SECOND];
  Deck *d = nullptr;

  if (isFirstAnswer) {
    while (!d1.empty() && !d2.empty()) {
      const Card c1 = next(d1);
      const Card c2 = next(d2);
      Deck &d = c1 > c2 ? d1 : d2;
      const auto [mi, ma] = minmax(c1, c2);
      assert(ma > mi);
      put(d, ma, mi);
    }
    d = d2.empty() ? &d1 : &d2;
  } else {
    d = recursiveCombat(decks) == FIRST ? &d1 : &d2;
  }
  assert(d != nullptr);
  return answer(*d);
}

TEST_CASE("Day22-SLOW", "[22]") {
  REQUIRE(30138 == day22(true));
  return; // takes 16 seconds
  REQUIRE(31587 == day22(false));
}