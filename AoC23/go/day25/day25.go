package day25

import (
	"cmp"
	"github.com/yury-fedorov/AoC/AoC23/aoc"
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

// original map given as input
func parse(file string) Input {
	result := make(Input)
	for _, line := range aoc.ReadFile(file) {
		fs := strings.Split(line, ": ")
		result[fs[0]] = strings.Split(fs[1], " ")
	}
	return result
}

var cacheLinks = make(map[string][]string)

func allDirectLinks(input Input, from string) []string {
	cache, ok := cacheLinks[from]
	if ok {
		return cache
	}
	next, _ := input[from]
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

// returns all defined links
func allLinks(input Input) []Link {
	var result []Link
	for from, list := range input {
		for _, to := range list {
			result = append(result, createLink(from, to))
		}
	}
	return result
}

// given the starting point, returns all points you may arrive to
func travel(input Input, start string, skip []Link) []string {
	ok, path := travelWithBomb(input, start, skip, "")
	if !ok {
		panic("Not expected")
	}
	return path
}

// given the starting point, check that travel doesn't pass a bomb
func travelWithBomb(input Input, start string, skip []Link, bomb string) (bool, []string) {
	toProcess := []string{start}
	visited := make(map[string]bool)
	var result []string
	for len(toProcess) > 0 {
		cur := toProcess[0]
		if cur == bomb {
			return false, nil
		}
		visited[cur] = true
		result = append(result, cur)
		toProcess = toProcess[1:]
		next := allDirectLinks(input, cur)
		for _, n := range next {
			if slices.Contains(skip, createLink(n, cur)) {
				continue
			}
			if n == bomb {
				return false, nil
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
	return true, result
}

func answer1(input Input) int {
	allLinks := allLinks(input)
	slices.SortFunc(allLinks, func(a, b Link) int {
		a1 := len(allDirectLinks(input, a.from))
		a2 := len(allDirectLinks(input, a.to))
		b1 := len(allDirectLinks(input, b.from))
		b2 := len(allDirectLinks(input, b.to))
		return cmp.Compare(min(a1, a2), min(b1, b2))
	})
	allSize := len(allLinks)
	firstComponent := allLinks[0].from
	all := travel(input, firstComponent, nil)
	totalNodes := len(all)
	shortSize := allSize
	for i := 0; i < shortSize; i++ {
		link := allLinks[i]
		for j := i + 1; j < shortSize; j++ {
			for k := j + 1; k < allSize; k++ {
				skip := []Link{link, allLinks[j], allLinks[k]}
				ok, g1 := travelWithBomb(input, link.from, skip, link.to)
				if !ok {
					// bomb was crossed
					continue
				}
				firstGroupCount := len(g1)
				if firstGroupCount > 1 && firstGroupCount < totalNodes {
					g2 := travel(input, link.to, skip)
					secondGroupCount := len(g2)
					if firstGroupCount+secondGroupCount != totalNodes {
						// something went wrong
						continue
					}
					return firstGroupCount * secondGroupCount
				}
			}
		}
	}
	return -1
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
	part1 = answer1(input)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
