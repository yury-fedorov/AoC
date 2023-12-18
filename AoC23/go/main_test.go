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
	"github.com/yury-fedorov/AoC/AoC23/day08"
	"github.com/yury-fedorov/AoC/AoC23/day09"
	"github.com/yury-fedorov/AoC/AoC23/day10"
	"github.com/yury-fedorov/AoC/AoC23/day11"
	"github.com/yury-fedorov/AoC/AoC23/day12"
	"github.com/yury-fedorov/AoC/AoC23/day13"
	"github.com/yury-fedorov/AoC/AoC23/day14"
	"github.com/yury-fedorov/AoC/AoC23/day15"
	"github.com/yury-fedorov/AoC/AoC23/day16"
	"github.com/yury-fedorov/AoC/AoC23/day17"
	"github.com/yury-fedorov/AoC/AoC23/day18"
	"github.com/yury-fedorov/AoC/AoC23/day19"
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
		{day: &day08.Day08{}, want: aoc.Solution{Part1: "21251", Part2: "11678319315857"}},
		{day: &day09.Day09{}, want: aoc.Solution{Part1: "1834108701", Part2: "993"}},
		{day: &day11.Day11{}, want: aoc.Solution{Part1: "9681886", Part2: "791134099634"}},
		{day: &day12.Day12{}, want: aoc.Solution{Part1: "7221", Part2: "0"}}, // TODO - part 2
		{day: &day13.Day13{}, want: aoc.Solution{Part1: "42974", Part2: "27587"}},
		{day: &day15.Day15{}, want: aoc.Solution{Part1: "512283", Part2: "215827"}},
		{day: &day16.Day16{}, want: aoc.Solution{Part1: "6978", Part2: "7315"}},
		{day: &day18.Day18{}, want: aoc.Solution{Part1: "0", Part2: "0"}},
		{day: &day19.Day19{}, want: aoc.Solution{Part1: "0", Part2: "0"}},
	}
	// Slow tests and TODO zone.
	slowTests := []Test{
		// takes 8 minutes
		{day: &day05.Day05{}, want: aoc.Solution{Part1: "174137457", Part2: "1493866"}},
		{day: &day14.Day14{}, want: aoc.Solution{Part1: "109755", Part2: "90928"}}, // takes 4-5 secs

		{day: &day10.Day10{}, want: aoc.Solution{Part1: "6613", Part2: "0"}}, // TODO - part 2
		// takes 5 secs, TODO part 1, 2
		{day: &day17.Day17{}, want: aoc.Solution{Part1: "0", Part2: "0"}},
	}
	tests := fastTests
	if runSlowTests {
		tests = append(tests, slowTests...)
	}
	for _, test := range tests {
		got := test.day.Solve()
		want := test.want
		if got != want {
			t.Errorf(`%T returned %v, want %v`, test.day, got, want)
		}
	}
}
