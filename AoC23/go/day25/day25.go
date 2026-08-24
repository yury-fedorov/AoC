package day25

import (
	"cmp"
	"slices"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
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
func travel(input Input, start string, skipSet map[string]bool) []string {
	ok, path := travelWithBomb(input, start, skipSet, "")
	if !ok {
		panic("Not expected")
	}
	return path
}

// Convert link to string key for set lookup
func linkKey(l Link) string {
	return l.from + "|" + l.to
}

// given the starting point, check that travel doesn't pass a bomb
func travelWithBomb(input Input, start string, skipSet map[string]bool, bomb string) (bool, []string) {
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
			if skipSet[linkKey(createLink(n, cur))] {
				continue
			}
			if n == bomb {
				return false, nil
			}
			if visited[n] {
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

// Calculate edge betweenness using BFS from random nodes
func edgeBetweenness(input Input, sampleSize int) map[string]int {
	edgeCount := make(map[string]int)
	allNodes := make([]string, 0, len(input))
	for node := range input {
		allNodes = append(allNodes, node)
	}

	// Sample edges by running BFS from a few nodes and counting edge usage
	for i := 0; i < sampleSize && i < len(allNodes); i++ {
		start := allNodes[i]
		toProcess := []string{start}
		visited := make(map[string]bool)
		parent := make(map[string][]string)
		visited[start] = true

		for len(toProcess) > 0 {
			cur := toProcess[0]
			toProcess = toProcess[1:]
			next := allDirectLinks(input, cur)
			for _, n := range next {
				if !visited[n] {
					visited[n] = true
					parent[n] = append(parent[n], cur)
					toProcess = append(toProcess, n)
				}
			}
		}

		// Backtrack and count edges
		for _, node := range allNodes {
			if visited[node] && node != start {
				for _, p := range parent[node] {
					edgeCount[linkKey(createLink(p, node))]++
				}
			}
		}
	}
	return edgeCount
}

func answer1(input Input) int {
	allLinksList := allLinks(input)

	// Get high-betweenness edges as candidates
	edgeFreq := edgeBetweenness(input, 10)

	// Sort by frequency
	slices.SortFunc(allLinksList, func(a, b Link) int {
		return cmp.Compare(edgeFreq[linkKey(b)], edgeFreq[linkKey(a)])
	})

	allSize := len(allLinksList)
	firstComponent := allLinksList[0].from
	all := travel(input, firstComponent, make(map[string]bool))
	totalNodes := len(all)

	// Only check top candidates (much smaller search space)
	searchLimit := min(len(allLinksList), 100)

	for i := range searchLimit {
		link := allLinksList[i]
		for j := i + 1; j < searchLimit; j++ {
			for k := j + 1; k < allSize; k++ {
				skipSet := make(map[string]bool)
				skipSet[linkKey(link)] = true
				skipSet[linkKey(allLinksList[j])] = true
				skipSet[linkKey(allLinksList[k])] = true

				ok, g1 := travelWithBomb(input, link.from, skipSet, link.to)
				if !ok {
					// bomb was crossed
					continue
				}
				firstGroupCount := len(g1)
				if firstGroupCount > 1 && firstGroupCount < totalNodes {
					g2 := travel(input, link.to, skipSet)
					secondGroupCount := len(g2)
					if firstGroupCount+secondGroupCount == totalNodes {
						return firstGroupCount * secondGroupCount
					}
				}
			}
		}
	}
	return -1
}

func (day Day25) Solve() aoc.Solution {
	var part1, part2 int
	input := parse("25")
	part1 = answer1(input)
	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(part2)}
}
