package day20

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
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
	process(p Pulse) []Pulse
}

type AbstractModule struct {
	name string
	to   []string
	ModuleProcessor
}

const ButtonModuleName = "button"
const BroadcasterModuleName = "broadcaster"

type Button struct {
	ModuleProcessor
}

func (b *Button) process(p Pulse) []Pulse {
	return []Pulse{{from: ButtonModuleName, to: BroadcasterModuleName, pulseType: LowPulse}}
}

type Broadcaster struct {
	module *AbstractModule
}

func (a *Broadcaster) process(p Pulse) []Pulse {
	var result []Pulse
	for _, d := range a.module.to {
		result = append(result, Pulse{from: a.module.name, to: d, pulseType: p.pulseType})
	}
	return result
}

// Flip-flop modules (prefix %)
type FlipFlop struct {
	module  *AbstractModule
	stateOn bool // initially off = false
}

func (ff *FlipFlop) process(p Pulse) []Pulse {
	// If a flip-flop module receives a high pulse, it is ignored and nothing happens.
	// However, if a flip-flop module receives a low pulse, it flips between on and off.
	// If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.
	var result []Pulse
	if p.pulseType == LowPulse {
		ff.stateOn = !ff.stateOn
		for _, d := range ff.module.to {
			result = append(result, Pulse{from: ff.module.name, to: d, pulseType: ff.stateOn})
		}
	}
	return result
}

// Conjunction modules (prefix &)
type Conjunction struct {
	module      *AbstractModule
	recentPulse map[string]bool
}

func (c *Conjunction) process(p Pulse) []Pulse {
	// When a pulse is received, the conjunction module first updates its memory for that input.
	c.recentPulse[p.from] = p.pulseType

	// Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse.
	outPulse := aoc.Ifelse(len(c.recentPulse) > 0, true, false)
	for _, pulse := range c.recentPulse {
		outPulse = outPulse && (pulse == HighPulse)
	}

	var result []Pulse
	for _, d := range c.module.to {
		result = append(result, Pulse{from: c.module.name, to: d, pulseType: outPulse})
	}
	return result
}

func parse(file string) map[string]*ModuleProcessor {
	// TODO
	// for _, line := range aoc.ReadFile("25") {
	// }
	result := make(map[string]*ModuleProcessor)
	var button ModuleProcessor = &Button{}
	result[ButtonModuleName] = &button
	return result
}

func (day Day20) Solve() aoc.Solution {
	var part1, part2 int
	var output []Pulse
	m := parse("20-1")
	for i := 0; i < 1000; i++ {
		var button = m[ButtonModuleName]
		pulses := (*button).process(Pulse{})
		output = append(output, pulses...)
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
