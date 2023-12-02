package aoc23

import (
	"testing"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"github.com/yury-fedorov/AoC/AoC23/day01"
	"github.com/yury-fedorov/AoC/AoC23/day02"
	"github.com/yury-fedorov/AoC/AoC23/day03"
)

func Days() []aoc.Day {
	return []aoc.Day{&day01.Day01{}, &day02.Day02{}, &day03.Day03{}}
}

func TestDays(t *testing.T) {
	solutions := []aoc.Solution{
		{Part1: "54968", Part2: "54094"},
		{Part1: "2283", Part2: "78669"},
		{Part1: "0", Part2: "0"},
	}
	for i, d := range Days() {
		got := d.Solve()
		want := solutions[i]
		if got != want {
			day := i + 1 // zero based index
			t.Fatalf(`Day %d returned %v, want %v`, day, got, want)
		}
	}
}
