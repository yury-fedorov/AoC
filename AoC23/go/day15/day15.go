package day15

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
)

type Day15 struct{}

func hash(seq string) int {
	var result int
	for _, c := range seq {
		ascii := int(c)
		result += ascii
		result *= 17
		result = result % 256
	}
	return result
}

func (d Day15) Solve() aoc.Solution {
	var part1, part2 int
	initSeq := aoc.ReadFile("15")
	ss := strings.Split(initSeq[0], ",")
	for _, seq := range ss {
		part1 += hash(seq)
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
