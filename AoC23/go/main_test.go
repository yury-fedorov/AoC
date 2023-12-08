package aoc23

import (
	"testing"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"github.com/yury-fedorov/AoC/AoC23/day01"
	"github.com/yury-fedorov/AoC/AoC23/day02"
	"github.com/yury-fedorov/AoC/AoC23/day03"
	"github.com/yury-fedorov/AoC/AoC23/day04"
	"github.com/yury-fedorov/AoC/AoC23/day05"
	"github.com/yury-fedorov/AoC/AoC23/day06"
	"github.com/yury-fedorov/AoC/AoC23/day07"
)

const runSlowTests = false

type Test struct {
	day  aoc.Day
	want aoc.Solution
}

func TestDays(t *testing.T) {
	fastTests := []Test{
		{day: &day01.Day01{}, want: aoc.Solution{Part1: "54968", Part2: "54094"}},
		{day: &day02.Day02{}, want: aoc.Solution{Part1: "2283", Part2: "78669"}},
		{day: &day03.Day03{}, want: aoc.Solution{Part1: "540212", Part2: "87605697"}},
		{day: &day04.Day04{}, want: aoc.Solution{Part1: "21821", Part2: "5539496"}},
		{day: &day06.Day06{}, want: aoc.Solution{Part1: "608902", Part2: "46173809"}},
		{day: &day07.Day07{}, want: aoc.Solution{Part1: "253205868", Part2: "253907829"}},
	}
	slowTests := []Test{
		// takes 8 minutes
		{day: &day05.Day05{}, want: aoc.Solution{Part1: "174137457", Part2: "1493866"}},
	}
	tests := fastTests
	if runSlowTests {
		tests = append(tests, slowTests...)
	}
	for _, test := range tests {
		got := test.day.Solve()
		want := test.want
		if got != want {
			t.Fatalf(`%T returned %v, want %v`, test.day, got, want)
		}
	}
}
