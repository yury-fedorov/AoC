package day12

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
	"strconv"
	"strings"
)

type Day12 struct{}

type State rune

const (
	Operational State = '.'
	Damaged     State = '#'
	Unknown     State = '?'
)

type Record struct {
	record        []State
	damagedGroups []int64
}

func parse(file string) []Record {
	var result []Record
	for _, line := range aoc.ReadFile(file) {
		recordGroups := strings.Split(line, " ")
		var record []State
		for _, re := range recordGroups[0] {
			record = append(record, State(re))
		}
		dg := aoc.ToSlice(strings.ReplaceAll(recordGroups[1], ",", " "))
		result = append(result, Record{record: record, damagedGroups: dg})
	}
	return result
}

func count(record []State, state State) int {
	var result int
	for _, ri := range record {
		result += aoc.Ifelse(ri == state, 1, 0)
	}
	return result
}

func sum(groups []int64) int {
	var result int
	for _, g := range groups {
		result += int(g)
	}
	return result
}

func isValid(states []State, groups []int64) bool {
	var curGroup int64
	gi := 0
	for _, s := range append(states, Operational) {
		if s == Damaged {
			curGroup++
		} else {
			if curGroup > 0 {
				if len(groups) <= gi {
					return false
				}
				if groups[gi] != curGroup {
					return false
				}
				gi++
			}
			curGroup = 0
		}
	}
	return gi == len(groups)
}

func couldBeValid(pattern []State, groups []int64) bool {
	dc := count(pattern, Damaged)
	gs := sum(groups)
	if dc > gs {
		return false
	}
	if dc == gs {
		return true
	}
	uc := count(pattern, Unknown)
	return (uc + dc) >= gs
}

var options = []State{Damaged, Operational}

func countPossibleArrangements(pattern []State, groups []int64) int {
	/*
		if !couldBeValid(pattern, groups) {
			return 0
		}
	*/
	i := slices.Index(pattern, Unknown)
	if i < 0 {
		return aoc.Ifelse(isValid(pattern, groups), 1, 0)
	}
	var result int
	for _, o := range options {
		newPattern := make([]State, len(pattern))
		copy(newPattern, pattern)
		newPattern[i] = o
		result += countPossibleArrangements(newPattern, groups)
	}
	return result
}

func countPossibleArrangements2(pattern []State, groups []int64) int {
	var pattern2 []State
	var groups2 []int64
	for i := 0; i < 5; i++ {
		pattern2 = append(pattern2, pattern...)
		groups2 = append(groups2, groups...)
	}
	return countPossibleArrangements(pattern2, groups2)
}

func (day Day12) Solve() aoc.Solution {
	var part1, part2 int
	records := parse("12") // lines: 1000 max chr in line: 20
	for _, r := range records {
		part1 += countPossibleArrangements(r.record, r.damagedGroups)
		// part2 += countPossibleArrangements2(r.record, r.damagedGroups)
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
