package day04

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"math"
	"regexp"
	"slices"
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
	number, err := strconv.Atoi(match[1])
	if err != nil {
		panic(line)
	}
	return Card{number: number, winningNumbers: parseNumbers(match[2]), cardNumbers: parseNumbers(match[3])}
}

func points(card Card) int {
	var count int
	for _, w := range card.winningNumbers {
		if slices.Contains(card.cardNumbers, w) {
			count++
		}
	}
	if count == 0 {
		return 0
	}
	return int(math.Pow(2, float64(count-1)))
}

func (d Day04) Solve() aoc.Solution {
	var part1, part2 int
	for _, line := range aoc.ReadFile("04") {
		card := parse(line)
		part1 += points(card)
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
