package day07

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	slices "golang.org/x/exp/slices" // Due to 1.20
	"sort"
	"strconv"
	"strings"
)

// Order of the cards (lower index, more value)
const Cards = "AKQJT98765432"

type HandType int

const (
	FiveOfKind  HandType = iota // where all five cards have the same label: AAAAA
	FourOfKind                  // where four cards have the same label and one card has a different label: AA8AA
	FullHouse                   // where three cards have the same label, and the remaining two cards share a different label: 23332
	ThreeOfKind                 // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
	TwoPair                     // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
	OnePair                     // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
	HighCard                    // where all cards are different
)

func getType(hand []rune) HandType {
	m := make(map[rune]int)
	for _, c := range hand {
		m[c]++
	}
	counts := maps.Values(m)
	slices.Sort(counts)
	maxCount := counts[len(counts)-1]
	differentLabels := len(m)
	if differentLabels == 1 {
		return FiveOfKind
	}
	if differentLabels == 2 {
		// AAAAB or AAABB
		return aoc.Ifelse(maxCount == 4, FourOfKind, FullHouse)
	}
	if maxCount == 3 {
		return ThreeOfKind
	}
	if maxCount == 2 {
		return aoc.Ifelse(differentLabels == 3, TwoPair, OnePair)
	}
	return HighCard
}

func getType2(hand []rune) HandType {
	t1 := getType(hand)
	if t1 == FiveOfKind {
		return FiveOfKind
	}
	m := make(map[rune]int)
	for _, c := range hand {
		m[c]++
	}
	jokerCount := m['J']
	if jokerCount == 0 {
		return t1
	}
	differentLabels := len(m)
	if jokerCount == 4 || differentLabels == 2 {
		// 4 jokers + any card
		// or only jokers and any single card
		return FiveOfKind
	}

	counts := maps.Values(m)
	slices.Sort(counts)
	maxCount := counts[len(counts)-1]

	if jokerCount == 3 {
		// min: 4 same + 1 diff - FourOfKind
		return FourOfKind
	}

	if jokerCount == 2 {

		// 2 cards same label - FourOfKind
		// all different (ABC) - AaaBC - ThreeOfKind or AaBbC - TwoPairs, more precious is ThreeOfKind
		return aoc.Ifelse(differentLabels == 3, FourOfKind, ThreeOfKind)
	}

	// the case with a single joker
	switch maxCount {
	case 3:
		return FourOfKind
	case 2:
		return aoc.Ifelse(differentLabels == 3, FullHouse, ThreeOfKind)
	case 1:
		return OnePair
	}

	panic("Unexpected flow")
}

func compareCards(a, b rune) int {
	if a == b {
		return 0
	}
	ai := strings.IndexRune(Cards, a)
	bi := strings.IndexRune(Cards, b)
	return aoc.Ifelse(ai < bi, 1, -1)
}

func compareHands(a, b []rune) int {
	hta := getType(a)
	htb := getType(b)
	if hta != htb {
		return aoc.Ifelse(hta < htb, 1, -1)
	}
	for i := 0; i < len(a); i++ {
		cc := compareCards(a[i], b[i])
		if cc != 0 {
			return cc
		}
	}
	return 0
}

func compareCards2(a, b rune) int {
	if a == b {
		return 0
	}
	if a == 'J' {
		return -1
	}
	if b == 'J' {
		return 1
	}
	return compareCards(a, b)
}

func compareHands2(a, b []rune) int {
	hta := getType2(a)
	htb := getType2(b)
	if hta != htb {
		return aoc.Ifelse(hta < htb, 1, -1)
	}
	for i := 0; i < len(a); i++ {
		cc := compareCards2(a[i], b[i])
		if cc != 0 {
			return cc
		}
	}
	return 0
}

type HandBet struct {
	hand []rune
	bet  int
}

func parse(line string) HandBet {
	a := strings.Split(line, " ")
	hand := a[0]
	return HandBet{hand: []rune(hand), bet: aoc.Atoi(a[1])}
}

type Day07 struct{}

func (d Day07) Solve() aoc.Solution {
	var part1, part2 int
  data := aoc.ReadFile("07")
	var list []HandBet
	for _, line := range data {
		hb := parse(line)
		list = append(list, hb)
	}

	sort.SliceStable(list, func(i, j int) bool { return compareHands(list[i].hand, list[j].hand) < 0 })
	for i, hb := range list {
		rank := i + 1
		part1 += rank * hb.bet
	}

	sort.SliceStable(list, func(i, j int) bool { return compareHands2(list[i].hand, list[j].hand) < 0 })
	for i, hb := range list {
		rank := i + 1
		part2 += rank * hb.bet
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
