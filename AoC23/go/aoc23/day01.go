package aoc23

import (
	"strconv"
	"strings"
	"unicode"
)

type Day01 struct{}

func (d Day01) calibrationValue(part Part, line string) int {
	return (10 * d.firstDigit(part, line)) + d.lastDigit(part, line)
}

func (d Day01) textDigit(line string) (found bool, digit int) {
	Day01TextDigits := []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	for i, text := range Day01TextDigits {
		if strings.HasPrefix(line, text) {
			return true, i + 1
		}
	}
	return false, 0
}

func (d Day01) positionDigit(part Part, line string, i int) (found bool, digit int) {
	letter := []rune(line)[i]
	if unicode.IsDigit(letter) {
		return true, int(letter - '0')
	}
	if part == Part1 {
		return false, 0
	}
	return d.textDigit(line[i:])
}

func (d Day01) firstDigit(part Part, line string) int {
	for i := range line {
		if found, digit := d.positionDigit(part, line, i); found {
			return digit
		}
	}
	return 0
}

func (d Day01) lastDigit(part Part, line string) int {
	for i := len(line) - 1; i >= 0; i-- {
		if found, digit := d.positionDigit(part, line, i); found {
			return digit
		}
	}
	return 0
}

func (d Day01) Solve() Solution {
	var part1, part2 int
	for _, line := range ReadFile("01") {
		part1 += d.calibrationValue(Part1, line)
		part2 += d.calibrationValue(Part2, line)
	}
	return Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
