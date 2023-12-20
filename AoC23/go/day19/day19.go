package day19

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"regexp"
	"strconv"
)

type Day19 struct{}

type Part struct {
	x, m, a, s int
}

type WorkflowName string

const In WorkflowName = "in"
const Next WorkflowName = ""
const Accepted WorkflowName = "A"
const Rejected WorkflowName = "R"

type WorkflowStep func(p Part) WorkflowName

type Workflow []WorkflowStep

var rewf = regexp.MustCompile("(.+)\\{(.+)\\}")

func parseWorkflow(line string) (WorkflowName, Workflow) {
	// pv{a>1716:R,A}
	// TODO
	return "", Workflow{}
}

var rep = regexp.MustCompile("\\{(.+)\\}")

func parsePart(line string) Part {
	// {x=787,m=2655,a=1222,s=2876}
	// TODO
	return Part{}
}

func (day Day19) Solve() aoc.Solution {
	var part1, part2 int
	data := aoc.ReadFile("19-1")
	workflows := make(map[WorkflowName]Workflow)
	var parts []Part
	parsingWorkflows := true
	for _, line := range data {
		if len(line) == 0 {
			parsingWorkflows = false
		}
		if parsingWorkflows {
			workflowName, workflow := parseWorkflow(line)
			workflows[workflowName] = workflow
		} else {
			parts = append(parts, parsePart(line))
		}
	}
	var acceptedParts []Part
	for _, p := range parts {
		ws := workflows[In]
		for _, wsi := range ws {
			wfn := wsi(p)
			if wfn == Accepted {
				acceptedParts = append(acceptedParts, p)
				break
			}
			if wfn == Rejected {
				break
			}
			if wfn == Next {
				continue
			}
			ws = workflows[wfn]
		}
	}
	for _, ap := range acceptedParts {
		part1 += ap.x + ap.m + ap.a + ap.s
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
