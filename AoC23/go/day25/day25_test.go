package day25

import "testing"

func TestAllDirectLinks(t *testing.T) {
	input := parse("25-1")
	hfxLinks := allDirectLinks(input, "hfx")
	want := 5
	got := len(hfxLinks)
	if want != got {
		t.Errorf("allDirectLinks(hfx), got: %v, want: %v.", got, want)
	}
}

func TestSample(t *testing.T) {
	input := parse("25-1")
	got := answer1(input)
	want := 54
	if want != got {
		t.Errorf("answer1(sample), got: %v, want: %v.", got, want)
	}
}

func TestAnswer1(t *testing.T) {
	input := parse("25")
	got := answer1(input)
	want := -1
	if want != got {
		t.Errorf("answer1(real), got: %v, want: %v.", got, want)
	}
}
