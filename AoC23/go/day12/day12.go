package day12

import (
	"fmt"
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
	"regexp"
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

// for the second part
func countPossibleArrangements2(pattern []State, groups []int64, mode int) int {
	var pattern2 []State
	var groups2 []int64
	for i := 0; i < copiedForPart2; i++ {
		pattern2 = append(pattern2, pattern...)
		groups2 = append(groups2, groups...)
	}
	return countPossibleArrangementsI(pattern2, groups2, mode)
}

// -- alternative solution 2 (regex) - (Jan 8 2024)

func toPatternString(pattern []State) string {
	return string(pattern)
}

func newPattern(pattern string, index int, ch rune) string {
	a := []rune(pattern)
	a[index] = ch
	return string(a)
}

func fastCountPossibleArrangements3(pattern string, groups *regexp.Regexp) int {
	i := strings.Index(pattern, "?")
	if i < 0 {
		return aoc.Ifelse(groups.MatchString(pattern), 1, 0)
	}
	var result int
	for _, o := range options {
		result += fastCountPossibleArrangements3(newPattern(pattern, i, rune(o)), groups)
	}
	return result
}

func nDemages(n int) string {
	var result string
	for i := 0; i < n; i++ {
		result += "#"
	}
	return result
}

func toRegexp(groups []int64) string {
	// 1, 2 -> .*#.*##.*
	const anyAmountOperational = "\\.*"
	const atLeastOneOperational = "\\.+"
	reStr := "^" + anyAmountOperational
	for _, g := range groups {
		reStr += nDemages((int)(g)) + atLeastOneOperational
	}
	i := strings.LastIndex(reStr, atLeastOneOperational)
	reStr = reStr[:i] + anyAmountOperational + "$"
	return reStr
}

func countPossibleArrangements3(pattern []State, groups []int64) int {
	return fastCountPossibleArrangements3(toPatternString(pattern), regexp.MustCompile(toRegexp(groups)))
}

// -- alternative solution 3 - (Jan 8 2024)

func couldBeValid(dc, uc, gs int) (could bool, precise bool) {
	if dc > gs {
		return false, true
	}
	if dc == gs {
		return true, true
	}
	return (uc + dc) >= gs, false
}

func couldBeValid2(pattern []State, groups []int64) bool {
	var curGroup int64
	gn := len(groups)
	gi := 0
	for _, s := range pattern {
		if s == Unknown {
			return true // so far no logic for Unknown states
		}
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

// like 1st but with couldBeValid
func countPossibleArrangements4Ext(pattern []State, groups []int64, dc, uc, gs int) int {
	could, precise := couldBeValid(dc, uc, gs)
	if !could {
		return 0
	}
	if precise {
		// No need for further undefined parsing, the Damages count is set exact, all others are operational.
		return aoc.Ifelse(isValid(pattern, groups), 1, 0)
	}
	if !couldBeValid2(pattern, groups) {
		return 0
	}
	i := slices.Index(pattern, Unknown)
	if i < 0 {
		return aoc.Ifelse(isValid(pattern, groups), 1, 0)
	}
	var result int
	for _, o := range options {
		newPattern := make([]State, len(pattern))
		copy(newPattern, pattern)
		newPattern[i] = o
		ddc := aoc.Ifelse(o == Damaged, 1, 0)
		result += countPossibleArrangements4Ext(newPattern, groups, dc+ddc, uc-ddc, gs)
	}
	return result
}

const copiedForPart2 = 5

func optimizePattern(pattern []State, groups []int64) []State {
	longestGroup := 1
	longestGroupCount := 0
	for _, g := range groups {
		gi := int(g)
		if gi > longestGroup {
			longestGroup = gi
			longestGroupCount = 1
		} else if longestGroup == gi {
			longestGroupCount += 1
		}
	}
	ps := "." + toPatternString(pattern) + "."
	reStr := fmt.Sprintf("[\\.\\?][#\\?]{%d}[\\.\\?]", longestGroup)
	re := regexp.MustCompile(reStr)
	var result, tail string
	for j := 0; j < longestGroupCount; j++ {
		i := re.FindStringIndex(ps) // TODO - doesn't work here
		if i == nil {
			// too few options
			return pattern
		}
		head := ps[:i[0]]
		tail = ps[i[1]:]
		result += head + "." + nDemages(longestGroup)
		ps = "." + tail
	}
	if re.FindStringIndex(ps) != nil {
		// too many options
		return pattern
	}
	return []State(result + ps)
}

func countPossibleArrangements4(pattern []State, groups []int64) int {
	var dc, uc int
	for _, e := range pattern {
		dc += aoc.Ifelse(e == Damaged, 1, 0)
		uc += aoc.Ifelse(e == Unknown, 1, 0)
	}
	return countPossibleArrangements4Ext(pattern, groups, dc, uc, sum(groups)) // 6.3 sec (5 instead of 5!)
}

func countPossibleArrangementsI(pattern []State, groups []int64, mode int) int {
	// 1 - 1st impl (no regex), fastest so far
	// 2 - 2nd impl (lost)
	// 3 - 3rd impl (with regex), slowest so far
	// 4 - 4th impl (1st + couldBeValid), fastest so far
	switch mode {
	case 4:
		return countPossibleArrangements4(pattern, groups) // 6.3 sec (5 instead of 5!)
	case 1:
		return countPossibleArrangements(pattern, groups) // 56.32 sec (3 instead of 5)
	case 3:
		return countPossibleArrangements3(pattern, groups) // 166.49s (3 instead of 5)
	}
	return countPossibleArrangements(pattern, groups)
}

func (day Day12) Solve() aoc.Solution {
	var part1, part2 int
	// 12-1 - 6
	// 12-2 - 21
	records := parse("12") // lines: 1000 max chr in line: 20
	for _, r := range records {
		c1 := countPossibleArrangementsI(r.record, r.damagedGroups, 1)
		part1 += c1
		// c2 := countPossibleArrangements2(r.record, r.damagedGroups)
		// part2 += c2
	}
	// TODO: fast enough for 5 repetitions of part2 but doesn't calculate properly the part 2
	// TODO: direction - optimize the input pattern (replacing ? with deduced values)
	// ie. "?###????????" - [3 2 1] x 5 -> may be formalized with .###. instead of ?###?
	// ie. "?????.???.????#?." - [1 1 1 6] - x5 -> very long execution - also much be solvable in the same way
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
