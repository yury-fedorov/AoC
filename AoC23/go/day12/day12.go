package day12

import (
	"slices"
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

func isValid(states []State, groups []int64) bool {
	var curGroup int64
	gn := len(groups)
	gi := 0
	for _, s := range states { // append(states, Operational) {
		if s == Damaged {
			curGroup++
		} else if curGroup > 0 {
			if (gn <= gi) || (groups[gi] != curGroup) {
				return false
			}
			gi++
			curGroup = 0
		}
	}
	if curGroup == 0 {
		return gi == gn
	}
	return (gi+1 == gn) && (groups[gi] == curGroup)
}

var options = []State{Damaged, Operational}

func countPossibleArrangements(pattern []State, groups []int64) int {
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

func toPatternString(pattern []State) string {
	return string(pattern)
}

//---------------------

// DynamicProgramming solution with memoization for Part 2
// Avoids exponential branching by caching results

type MemoKey struct {
	patternIdx  int
	groupIdx    int
	currentSize int
}

func countArrangementsMemo(
	pattern []State,
	groups []int64,
	patternIdx int,
	groupIdx int,
	currentSize int,
	memo map[MemoKey]int,
) int {
	// Base case: reached end of pattern
	if patternIdx == len(pattern) {
		// Check if we've matched all groups exactly
		if groupIdx == len(groups) {
			// All groups matched
			if currentSize == 0 {
				return 1 // Valid arrangement
			}
			return 0 // Extra damaged springs
		}
		// If there's exactly one group left and it matches currentSize
		if groupIdx == len(groups)-1 && int64(currentSize) == groups[groupIdx] {
			return 1 // Valid arrangement
		}
		return 0 // Incomplete or invalid
	}

	// Check memo
	key := MemoKey{patternIdx, groupIdx, currentSize}
	if val, exists := memo[key]; exists {
		return val
	}

	state := pattern[patternIdx]
	var result int

	if state == Operational {
		// '.' - end current group if any
		if currentSize > 0 {
			// We were building a group, verify it matches
			if groupIdx < len(groups) && int64(currentSize) == groups[groupIdx] {
				// Group matches, move to next
				result = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx+1, 0, memo)
			} else {
				// Group size mismatch, invalid
				result = 0
			}
		} else {
			// No group being built, continue
			result = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, 0, memo)
		}
	} else if state == Damaged {
		// '#' - extend current group
		newSize := currentSize + 1
		if groupIdx < len(groups) && int64(newSize) <= groups[groupIdx] {
			// Group size still valid
			result = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, newSize, memo)
		} else {
			// Group exceeded, invalid
			result = 0
		}
	} else {
		// '?' - try both possibilities
		// Try as operational
		resultOp := 0
		if currentSize > 0 {
			if groupIdx < len(groups) && int64(currentSize) == groups[groupIdx] {
				resultOp = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx+1, 0, memo)
			}
		} else {
			resultOp = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, 0, memo)
		}

		// Try as damaged
		resultDam := 0
		newSize := currentSize + 1
		if groupIdx < len(groups) && int64(newSize) <= groups[groupIdx] {
			resultDam = countArrangementsMemo(pattern, groups, patternIdx+1, groupIdx, newSize, memo)
		}

		result = resultOp + resultDam
	}

	memo[key] = result
	return result
}

func countPossibleArrangementsDP(pattern []State, groups []int64) int {
	memo := make(map[MemoKey]int)
	return countArrangementsMemo(pattern, groups, 0, 0, 0, memo)
}

// For Part 2 - unfold the pattern and groups
func unfoldRecord(r Record) Record {
	var unfoldedPattern []State
	var unfoldedGroups []int64

	for i := 0; i < 5; i++ {
		unfoldedPattern = append(unfoldedPattern, r.record...)
		if i < 4 {
			unfoldedPattern = append(unfoldedPattern, Unknown)
		}
		unfoldedGroups = append(unfoldedGroups, r.damagedGroups...)
	}

	return Record{record: unfoldedPattern, damagedGroups: unfoldedGroups}
}

//---------------------

func (day Day12) Solve() aoc.Solution {
	var part1, part2 int
	// 12-1 - 6
	// 12-2 - 21
	records := parse("12") // lines: 1000 max chr in line: 20
	for _, r := range records {
		c1 := countPossibleArrangements(r.record, r.damagedGroups)
		part1 += c1
		// Part 2
		unfolded := unfoldRecord(r)
		c2 := countPossibleArrangementsDP(unfolded.record, unfolded.damagedGroups)
		part2 += c2
	}
	// ie. "?###????????" - [3 2 1] x 5 -> may be formalized with .###. instead of ?###?
	// ie. "?????.???.????#?." - [1 1 1 6] - x5 -> very long execution - also much be solvable in the same way
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
