package day05

import (
	"math"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type Day05 struct{}

type AlmanacMapLine struct {
	destinationRangeStart int64
	sourceRangeStart      int64
	rangeLength           int64
}

type AlmanacSection struct {
	name    string
	mapping []AlmanacMapLine
}

type Almanac struct {
	seeds    []int64
	sections []AlmanacSection
}

func toMapLine(text string) AlmanacMapLine {
	line := aoc.ToSlice(text)
	return AlmanacMapLine{destinationRangeStart: line[0], sourceRangeStart: line[1], rangeLength: line[2]}
}

func parse(data []string) Almanac {
	seeds := aoc.ToSlice(strings.Split(data[0], ": ")[1])
	var sections []AlmanacSection
	var section AlmanacSection
	for _, line := range data[2:] {
		if len(line) == 0 {
			sections = append(sections, section)
			section = AlmanacSection{}
			continue
		}
		if strings.HasSuffix(line, ":") {
			section.name = line
			continue
		}
		section.mapping = append(section.mapping, toMapLine(line))
	}
	if len(section.name) != 0 {
		sections = append(sections, section)
	}
	return Almanac{seeds: seeds, sections: sections}
}

// transform maps a single value through one section (used for Part 1 points).
func transform(start int64, section AlmanacSection) int64 {
	for _, line := range section.mapping {
		if start >= line.sourceRangeStart && start < line.sourceRangeStart+line.rangeLength {
			return start + (line.destinationRangeStart - line.sourceRangeStart)
		}
	}
	return start
}

func seedToLocation(seed int64, almanac Almanac) int64 {
	result := seed
	for _, section := range almanac.sections {
		result = transform(result, section)
	}
	return result
}

// -- part 2: propagate intervals through the almanac maps --

// Range is a half-open interval [lo, hi).
type Range struct{ lo, hi int64 }

// mapRange splits r against every mapping line of the section. The portion
// overlapping a line's source range is shifted by (dest - src); the rest
// passes through unchanged to be checked against later lines. Source ranges
// in a section are disjoint, so each value is mapped at most once.
func mapRange(r Range, section AlmanacSection) []Range {
	var mapped []Range
	unmapped := []Range{r}
	for _, line := range section.mapping {
		srcLo := line.sourceRangeStart
		srcHi := line.sourceRangeStart + line.rangeLength
		shift := line.destinationRangeStart - line.sourceRangeStart
		var nextUnmapped []Range
		for _, u := range unmapped {
			if u.hi <= srcLo || u.lo >= srcHi {
				nextUnmapped = append(nextUnmapped, u) // no overlap
				continue
			}
			if u.lo < srcLo {
				nextUnmapped = append(nextUnmapped, Range{u.lo, srcLo}) // before
			}
			mapped = append(mapped, Range{
				max(u.lo, srcLo) + shift,
				min(u.hi, srcHi) + shift,
			})
			if u.hi > srcHi {
				nextUnmapped = append(nextUnmapped, Range{srcHi, u.hi}) // after
			}
		}
		unmapped = nextUnmapped
	}
	mapped = append(mapped, unmapped...) // untouched tail passes through
	return mapped
}

func seedRangesToLocationRanges(seedRanges []Range, almanac Almanac) []Range {
	cur := seedRanges
	for _, section := range almanac.sections {
		var next []Range
		for _, r := range cur {
			next = append(next, mapRange(r, section)...)
		}
		cur = next
	}
	return cur
}

func (d Day05) Solve() aoc.Solution {
	almanac := parse(aoc.ReadFile("05"))

	// Part 1: individual seeds.
	var part1 int64 = math.MaxInt64
	for _, seed := range almanac.seeds {
		part1 = min(part1, seedToLocation(seed, almanac))
	}

	// Part 2: seeds come in (start, length) pairs -> intervals.
	var seedRanges []Range
	for i := 0; i+1 < len(almanac.seeds); i += 2 {
		start := almanac.seeds[i]
		seedRanges = append(seedRanges, Range{lo: start, hi: start + almanac.seeds[i+1]})
	}
	locRanges := seedRangesToLocationRanges(seedRanges, almanac)
	var part2 int64 = math.MaxInt64
	for _, r := range locRanges {
		part2 = min(part2, r.lo)
	}

	return aoc.Solution{Part1: strconv.Itoa(int(part1)), Part2: strconv.Itoa(int(part2))}
}
