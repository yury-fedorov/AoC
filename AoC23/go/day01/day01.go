package day01

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
	"unicode"
)

type Day01 struct{}

func calibrationValue(part aoc.Part, line string) int {
	return (10 * firstDigit(part, line)) + lastDigit(part, line)
}

var day01TextDigits = []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}

func positionDigit(part aoc.Part, line string, i int) (found bool, digit int) {
	letter := []rune(line)[i]
	if unicode.IsDigit(letter) {
		return true, int(letter - '0')
	}
	if part == aoc.Part2 {
		for j, text := range day01TextDigits {
			if strings.HasPrefix(line[i:], text) {
				return true, j + 1
			}
		}
	}
	return false, 0
}

func firstDigit(part aoc.Part, line string) int {
	for i := range line {
		if found, digit := positionDigit(part, line, i); found {
			return digit
		}
	}
	return 0
}

func lastDigit(part aoc.Part, line string) int {
	for i := len(line) - 1; i >= 0; i-- {
		if found, digit := positionDigit(part, line, i); found {
			return digit
		}
	}
	return 0
}

func (d Day01) Solve() aoc.Solution {
	var part1, part2 int
	for _, line := range aoc.ReadFile("01") {
		part1 += calibrationValue(aoc.Part1, line)
		part2 += calibrationValue(aoc.Part2, line)
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
