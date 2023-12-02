package main

import (
	"testing"

	aoc23 "github.com/yury-fedorov/AoC/AoC23/aoc23"
)

func TestDays(t *testing.T) {
	solutions := []aoc23.Solution{
		{Part1: "54968", Part2: "54094"},
		{Part1: "2283", Part2: "78669"},
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
