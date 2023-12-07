package day07

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
)

// Order of the cards (lower index, more value)
const Cards = "AKQJT98765432" 

type HandType int

const(
FiveOfKind HandType = iota, // where all five cards have the same label: AAAAA
FourOfKind, // where four cards have the same label and one card has a different label: AA8AA
FullHouse, // where three cards have the same label, and the remaining two cards share a different label: 23332
ThreeOfKind, // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
TwoPair, // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
OnePair, // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
HighCard, // where all cards are different
)

type Day07 struct{}

func (d Day07) Solve() aoc.Solution {
	var part1, part2 int
	data := ReadFile("07-1")
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
