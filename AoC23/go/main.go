package main

import (
	"fmt"

	aoc23 "github.com/yury-fedorov/AoC/AoC23/aoc23"
)

func main() {
	for i, d := range aoc23.Days() {
		s := d.Solve()
		fmt.Printf("%d %q %q", i, s.Part1, s.Part2)
	}
}
