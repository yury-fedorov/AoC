package main

import (
	"testing"

	aoc23 "github.com/yury-fedorov/AoC/AoC23/aoc23"
)

func TestDays(t *testing.T) {
	solutions := []aoc23.Solution{
		{Part1: "", Part2: ""},
		{Part1: "", Part2: ""},
	}
	for i, d := range aoc23.Days() {
		got := d.Solve()
		want := solutions[i]
		if got != want {
			day := i + 1 // zero based index
			t.Fatalf(`Day %d returned %v, want %v`, day, got, want)
		}
	}
}
