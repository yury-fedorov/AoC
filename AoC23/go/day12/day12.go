package day12

import (
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
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
	damagedGroups []int
}

func parse(file string) []Record {
	var result []Record
	for _, line := range aoc.ReadFile(file) {
		parts := strings.Split(line, " ")
		patternStr := parts[0]
		groupsStr := strings.ReplaceAll(parts[1], ",", " ")

		var pattern []State
		for _, r := range patternStr {
			pattern = append(pattern, State(r))
		}

		// aoc.ToSlice returns []int64 in this codebase; convert to []int
		groups64 := aoc.ToSlice(groupsStr)
		groups := make([]int, 0, len(groups64))
		for _, g := range groups64 {
			groups = append(groups, int(g))
		}

		result = append(result, Record{record: pattern, damagedGroups: groups})
	}
	return result
}

// Dynamic programming memoized solution that counts valid replacements for Unknowns
// so that damaged groups match `groups`.
type MemoKey struct {
	patternIdx  int
	groupIdx    int
	currentSize int
}

func countArrangementsMemo(pattern []State, groups []int, patternIdx, groupIdx, currentSize int, memo map[MemoKey]int) int {
	// Base case: reached end of pattern
	if patternIdx == len(pattern) {
		// Valid only if we've matched all groups and aren't mid-group
		if groupIdx == len(groups) {
			if currentSize == 0 {
				return 1
			}
			return 0
		}
		// If there's exactly one remaining group and its size equals currentSize
		if groupIdx == len(groups)-1 && groups[groupIdx] == currentSize {
			return 1
		}
		return 0
	}

	key := MemoKey{patternIdx, groupIdx, currentSize}
	if v, ok := memo[key]; ok {
		return v
	}

	state := pattern[patternIdx]
	res := 0

	switch state {
	case Operational:
		// Operational ends a group if we were building one
		if currentSize > 0 {
			// must match exactly the next expected group
			if groupIdx < len(groups) && groups[groupIdx] == currentSize {
				res = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx+1, 0, memo)
			} else {
				res = 0
			}
		} else {
			res = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, 0, memo)
		}
	case Damaged:
		// extend current group
		newSize := currentSize + 1
		if groupIdx < len(groups) && newSize <= groups[groupIdx] {
			res = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, newSize, memo)
		} else {
			res = 0
		}
	case Unknown:
		// try as operational
		if currentSize > 0 {
			if groupIdx < len(groups) && groups[groupIdx] == currentSize {
				res += countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx+1, 0, memo)
			}
		} else {
			res += countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, 0, memo)
		}
		// try as damaged
		newSize := currentSize + 1
		if groupIdx < len(groups) && newSize <= groups[groupIdx] {
			res += countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, newSize, memo)
		}
	}

	memo[key] = res
	return res
}

// Public DP wrapper
func countPossibleArrangementsDP(pattern []State, groups []int) int {
	memo := make(map[MemoKey]int)
	return countArrangementsMemo(pattern, groups, 0, 0, 0, memo)
}

// unfoldRecord repeats the pattern 5 times inserting an Unknown between blocks
// and repeats the groups sequence 5 times.
func unfoldRecord(r Record) Record {
	var unfoldedPattern []State
	var unfoldedGroups []int

	for i := 0; i < 5; i++ {
		unfoldedPattern = append(unfoldedPattern, r.record...)
		if i < 4 {
			unfoldedPattern = append(unfoldedPattern, Unknown)
		}
		unfoldedGroups = append(unfoldedGroups, r.damagedGroups...)
	}

	return Record{record: unfoldedPattern, damagedGroups: unfoldedGroups}
}

func (day Day12) Solve() aoc.Solution {
	var part1, part2 int
	records := parse("12") // lines: 1000 max chr in line: 20
	for _, r := range records {
		// Use the DP solver for both parts; it's fast and avoids exponential branching.
		c1 := countPossibleArrangementsDP(r.record, r.damagedGroups)
		part1 += c1

		unfolded := unfoldRecord(r)
		c2 := countPossibleArrangementsDP(unfolded.record, unfolded.damagedGroups)
		part2 += c2
	}
	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(part2)}
}
