package day20

import (
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
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

var input []string

func initConjuctionMap(name string) map[string]bool {
	var result = make(map[string]bool)
	for _, line := range input {
		a := strings.Split(line, " -> ")
		if strings.Contains(a[1], name) {
			name := a[0]
			if strings.HasPrefix(name, "%") || strings.HasPrefix(name, "&") {
				name = name[1:]
			}
			result[name] = false
		}
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
		n := name[1:]
		mp = &Conjunction{AbstractModule{name: n, to: to}, initConjuctionMap(n)}
	} else if strings.HasPrefix(name, "%") {
		// flip-flop
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
	input = aoc.ReadFile(file)
	for _, line := range input {
		mp := parseModule(line)
		result[(*mp).moduleName()] = mp
	}
	return result
}

func (day Day20) Solve() aoc.Solution {
	var part1, part2 int
	var output []Pulse

	// Part 1: replicate existing behavior (1000 button presses without resetting between presses)
	m := parse("20")
	for range 1000 {
		// press button once, wait until all pulses handled
		output = append(output, Pulse{ButtonModuleName, BroadcasterModuleName, LowPulse})
		for j := len(output) - 1; j < len(output); j++ {
			nextPulse := output[j]
			var processor, isTypedModule = m[nextPulse.to]
			if !isTypedModule {
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
	part1 = low * high

	// Part 2:
	// Reset all modules to their default states, then press the button repeatedly,
	// waiting for pulses to settle after each press, until a single low pulse is sent to "rx".
	// Modules retain their state between presses (flip-flops/conjunction memories).
	m2 := parse("20") // fresh modules/state
	const MaxPresses = 1000000
	found := false
	for presses := 1; presses <= MaxPresses && !found; presses++ {
		queue := []Pulse{{ButtonModuleName, BroadcasterModuleName, LowPulse}}
		for i := 0; i < len(queue) && !found; i++ {
			p := queue[i]
			// If the pulse is targeted to rx directly, check it first
			if p.to == "rx" && p.pulseType == LowPulse {
				part2 = presses
				found = true
				break
			}
			processor, isTypedModule := m2[p.to]
			if !isTypedModule {
				// pulse goes to an output that's not a module (like rx), already checked above,
				// so nothing to do.
				continue
			}
			pulses := (*processor).process(p)
			for _, np := range pulses {
				if np.to == "rx" && np.pulseType == LowPulse {
					part2 = presses
					found = true
					break
				}
				queue = append(queue, np)
			}
		}
	}
	// If not found within MaxPresses, part2 remains 0.
	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(part2)}
}
