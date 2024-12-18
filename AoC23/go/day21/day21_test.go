package day21

import (
	"testing"
)

func TestAnswer1(t *testing.T) {
	d := Day21{}
	got := d.Solve().Part2
	const want = ""
	if want != got {
		// TODO - no implementation, no answer
		t.Errorf("Day 24 answer 1, got: %v, want: %v.", got, want)
	}
}
