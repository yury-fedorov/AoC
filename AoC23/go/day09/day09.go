package day09

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
)

type Sequence []int64

func parse(data []string) []Sequence {
	var result []Sequence
	for _, line := range data {
		seq := Sequence(aoc.ToSlice(line))
		result = append(result, seq)
	}
	return result
}

func down(seq Sequence) Sequence {
	s1 := seq[1:]
	n := len(s1)
	result := make(Sequence, n)
	for i := 0; i < n; i++ {
		result[i] = s1[i] - seq[i]
	}
	return result
}

func isZeros(seq Sequence) bool {
	for _, n := range seq {
		if n != 0 {
			return false
		}
	}
	return true
}

func nextPrev(seq Sequence) (int64, int64) {
	var downList []Sequence
	for !isZeros(seq) {
		downList = append(downList, seq)
		seq = down(seq)
	}
	n := len(downList)
	var next, prev int64
	for i := n - 1; i >= 0; i-- {
		curDown := downList[i]
		next += curDown[len(curDown)-1]
		prev = curDown[0] - prev
	}
	return next, prev
}

type Day09 struct{}

func (d Day09) Solve() aoc.Solution {
	var part1, part2 int64
	data := aoc.ReadFile("09")
	sequences := parse(data)
	for _, s := range sequences {
		next, prev := nextPrev(s)
		part1 += next
		part2 += prev
	}
	return aoc.Solution{strconv.Itoa(int(part1)), strconv.Itoa(int(part2))}
}
