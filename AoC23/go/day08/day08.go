package day08

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"regexp"
	"strconv"
)

type NodeName string

type Node struct {
   name NodeName
   left NodeName
   right NodeName
}

type Network struct {
   navigation string
   nodes map[NodeName]Node
}

const (
   StartNode NodeName = "AAA"
   EndNode NodeName = "ZZZ"
)

func parse(data []string) Network {
	// AAA = (BBB, CCC)
	var re = regexp.MustCompile( `([A-Z]+) = \(([A-Z]+), ([A-Z]+)\)` )
	var result Network
	result.nodes = make(map[NodeName]Node)
	result.navigation = data[0]
	for _, line := range data[2:] {
		m := re.FindStringSubmatch(line)
		node := Node{ name: NodeName(m[1]), left: NodeName(m[2]), right: NodeName(m[3]) }
		result.nodes[node.name] = node
	}
	return result
}

type Day08 struct{}

func (d Day08) Solve() aoc.Solution {
	var part1, part2 int
	data := aoc.ReadFile("08-1")
	n := parse(data)
	navIndex := 0
	navIndexLength := len(n.navigation)
	var curNode Node
	curNode = n.nodes[StartNode]
	for curNode.name != EndNode {
		navInstruction := n.navigation[navIndex]
		curNode = n.nodes[aoc.Ifelse(navInstruction == 'L', curNode.left, curNode.right)]
		navIndex = ( navIndex + 1 ) % navIndexLength
		part1++
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
