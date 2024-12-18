package day17

import (
	"testing"
)

func TestAnswer1(t *testing.T) {
	d := Day17{}
	got := d.Solve().Part1
	const want = ""
	if want != got {
		// 1254 - too low
		// 1281 ... 1285 - too high
		// TODO - still wrong answer
		t.Errorf("Day 17 answer 1, got: %v, want: %v.", got, want)
	}
}
