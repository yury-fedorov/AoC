package day19

import (
	"regexp"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
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

type WorkflowName string

const (
	In       WorkflowName = "in"
	Accepted WorkflowName = "A"
	Rejected WorkflowName = "R"
)

type Rule struct {
	varName string // "x" | "m" | "a" | "s"
	op      string // "<" | ">"
	value   int
	dest    WorkflowName
}

type WorkflowDef struct {
	rules []Rule
	final WorkflowName // destination when no rule matches
}

var rewf = regexp.MustCompile("(.+)\\{(.+)}")
var rewfs = regexp.MustCompile("([xmas])([^0-9]+)([0-9]+):(.+)")

func parseWorkflow(line string) (WorkflowName, WorkflowDef) {
	// pv{a>1716:R,A}
	match := rewf.FindStringSubmatch(line)
	name := WorkflowName(match[1])
	var def WorkflowDef
	for _, s := range strings.Split(match[2], ",") {
		m := rewfs.FindStringSubmatch(s)
		if m == nil {
			def.final = WorkflowName(s) // bare destination, e.g. "A"
			continue
		}
		def.rules = append(def.rules, Rule{
			varName: m[1],
			op:      m[2],
			value:   aoc.Atoi(m[3]),
			dest:    WorkflowName(m[4]),
		})
	}
	return name, def
}

// runWorkflow evaluates one Part against the workflow graph.
func runWorkflow(workflows map[WorkflowName]WorkflowDef, p Part) bool {
	name := In
	for {
		w := workflows[name]
		next := w.final
		for _, rule := range w.rules {
			v := getPart(p, rule.varName)
			if (rule.op == "<" && v < rule.value) || (rule.op == ">" && v > rule.value) {
				next = rule.dest
				break
			}
		}
		name = next
		if name == Accepted {
			return true
		}
		if name == Rejected {
			return false
		}
	}
}

// -- part 1 input parsing --

var rep = regexp.MustCompile("\\{(.+)}")

func parseValue(expr string) int {
	return aoc.Atoi(strings.Split(expr, "=")[1])
}

func parsePart(line string) Part {
	// {x=787,m=2655,a=1222,s=2876}
	match := rep.FindStringSubmatch(line)
	sets := strings.Split(match[1], ",")
	return Part{
		x: parseValue(sets[0]),
		m: parseValue(sets[1]),
		a: parseValue(sets[2]),
		s: parseValue(sets[3]),
	}
}

// -- part 2: recursive range splitting through the workflow graph --

type Range struct{ lo, hi int } // inclusive

func (r Range) size() int { return max(0, r.hi-r.lo+1) }

type Ranges struct {
	x, m, a, s Range
}

func rangeAt(rs Ranges, v string) Range {
	switch v {
	case "x":
		return rs.x
	case "m":
		return rs.m
	case "a":
		return rs.a
	default:
		return rs.s
	}
}

func withRange(rs Ranges, v string, r Range) Ranges {
	switch v {
	case "x":
		rs.x = r
	case "m":
		rs.m = r
	case "a":
		rs.a = r
	default:
		rs.s = r
	}
	return rs
}

// splitRange partitions r into the part satisfying (var op value) and the rest.
// Both halves are clamped to [r.lo, r.hi] so they are exact sub-partitions.
func splitRange(r Range, op string, value int) (matched, unmatched Range) {
	switch op {
	case "<":
		matched = Range{r.lo, min(r.hi, value-1)}
		unmatched = Range{max(r.lo, value), r.hi}
	case ">":
		matched = Range{max(r.lo, value+1), r.hi}
		unmatched = Range{r.lo, min(r.hi, value)}
	}
	return
}

func countAccepted(workflows map[WorkflowName]WorkflowDef, rs Ranges, name WorkflowName) int64 {
	if rs.x.size() == 0 || rs.m.size() == 0 || rs.a.size() == 0 || rs.s.size() == 0 {
		return 0
	}
	if name == Accepted {
		return int64(rs.x.size()) * int64(rs.m.size()) * int64(rs.a.size()) * int64(rs.s.size())
	}
	if name == Rejected {
		return 0
	}
	w := workflows[name]
	var total int64
	cur := rs
	for _, rule := range w.rules {
		matched, unmatched := splitRange(rangeAt(cur, rule.varName), rule.op, rule.value)
		if matched.size() > 0 {
			total += countAccepted(workflows, withRange(cur, rule.varName, matched), rule.dest)
		}
		if unmatched.size() == 0 {
			return total // entire current range was consumed by this rule
		}
		cur = withRange(cur, rule.varName, unmatched)
	}
	total += countAccepted(workflows, cur, w.final)
	return total
}

func (day Day19) Solve() aoc.Solution {
	var part1, part2 int
	data := aoc.ReadFile("19")
	workflows := make(map[WorkflowName]WorkflowDef)
	var parts []Part
	parsingWorkflows := true
	for _, line := range data {
		if len(line) == 0 {
			parsingWorkflows = false
			continue
		}
		if parsingWorkflows {
			name, def := parseWorkflow(line)
			workflows[name] = def
		} else {
			parts = append(parts, parsePart(line))
		}
	}
	for _, p := range parts {
		if runWorkflow(workflows, p) {
			part1 += p.x + p.m + p.a + p.s
		}
	}
	full := Ranges{
		x: Range{1, 4000}, m: Range{1, 4000}, a: Range{1, 4000}, s: Range{1, 4000},
	}
	part2 = int(countAccepted(workflows, full, In))
	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(part2)}
}
