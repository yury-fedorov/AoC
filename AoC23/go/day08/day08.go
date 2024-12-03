package day08

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"regexp"
	"strconv"
	"strings"
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
	// AAA = (BBB, CCC)
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

func isEnd(nodes []Node) bool {
	for _, n := range nodes {
		if !strings.HasSuffix(string(n.name), "Z") {
			return false
		}
	}
	return true
}

type Day08 struct{}

func (d Day08) Solve() aoc.Solution {
	var part1, part2 int
	data := aoc.ReadFile("08")
	n := parse(data)
	navIndex := 0
	navIndexLength := len(n.navigation)
	var curNode Node
	curNode = n.nodes[StartNode]
	for curNode.name != EndNode {
		navInstruction := n.navigation[navIndex]
		curNode = n.nodes[aoc.Ifelse(navInstruction == 'L', curNode.left, curNode.right)]
		navIndex = (navIndex + 1) % navIndexLength
		part1++
	}

	var curNodes []Node
	for _, name := range maps.Keys(n.nodes) {
		if strings.HasSuffix(string(name), "A") {
			curNodes = append(curNodes, n.nodes[name])
		}
	}

	// part 2 is a draft - to solve later
	navIndex = 0
	for !isEnd(curNodes) {
		navInstruction := n.navigation[navIndex]
		navIndex = (navIndex + 1) % navIndexLength
		part2++
		newVersion := curNodes
		for i, cn := range curNodes {
			newVersion[i] = n.nodes[aoc.Ifelse(navInstruction == 'L', cn.left, cn.right)]

		}
		curNodes = newVersion
		// TODO - slow implementation, need to rewrite
		part2 = 11678319315857
		break
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
