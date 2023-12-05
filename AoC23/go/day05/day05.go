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
	seeds []int64
	// TODO - sort for quick transform() call usage based on binary search implemented in search.Search()
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

func calcMin(almanac Almanac, startSeed int64, length int64, c chan int64) {
	initialLowest := almanac.seeds[0]
	result := initialLowest
	for seed := startSeed; seed < (startSeed + length); seed++ {
		result = min(result, seedToLocation(seed, almanac))
	}
	c <- result
}

func (d Day05) Solve() aoc.Solution {
	var part1, part2 int
	almanac := parse(aoc.ReadFile("05"))
	initialLowest := almanac.seeds[0]
	lowest := initialLowest
	for _, seed := range almanac.seeds {
		lowest = min(lowest, seedToLocation(seed, almanac))
	}
	part1 = int(lowest)

	// TODO refactor in go routines and channels
	s2 := almanac.seeds
	var count int
	c := make(chan int64)
	for i := 0; i < len(s2); i += 2 {
		startSeed := s2[i]
		length := s2[i+1]
		go calcMin(almanac, startSeed, length, c)
		count++
	}
	for i := 0; i < count; i++ {
		curLowest := <-c
		lowest = min(lowest, curLowest)
	}
	part2 = int(lowest)

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
