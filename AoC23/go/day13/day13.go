package day13

import (
	"fmt"
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
)

type Day13 struct{}

type Pattern []string

type ReflectionType bool

const Vertical ReflectionType = false // abccba
const Horizontal ReflectionType = true

type Reflection struct {
	refType   ReflectionType
	firstCell int
}

func parse(file string) []Pattern {
	var result []Pattern
	var pattern Pattern
	for _, line := range aoc.ReadFile(file) {
		if len(line) > 0 {
			pattern = append(pattern, line)
		} else {
			result = append(result, pattern)
			pattern = Pattern{}
		}
	}
	result = append(result, pattern)
	return result
}

func size(p Pattern) (dx int, dy int) {
	return len(p[0]), len(p)
}

func toSlice(p Pattern, rt ReflectionType, i int) string {
	if rt == Horizontal {
		return p[i]
	}
	// Vertical
	var result string
	_, yMax := size(p)
	for y := 0; y < yMax; y++ {
		result += string(([]rune(p[y]))[i])
	}
	return result
}

func reflectionPoint(p Pattern, rt ReflectionType, i int) bool {
	xMax, yMax := size(p)
	a, b := i, i+1
	max := aoc.Ifelse(rt == Vertical, xMax, yMax)
	var count int
	for a >= 0 && b < max {
		if toSlice(p, rt, a) != toSlice(p, rt, b) {
			return false
		}
		count++
		a--
		b++
	}
	return count > 0
}

func reflection(p Pattern, exclude *Reflection) (Reflection, error) {
	xMax, yMax := size(p)
	for x := 0; x < xMax; x++ {
		if reflectionPoint(p, Vertical, x) {
			r := Reflection{Vertical, x}
			if exclude == nil || r != *exclude {
				return r, nil
			}
		}
	}
	for y := 0; y < yMax; y++ {
		if reflectionPoint(p, Horizontal, y) {
			r := Reflection{Horizontal, y}
			if exclude == nil || r != *exclude {
				return r, nil
			}
		}
	}
	return Reflection{}, fmt.Errorf("")
}

func summarize(reflection Reflection) int {
	k := aoc.Ifelse(reflection.refType == Vertical, 1, 100)
	return k * (reflection.firstCell + 1)
}

func fixSmudgeCell(c rune) rune {
	return aoc.Ifelse(c == '#', '.', '#')
}

func fixSmudge(p Pattern, x, y int) Pattern {
	var result Pattern
	for yi, li := range p {
		if yi == y {
			l := []rune(li)
			for xi := 0; xi < len(li); xi++ {
				if xi == x {
					l[xi] = fixSmudgeCell(l[xi])
				}
			}
			result = append(result, string(l))
		} else {
			result = append(result, li)
		}
	}
	return result
}

func reflection2(p Pattern, r1 Reflection) Reflection {
	xMax, yMax := size(p)
	for y := 0; y < yMax; y++ {
		for x := 0; x < xMax; x++ {
			p2 := fixSmudge(p, x, y)
			r2, err := reflection(p2, &r1)
			if err == nil && r2 != r1 {
				return r2
			}
		}
	}
	panic("bad implementation")
}

func (d Day13) Solve() aoc.Solution {
	var part1, part2 int
	patterns := parse("13")
	for _, p := range patterns {
		r1, _ := reflection(p, nil)
		part1 += summarize(r1)
		part2 += summarize(reflection2(p, r1))
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
