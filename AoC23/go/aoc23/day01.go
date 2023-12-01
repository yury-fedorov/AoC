package aoc23

import (
	"strconv"
	"strings"
	"unicode"
)

type Day01 struct{}

func (d Day01) Line(part Part, line string) int {
	return (10 * d.FirstDigit(part, line)) + d.LastDigit(part, line)
}

func (d Day01) FirstDigit(part Part, line string) int {
	a := []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	ac := []rune(line)
	for i, letter := range ac {
		if unicode.IsDigit(letter) {
			return int(letter - '0')
		}
		if part == Part1 {
			continue
		}
		for j, text := range a {
			if strings.HasPrefix(line[i:], text) {
				return j + 1
			}
		}
	}
	return 0
}

func (d Day01) LastDigit(part Part, line string) int {
	a := []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	ac := []rune(line)
	for i := len(ac) - 1; i >= 0; i-- {
		letter := ac[i]
		if unicode.IsDigit(letter) {
			return int(letter - '0')
		}
		if part == Part1 {
			continue
		}
		for j, text := range a {
			if strings.HasPrefix(line[i:], text) {
				return j + 1
			}
		}
	}
	return 0
}

func (d Day01) Solve() Solution {
	part1 := 0
	part2 := 0
	for _, line := range ReadFile("01") {
		part1 += d.Line(Part1, line)
		part2 += d.Line(Part2, line)
	}
	return Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
