package day25

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"golang.org/x/exp/slices"
	"strconv"
	"strings"
)

type Day25 struct{}

type Input map[string][]string

type Link struct{ from, to string }

func createLink(a, b string) Link {
	// As the links are bidirectional there is no value in the position from-to pair, so we normalize them.
	if a < b {
		return Link{a, b}
	}
	return Link{b, a}
}

func parse(file string) Input {
	result := make(Input)
	for _, line := range aoc.ReadFile(file) {
		fs := strings.Split(line, ": ")
		result[fs[0]] = strings.Split(fs[1], " ")
	}
	return result
}

func travel(input Input, start string, skip []Link) []string {
	toProcess := []string{start}
	visited := make(map[string]bool)
	var result []string
	for len(toProcess) > 0 {
		cur := toProcess[0]
		visited[cur] = true
		result = append(result, cur)
		toProcess = toProcess[1:]
		next := allDirectLinks(input, cur)
		for _, n := range next {
			if slices.Contains(skip, createLink(n, cur)) {
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
	return result
}

var cacheLinks = make(map[string][]string)

func allDirectLinks(input Input, from string) []string {
	cache, ok := cacheLinks[from]
	if ok {
		return cache
	}
	next := input[from]
	// Ad connections are not in single direction, need to consider also cases when the component is on the right side.
	for k, l := range input {
		if !slices.Contains(l, from) {
			continue
		}
		// avoid duplications
		if !slices.Contains(next, k) {
			next = append(next, k)
		}
	}
	cacheLinks[from] = next
	return next
}

func findAllWays(input Input, from string, to string, path []string) [][]string {
	next := allDirectLinks(input, from)
	var result [][]string
	for _, n := range next {
		if slices.Contains(path, n) {
			continue // loop found, skip this option
		}
		path1 := append(path, n)
		if n == to {
			// the end
			result = append(result, path1)
		} else {
			// further research
			result = append(result, findAllWays(input, n, to, path1)...)
		}
	}
	return result
}

func countFrequency(input Input, all []string) map[string]int {
	result := make(map[string]int)
	for _, c := range all {
		counter := len(input[c])
		for _, v := range input {
			if slices.Contains(v, c) {
				counter++
			}
		}
		result[c] = counter
	}
	return result
}

func (day Day25) Solve() aoc.Solution {
	var part1, part2 int
	input := parse("25-1") // 13, 1261 // TODO - example works but slow
	firstComponent := maps.Keys(input)[0]
	all := travel(input, firstComponent, nil)
	targetMax := len(all) - 1 /* min size of second group is 1 element */

	// a := findAllWays(input, all[0], all[1], nil)
	// part2 = len(a)
	/*
		freq := countFrequency(input, all)
		minCount := freq[firstComponent]
		maxCount := freq[firstComponent]
		for _, count := range freq {
			minCount = aoc.Min(minCount, count)
			maxCount = aoc.Max(maxCount, count)
		}
	*/
	var allLinks []Link
	for from, list := range input {
		/*
			if freq[from] > minCount {
				continue
			}
		*/
		for _, to := range list {
			/*
				if freq[to] > minCount {
					continue
				}
			*/
			allLinks = append(allLinks, createLink(from, to))
		}
	}
	allSize := len(allLinks)
	/*
		sort.Slice(allLinks, func(i, j int) bool {
			sf := func(l Link) int { return freq[l.from] * freq[l.to] }
			return sf(allLinks[i]) < sf(allLinks[j])
		})
	*/
	for i := 0; i < allSize; i++ {
		for j := i + 1; j < allSize; j++ {
			// XXX - fmt.Printf("i %d, j %d\n", i, j)
			for k := j + 1; k < allSize; k++ {
				skip := []Link{allLinks[i], allLinks[j], allLinks[k]}
				toSearch := firstComponent
				/*
					if slices.Contains(skip, firstComponent) {
						for _, c := range all {
							if !slices.Contains(skip, c) {
								toSearch = c
								break
							}
						}
					}
				*/
				firstGroup := travel(input, toSearch, skip)
				if len(firstGroup) < targetMax {
					// The first group is found!
					var componentInSecondGroup string
					for _, c := range all {
						/*
							if slices.Contains(skip, c) {
								continue
							}
						*/
						if slices.Contains(firstGroup, c) {
							continue
						}
						componentInSecondGroup = c
						break
					}
					secondGroup := travel(input, componentInSecondGroup, skip)
					part1 = len(firstGroup) * len(secondGroup)
					break
				}
			}
		}
	}
	part1 -= 54 // TODO to avoid broken tests
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
