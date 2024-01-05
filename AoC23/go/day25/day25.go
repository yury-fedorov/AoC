package day25

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"slices"
	"strconv"
	"strings"
)

type Day25 struct{}

type Input map[string][]string

func parse(file string) Input {
	result := make(Input)
	for _, line := range aoc.ReadFile(file) {
		fs := strings.Split(line, ": ")
		result[fs[0]] = strings.Split(fs[1], " ")
	}
	return result
}

func travel(input Input, start string, skip []string) []string {
	toProcess := []string{start}
	visited := make(map[string]bool)
	result := make(map[string]bool)
	for len(toProcess) > 0 {
		cur := toProcess[0]
		visited[cur] = true
		result[cur] = true
		toProcess = toProcess[1:]
		next := input[cur]
		// Ad connections are not in single direction, we need to consider also cases when the component is on the right
		// side.
		for k, l := range input {
			if !slices.Contains(l, cur) {
				continue
			}
			next = append(next, k)
		}
		for _, n := range next {
			if slices.Contains(skip, n) {
				continue
			}
			_, seenAlready := visited[n]
			if seenAlready {
				continue
			}
			if slices.Contains(toProcess, n) {
				continue
			}
			toProcess = append(toProcess, n)
		}
	}
	return maps.Keys(result)
}

func (day Day25) Solve() aoc.Solution {
	var part1, part2 int
	input := parse("25-1") // 13, 1261
	firstComponent := maps.Keys(input)[0]
	all := travel(input, firstComponent, nil)
	allSize := len(all)
	targetMax := allSize - 3 /* skipped components */ - 1 /* min size of second group is 1 element */
	for i := 0; i < allSize; i++ {
		for j := i + 1; j < allSize; j++ {
			for k := j + 1; k < allSize; k++ {
				skip := []string{all[i], all[j], all[k]}
				toSearch := firstComponent
				if slices.Contains(skip, firstComponent) {
					for _, c := range all {
						if !slices.Contains(skip, c) {
							toSearch = c
							break
						}
					}
				}
				firstGroup := travel(input, toSearch, skip)
				if len(firstGroup) < targetMax {
					// The first group is found!
					var componentInSecondGroup string
					for _, c := range all {
						if slices.Contains(skip, c) {
							continue
						}
						if slices.Contains(firstGroup, c) {
							continue
						}
						componentInSecondGroup = c
						break
					}
					secondGroup := travel(input, componentInSecondGroup, skip)
					part1 = len(firstGroup) * len(secondGroup) * 0 // TODO - alpha
					break
				}
			}
		}
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
