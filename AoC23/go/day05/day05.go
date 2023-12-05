package day05

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
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

func toSlice(text string) []int64 {
	var result []int64
	for _, textNumber := range strings.Split(text, " ") {
		number, err := strconv.Atoi(textNumber)
		if err != nil {
			panic(textNumber)
		}
		result = append(result, int64(number))
	}
	return result
}

func toMapLine(text string) AlmanacMapLine {
	line := toSlice(text)
	return AlmanacMapLine{destinationRangeStart: line[0], sourceRangeStart: line[1], rangeLength: line[2]}
}

func parse(data []string) Almanac {
	seeds := toSlice(strings.Split(data[0], ": ")[1])
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

func transform(start int64, section AlmanacSection) int64 {
	for _, line := range section.mapping {
		if start >= line.sourceRangeStart && start < (line.sourceRangeStart+line.rangeLength) {
			// found the right transformation line
			shift := line.destinationRangeStart - line.sourceRangeStart
			return start + shift
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

// Go 1.20 doesn't have min yet, only in Go 1.21
func min(a, b int64) int64 {
	if a < b {
		return a
	}
	return b
}

func (d Day05) Solve() aoc.Solution {
	var part1, part2 int
	almanac := parse(aoc.ReadFile("05"))
	lowest := almanac.seeds[0]
	for _, seed := range almanac.seeds {
		lowest = min(lowest, seedToLocation(seed, almanac))
	}
	part1 = int(lowest)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
