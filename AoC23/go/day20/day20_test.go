package day20

import (
	"testing"
)

func TestAnswer1(t *testing.T) {
	d := Day20{}
	got := d.Solve().Part2
	const want = ""
	if want != got {
		// TODO - too slow, no answer
		t.Errorf("Day 20 answer 1, got: %v, want: %v.", got, want)
	}
}
