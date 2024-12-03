package day15

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
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

type Lens struct {
	label       string
	focalLength int
}

type Operation rune

const (
	Dash Operation = '-'
	Set  Operation = '='
)

func parseLens(seq string) (Operation, Lens) {
	op := aoc.Ifelse(strings.HasSuffix(seq, "-"), Dash, Set)
	var label string
	focalLength := -1
	if op == Set {
		i := strings.Index(seq, "=")
		label = seq[:i]
		var err error
		focalLength, err = strconv.Atoi(seq[i+1:])
		if err != nil {
			panic(seq)
		}
	} else {
		label = seq[:len(seq)-1]
	}
	return op, Lens{label: label, focalLength: focalLength}
}

func (d Day15) Solve() aoc.Solution {
	var part1, part2 int
	initSeq := aoc.ReadFile("15")
	ss := strings.Split(initSeq[0], ",")
	boxes := make(map[int][]Lens)
	for _, seq := range ss {
		part1 += hash(seq)
		op, lens := parseLens(seq)
		boxIndex := hash(lens.label)
		cur := boxes[boxIndex]
		f := func(l Lens) bool { return l.label == lens.label }
		i := slices.IndexFunc(cur, f)
		if op == Set {
			if i < 0 {
				cur = append(cur, lens)
			} else {
				cur[i] = lens
			}
		} else {
			cur = slices.DeleteFunc(cur, f)
		}
		boxes[boxIndex] = cur
	}

	for boxIndex, lenses := range boxes {
		for lensIndex, lens := range lenses {
			part2 += (boxIndex + 1) * (lensIndex + 1) * (lens.focalLength)
		}
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
