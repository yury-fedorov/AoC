package day19

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

type Day19 struct{}

type Part struct {
	x, m, a, s int
}

func getPart(p Part, t string) int {
	switch t {
	case "x":
		return p.x
	case "m":
		return p.m
	case "a":
		return p.a
	case "s":
		return p.s
	}
	panic(t)
}

func toFunc(op string) func(a, b int) bool {
	switch op {
	case ">":
		return func(a, b int) bool { return a > b }
	case "<":
		return func(a, b int) bool { return a < b }
	}
	panic(op)
}

type WorkflowName string

const In WorkflowName = "in"
const Next WorkflowName = ""
const Accepted WorkflowName = "A"
const Rejected WorkflowName = "R"

type WorkflowStep func(p Part) WorkflowName

type Workflow []WorkflowStep

var rewf = regexp.MustCompile("(.+)\\{(.+)\\}")
var rewfs = regexp.MustCompile("([xmas])([^0-9]+)([0-9]+):(.+)")

func parseWorkflow(line string, wstats *WorkflowStats) (WorkflowName, Workflow) {
	// pv{a>1716:R,A}
	match := rewf.FindStringSubmatch(line)
	steps := strings.Split(match[2], ",")
	var w Workflow
	for _, s := range steps {
		var ws WorkflowStep
		// a>1716:R
		match1 := rewfs.FindStringSubmatch(s)
		if match1 == nil {
			ws = func(Part) WorkflowName { return WorkflowName(s) }
		} else {
			v := match1[1]
			op := match1[2]
			var co = aoc.Atoi(match1[3])
			dist := WorkflowName(match1[4])
			ws = func(p Part) WorkflowName {
				vv := getPart(p, v)
				f := toFunc(op)
				return aoc.Ifelse(f(vv, co), dist, Next)
			}
			(*wstats)[v] = append((*wstats)[v], co)
		}
		w = append(w, ws)
	}
	return WorkflowName(match[1]), w
}

var rep = regexp.MustCompile("\\{(.+)\\}")

func parseValue(expr string) int {
	return aoc.Atoi(strings.Split(expr, "=")[1])
}

func parsePart(line string) Part {
	// {x=787,m=2655,a=1222,s=2876}
	match := rep.FindStringSubmatch(line)
	sets := strings.Split(match[1], ",")
	return Part{x: parseValue(sets[0]), m: parseValue(sets[1]), a: parseValue(sets[2]), s: parseValue(sets[3])}
}

func runWorkflow(workflows map[WorkflowName]Workflow, p Part) bool {
	ws := workflows[In]
	for {
		for _, wsi := range ws {
			wfn := wsi(p)
			if wfn == Next {
				continue
			}
			if wfn == Accepted {
				return true
			}
			if wfn == Rejected {
				return false
			}
			ws = workflows[wfn]
			break
		}
	}
}

// -- part 2 --

// WorkflowStats contains border lines mentioned in workflows conditions.
type WorkflowStats map[string][]int

func uniqueOrdered(s []int) []int {
	r := make(map[int]bool)
	for _, v := range s {
		r[v] = true
	}
	var result = maps.Keys(r)
	sort.Ints(result)
	return result
}

const kMin = 1
const kMax = 4000

type Range struct {
	a, b int
}

func pointsToRanges(pp []int) []Range {
	// kMin, kMax, all points in between -1, 0, 1
	var result []Range
	a := kMin
	for _, p := range pp {
		result = append(result, Range{a: a, b: p - 1})
		result = append(result, Range{a: p, b: p})
		a = p + 1
	}
	result = append(result, Range{a: a, b: kMax})
	return result
}

func rangeSize(r Range) int { return r.b - r.a + 1 }

// Global variables to make it faster.
var xr, mr, ar, sr []Range
var isAccepted func(Part) bool

func countAccepted(xi, mi, ai int) int {
	var result int
	if xi < 0 {
		// go through x ranges
		for xi = 0; xi < len(xr); xi++ {
			result += countAccepted(xi, mi, ai)
		}
	} else if mi < 0 {
		// go through m ranges
		for mi = 0; mi < len(mr); mi++ {
			result += countAccepted(xi, mi, ai)
		}
	} else if ai < 0 {
		// go through a ranges
		for ai = 0; ai < len(ar); ai++ {
			result += countAccepted(xi, mi, ai)
		}
	} else {
		// go through s ranges
		x := xr[xi]
		m := mr[mi]
		a := ar[ai]
		k := rangeSize(x) * rangeSize(m) * rangeSize(a)
		for _, srv := range sr {
			if isAccepted(Part{x: x.a, m: m.a, a: a.a, s: srv.a}) {
				result += k * rangeSize(srv)
			}
		}
	}
	return result
}

func (day Day19) Solve() aoc.Solution {
	var part1, part2 int
	data := aoc.ReadFile("19")
	workflows := make(map[WorkflowName]Workflow)
	var parts []Part
	parsingWorkflows := true

	var wstats = WorkflowStats{
		"x": {},
		"m": {},
		"a": {},
		"s": {},
	}
	for _, line := range data {
		if len(line) == 0 {
			parsingWorkflows = false
			continue
		}
		if parsingWorkflows {
			workflowName, workflow := parseWorkflow(line, &wstats)
			workflows[workflowName] = workflow
		} else {
			parts = append(parts, parsePart(line))
		}
	}
	for _, p := range parts {
		isAccepted := runWorkflow(workflows, p)
		if isAccepted {
			part1 += p.x + p.m + p.a + p.s
		}
	}

	// part 2
	// Global variables initialization.
	xr = pointsToRanges(uniqueOrdered(wstats["x"]))
	mr = pointsToRanges(uniqueOrdered(wstats["m"]))
	ar = pointsToRanges(uniqueOrdered(wstats["a"]))
	sr = pointsToRanges(uniqueOrdered(wstats["s"]))
	isAccepted = func(p Part) bool { return runWorkflow(workflows, p) }
	part2 = countAccepted(-1, -1, -1)

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
