package aoc23

import (
	"bufio"
	"os"
	"slices"
	"strconv"
	"strings"
	"unicode"
)

type Day01 struct{}

func (d Day01) Line(line string) int {
	lastDigit := 0
	firstDigit := 0
	a := []rune(line)
	for _, letter := range a {
		if unicode.IsDigit(letter) {
			firstDigit = int(letter - '0')
			break
		}
	}
	slices.Reverse(a)
	for _, letter := range a {
		if unicode.IsDigit(letter) {
			lastDigit = int(letter - '0')
			break
		}
	}
	return (10 * firstDigit) + lastDigit
}

func (d Day01) Line2(line string) int {
	return (10 * d.FirstDigit(line)) + d.LastDigit(line)
}

func (d Day01) FirstDigit(line string) int {
	a := []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	ac := []rune(line)
	for i, letter := range ac {
		if unicode.IsDigit(letter) {
			return int(letter - '0')
		}
		for j, text := range a {
			if strings.HasPrefix(line[i:], text) {
				return j + 1
			}
		}
	}
	return 0
}

func (d Day01) LastDigit(line string) int {
	a := []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	ac := []rune(line)
	for i := len(ac) - 1; i >= 0; i-- {
		letter := ac[i]
		if unicode.IsDigit(letter) {
			return int(letter - '0')
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
	file, err := os.Open("../data/01.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()
	fileScanner := bufio.NewScanner(file)
	fileScanner.Split(bufio.ScanLines)
	part1 := 0
	part2 := 0
	for fileScanner.Scan() {
		line := fileScanner.Text()
		part1 += d.Line(line)
		part2 += d.Line2(line)
	}
	return Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
