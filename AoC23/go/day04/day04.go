package day04

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"regexp"
	slices "golang.org/x/exp/slices" // Due to 1.20
	"strconv"
	"strings"
)

type Day04 struct{}

type Card struct {
	number         int
	winningNumbers []int
	cardNumbers    []int
}

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
var r1 = regexp.MustCompile(`Card +(\d+): (.+) \| (.+)`)

func parseNumbers(line string) []int {
	var result []int
	numbers := strings.Split(strings.TrimSpace(line), " ")
	for _, n := range numbers {
		cleanN := strings.TrimSpace(n)
		if len(cleanN) == 0 {
			continue
		}
		number, err := strconv.Atoi(cleanN)
		if err != nil {
			panic(n)
		}
		result = append(result, number)
	}
	return result
}

func parse(line string) Card {
	match := r1.FindStringSubmatch(line)
	number := aoc.Atoi(match[1])
	return Card{number: number, winningNumbers: parseNumbers(match[2]), cardNumbers: parseNumbers(match[3])}
}

// call repeats multiple times
var matchingNumbersCache = make(map[int]int)

func matchingNumbers(card Card) int {
	result, found := matchingNumbersCache[card.number]
	if found {
		return result
	}
	for _, w := range card.winningNumbers {
		if slices.Contains(card.cardNumbers, w) {
			result++
		}
	}
	matchingNumbersCache[card.number] = result
	return result
}

func points(card Card) int {
	count := matchingNumbers(card)
	var result int
	if count > 0 {
		result = aoc.Pow(2, count-1)
	}
	return result
}

func countCards(cards []Card, index int) map[int]int {
	card := cards[index]
	count := matchingNumbers(card)
	var result = make(map[int]int)
	result[card.number] = 1
	for di := 1; di <= count; di++ {
		r := countCards(cards, index+di)
		for cn, cc := range r {
			result[cn] = result[cn] + cc
		}
	}
	return result
}

func (d Day04) Solve() aoc.Solution {
	var part1, part2 int
	var cards []Card
	for _, line := range aoc.ReadFile("04") {
		cards = append(cards, parse(line))
	}

	wonCards := make(map[int]int)
	for index, card := range cards {
		cardPoints := points(card)
		part1 += cardPoints
		wc := countCards(cards, index)
		for cn, cc := range wc {
			wonCards[cn] = wonCards[cn] + cc
		}
	}

	for _, won := range wonCards {
		part2 += won
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
