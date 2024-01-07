package day20

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
)

type Day20 struct{}

const LowPulse = false
const HighPulse = true

// ie. button -low-> broadcaster
// ie. inv -high-> a
type Pulse struct {
	from, to  string
	pulseType bool
}

type ModuleProcessor interface {
	moduleName() string
	process(p Pulse) []Pulse
}

type AbstractModule struct {
	ModuleProcessor
	name string
	to   []string
}

func (am *AbstractModule) moduleName() string { return am.name }

const ButtonModuleName = "button"
const BroadcasterModuleName = "broadcaster"
const OutputModuleName = "output"

type Button struct {
	ModuleProcessor
}

func (b *Button) process(p Pulse) []Pulse {
	return []Pulse{{from: ButtonModuleName, to: BroadcasterModuleName, pulseType: LowPulse}}
}

type Broadcaster struct {
	AbstractModule
}

func (a *Broadcaster) process(p Pulse) []Pulse {
	var result []Pulse
	for _, d := range a.to {
		result = append(result, Pulse{from: a.name, to: d, pulseType: p.pulseType})
	}
	return result
}

// Flip-flop modules (prefix %)
type FlipFlop struct {
	AbstractModule
	stateOn bool // initially off = false
}

func (ff *FlipFlop) process(p Pulse) []Pulse {
	// If a flip-flop module receives a high pulse, it is ignored and nothing happens.
	// However, if a flip-flop module receives a low pulse, it flips between on and off.
	// If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.
	var result []Pulse
	if p.pulseType == LowPulse {
		ff.stateOn = !ff.stateOn
		for _, d := range ff.to {
			result = append(result, Pulse{from: ff.name, to: d, pulseType: ff.stateOn})
		}
	}
	return result
}

// Conjunction modules (prefix &)
type Conjunction struct {
	AbstractModule
	recentPulse map[string]bool
}

func (c *Conjunction) process(p Pulse) []Pulse {
	// When a pulse is received, the conjunction module first updates its memory for that input.
	c.recentPulse[p.from] = p.pulseType

	// Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse.
	allHigh := true
	for _, pulse := range c.recentPulse {
		allHigh = allHigh && (pulse == HighPulse)
	}

	var result []Pulse
	for _, d := range c.to {
		result = append(result, Pulse{from: c.name, to: d, pulseType: !allHigh})
	}
	return result
}

func parseModule(line string) *ModuleProcessor {
	a := strings.Split(line, " -> ")
	name := a[0]
	to := strings.Split(a[1], ", ")
	var mp ModuleProcessor
	if strings.HasPrefix(name, "&") {
		// conjunction
		mp = &Conjunction{AbstractModule{name: name[1:], to: to}, make(map[string]bool)}
	} else if strings.HasPrefix(name, "%") {
		// flip flop
		mp = &FlipFlop{AbstractModule{name: name[1:], to: to}, false} // they are initially off
	} else if name == BroadcasterModuleName {
		// broadcaster
		mp = &Broadcaster{AbstractModule{name: name, to: to}}
	} else {
		panic(line)
	}
	return &mp
}

func parse(file string) map[string]*ModuleProcessor {
	result := make(map[string]*ModuleProcessor)
	var button ModuleProcessor = &Button{}
	result[ButtonModuleName] = &button
	// TODO
	for _, line := range aoc.ReadFile(file) {
		mp := parseModule(line)
		result[(*mp).moduleName()] = mp
	}
	return result
}

func (day Day20) Solve() aoc.Solution {
	var part1, part2 int
	var output []Pulse
	m := parse("20")
	for i := 0; i < 1000; i++ {
		output = append(output, Pulse{ButtonModuleName, BroadcasterModuleName, false})
		for j := len(output) - 1; j < len(output); j++ {
			nextPulse := output[j]
			if nextPulse.to == OutputModuleName {
				continue
			}
			var processor, ok = m[nextPulse.to]
			if !ok {
				continue
			}
			pulses := (*processor).process(nextPulse)
			output = append(output, pulses...)
		}
	}
	var low, high int
	for _, p := range output {
		high += aoc.Ifelse(p.pulseType == HighPulse, 1, 0)
		low += aoc.Ifelse(p.pulseType == LowPulse, 1, 0)
	}
	// 857592831 - too high
	part1 = low * high
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
