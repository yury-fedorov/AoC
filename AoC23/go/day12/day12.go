package day12

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
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
	label         string
	record        []State
	damagedGroups []int64
	countUnknown  int
	knownDamaged  int // known positions of damaged
	sumDamaged    int // precise (from damagedGroups)
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
		var sum int
		for _, dgi := range dg {
			sum += int(dgi)
		}
		var known, unknown int
		for _, ri := range record {
			known += aoc.Ifelse(ri == Damaged, 1, 0)
			unknown += aoc.Ifelse(ri == Unknown, 1, 0)
		}
		result = append(result, Record{label: recordGroups[0], record: record, damagedGroups: dg,
			knownDamaged: known, sumDamaged: sum, countUnknown: unknown})
	}
	return result
}

func isValid(states []State, groups []int64) bool {
	var curGroup int64
	gi := 0
	for _, s := range states {
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
	return true
}

func possibleArrangements(r Record) int {
	countUnknown := r.countUnknown
	unknownDamaged := r.sumDamaged - r.knownDamaged
	d := countUnknown - unknownDamaged
	return aoc.Pow(2, d)
}

func (day Day12) Solve() aoc.Solution {
	var part1, part2 int
	records := parse("12") // lines: 1000 max chr in line: 20
	for _, r := range records {
		part1 += possibleArrangements(r)
		part2 = aoc.Max(part2, len(r.label))
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
