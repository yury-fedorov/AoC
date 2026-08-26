package day08

import (
	"maps"
	"regexp"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type NodeName string

type Node struct {
	name  NodeName
	left  NodeName
	right NodeName
}

type Network struct {
	navigation string
	nodes      map[NodeName]Node
}

const (
	StartNode NodeName = "AAA"
	EndNode   NodeName = "ZZZ"
)

func parse(data []string) Network {
	var re = regexp.MustCompile(`([0-9A-Z]+) = \(([0-9A-Z]+), ([0-9A-Z]+)\)`)
	var result Network
	result.nodes = make(map[NodeName]Node)
	result.navigation = data[0]
	for _, line := range data[2:] {
		m := re.FindStringSubmatch(line)
		if m == nil {
			panic(line)
		}
		node := Node{name: NodeName(m[1]), left: NodeName(m[2]), right: NodeName(m[3])}
		result.nodes[node.name] = node
	}
	return result
}

type Day08 struct{}

// stepsToZ counts navigation steps from `start` until a node whose name ends
// in 'Z' is reached. The navigation instructions repeat cyclically.
func stepsToZ(n Network, start NodeName) int {
	navLen := len(n.navigation)
	cur := n.nodes[start]
	steps := 0
	for !strings.HasSuffix(string(cur.name), "Z") {
		nav := n.navigation[steps%navLen]
		cur = n.nodes[aoc.Ifelse(nav == 'L', cur.left, cur.right)]
		steps++
	}
	return steps
}

func gcd(a, b int64) int64 {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func lcm(a, b int64) int64 {
	return a / gcd(a, b) * b // divide first to avoid overflow
}

func (d Day08) Solve() aoc.Solution {
	data := aoc.ReadFile("08")
	n := parse(data)

	// Part 1: single walk AAA -> ZZZ.
	navIndex := 0
	navIndexLength := len(n.navigation)
	curNode := n.nodes[StartNode]
	var part1 int
	for curNode.name != EndNode {
		navInstruction := n.navigation[navIndex]
		curNode = n.nodes[aoc.Ifelse(navInstruction == 'L', curNode.left, curNode.right)]
		navIndex = (navIndex + 1) % navIndexLength
		part1++
	}

	// Part 2: every node ending in 'A' walks in parallel. Each reaches a 'Z'
	// node periodically with first 'Z' at step == cycle length, so the first
	// step where all ghosts are simultaneously on a 'Z' is the LCM of the
	// individual cycle lengths.
	var starts []NodeName
	for name := range maps.Keys(n.nodes) {
		if strings.HasSuffix(string(name), "A") {
			starts = append(starts, name)
		}
	}
	part2 := int64(1)
	for _, s := range starts {
		part2 = lcm(part2, int64(stepsToZ(n, s)))
	}

	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(int(part2))}
}
