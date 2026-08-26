package aoc23

import (
	"os"
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
	"github.com/yury-fedorov/AoC/AoC23/day20"
	"github.com/yury-fedorov/AoC/AoC23/day21"
	"github.com/yury-fedorov/AoC/AoC23/day22"
	"github.com/yury-fedorov/AoC/AoC23/day23"
	"github.com/yury-fedorov/AoC/AoC23/day24"
	"github.com/yury-fedorov/AoC/AoC23/day25"
)

type testCase struct {
	name string
	day  aoc.Day
	want aoc.Solution
	slow bool
}

func (tc testCase) run(t *testing.T) {
	t.Run(tc.name, func(t *testing.T) {
		if tc.slow && os.Getenv("AOC_SLOW") == "" {
			t.Skip("skipping slow test (set AOC_SLOW=1 to run)")
		}
		got := tc.day.Solve()
		if got != tc.want {
			t.Errorf("%T returned %v, want %v", tc.day, got, tc.want)
		}
	})
}

var tests = []testCase{
	{name: "Day01", day: &day01.Day01{}, want: aoc.Solution{Part1: "54968", Part2: "54094"}},
	{name: "Day02", day: &day02.Day02{}, want: aoc.Solution{Part1: "2283", Part2: "78669"}},
	{name: "Day03", day: &day03.Day03{}, want: aoc.Solution{Part1: "540212", Part2: "87605697"}},
	{name: "Day04", day: &day04.Day04{}, want: aoc.Solution{Part1: "21821", Part2: "5539496"}},
	{name: "Day05", day: &day05.Day05{}, want: aoc.Solution{Part1: "174137457", Part2: "1493866"}},
	{name: "Day06", day: &day06.Day06{}, want: aoc.Solution{Part1: "608902", Part2: "46173809"}},
	{name: "Day07", day: &day07.Day07{}, want: aoc.Solution{Part1: "253205868", Part2: "253907829"}},
	{name: "Day08", day: &day08.Day08{}, want: aoc.Solution{Part1: "21251", Part2: "11678319315857"}},
	{name: "Day09", day: &day09.Day09{}, want: aoc.Solution{Part1: "1834108701", Part2: "993"}},
	{name: "Day10", day: &day10.Day10{}, want: aoc.Solution{Part1: "6613", Part2: "511"}},
	{name: "Day11", day: &day11.Day11{}, want: aoc.Solution{Part1: "9681886", Part2: "791134099634"}},
	{name: "Day12", day: &day12.Day12{}, want: aoc.Solution{Part1: "7221", Part2: "7139671893722"}},
	{name: "Day13", day: &day13.Day13{}, want: aoc.Solution{Part1: "42974", Part2: "27587"}},
	{name: "Day14", day: &day14.Day14{}, want: aoc.Solution{Part1: "109755", Part2: "90928"}},
	{name: "Day15", day: &day15.Day15{}, want: aoc.Solution{Part1: "512283", Part2: "215827"}},
	{name: "Day16", day: &day16.Day16{}, want: aoc.Solution{Part1: "6978", Part2: "7315"}},
	{name: "Day17", day: &day17.Day17{}, want: aoc.Solution{Part1: "1256", Part2: "1382"}},
	{name: "Day18", day: &day18.Day18{}, want: aoc.Solution{Part1: "40745", Part2: "90111113594927"}},
	{name: "Day19", day: &day19.Day19{}, want: aoc.Solution{Part1: "399284", Part2: "121964982771486"}},
	{name: "Day20", day: &day20.Day20{}, want: aoc.Solution{Part1: "832957356", Part2: "240162699605221"}},
	{name: "Day21", day: &day21.Day21{}, want: aoc.Solution{Part1: "3699", Part2: "613391294577878"}, slow: true}, // takes 16 seconds
	{name: "Day22", day: &day22.Day22{}, want: aoc.Solution{Part1: "398", Part2: "70727"}, slow: true},            // part 2 takes 18.5 seconds
	{name: "Day23", day: &day23.Day23{}, want: aoc.Solution{Part1: "2430", Part2: "6534"}},
	{name: "Day24", day: &day24.Day24{}, want: aoc.Solution{Part1: "13910", Part2: "618534564836937"}},
	{name: "Day25", day: &day25.Day25{}, want: aoc.Solution{Part1: "601310", Part2: "0"}, slow: true}, // takes variable time (randomness)
}

func TestDays(t *testing.T) {
	for _, tc := range tests {
		tc.run(t)
	}
}
