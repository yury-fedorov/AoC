package aoc23

import (
	"testing"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"github.com/yury-fedorov/AoC/AoC23/day01"
	"github.com/yury-fedorov/AoC/AoC23/day02"
	"github.com/yury-fedorov/AoC/AoC23/day03"
	"github.com/yury-fedorov/AoC/AoC23/day04"
)

func TestDays(t *testing.T) {
	tests := []struct {
		day  aoc.Day
		want aoc.Solution
	}{
		{day: &day01.Day01{}, want: aoc.Solution{Part1: "54968", Part2: "54094"}},
		{day: &day02.Day02{}, want: aoc.Solution{Part1: "2283", Part2: "78669"}},
		{day: &day03.Day03{}, want: aoc.Solution{Part1: "540212", Part2: "0"}},
		{day: &day04.Day04{}, want: aoc.Solution{Part1: "0", Part2: "0"}},
	}
	for i, test := range tests {
		got := test.day.Solve()
		want := test.want
		if got != want {
			day := i + 1 // zero based index
			t.Fatalf(`Day %d returned %v, want %v`, day, got, want)
		}
	}
}
