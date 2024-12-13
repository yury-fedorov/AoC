package day12

import (
	"fmt"
	"testing"
)

const file = "12-1"     // lines: 1000 max chr in line: 20
const real_file = "12"  // real input data
const fastest_mode = 1  // for the first case
const fastest_mode2 = 2 // for the second case

func TestModes(t *testing.T) {
	records := parse(file)

	var tests = []struct {
		name string
		mode int
	}{
		// the table itself
		{"mode 1", 1},
		{"mode 2", 2},
		{"mode 3", 3},
		{"mode 4", 4},
	}

	// The execution loop
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			for _, r := range records {
				// answer 1
				want := countPossibleArrangementsI(r.record, r.damagedGroups, fastest_mode)
				got := countPossibleArrangementsI(r.record, r.damagedGroups, tt.mode)
				if want != got {
					t.Errorf("Result was incorrect for record %s %v, got: %v, want: %v.",
						toPatternString(r.record), r.damagedGroups, got, want)
				}

				// answer 2
				want2 := countPossibleArrangements2(r.record, r.damagedGroups, fastest_mode)
				got2 := countPossibleArrangements2(r.record, r.damagedGroups, tt.mode)
				if want2 != got2 {
					t.Errorf("Result 2 was incorrect for record %s %v, got: %v, want: %v.",
						toPatternString(r.record), r.damagedGroups, got2, want2)
				}

			}
		})
	}

}

func TestAnswer1(t *testing.T) {
	records := parse(real_file) // lines: 1000 max chr in line: 20
	got := 0
	for _, r := range records {
		got += countPossibleArrangementsI(r.record, r.damagedGroups, fastest_mode)
	}
	const want = 7221
	if want != got {
		t.Errorf("Day 12 answer 1, got: %v, want: %v.", got, want)
	}
}

func TestAnswer2(t *testing.T) {
	t.Skip("skipping test as it is eternal.")
	records := parse(real_file) // lines: 1000 max chr in line: 20
	got := 0
	for i, r := range records {
		fmt.Printf("%d %v %v", i, toPatternString(r.record), r.damagedGroups)
		got += countPossibleArrangements2(r.record, r.damagedGroups, fastest_mode2)
	}
	const want = -1
	if want != got {
		t.Errorf("Day 12 answer 2, got: %v, want: %v.", got, want)
	}
}

// 163 ns
func BenchmarkV1(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangementsI(r.record, r.damagedGroups, 1)
		}
	}
}

// 171.5 ns
func BenchmarkV2(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangementsI(r.record, r.damagedGroups, 2)
		}
	}
}

// 61221 ns
func BenchmarkV3(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangementsI(r.record, r.damagedGroups, 3)
		}
	}
}

// 216.9 ns
func BenchmarkV4(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangementsI(r.record, r.damagedGroups, 4)
		}
	}
}

// 5272 ns
func BenchmarkV21(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangements2(r.record, r.damagedGroups, 1)
		}
	}
}

// 5110 ns
func BenchmarkV22(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangements2(r.record, r.damagedGroups, 2)
		}
	}
}

// 217732 ns
func BenchmarkV23(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangements2(r.record, r.damagedGroups, 3)
		}
	}
}

// 5366
func BenchmarkV24(b *testing.B) {
	records := parse(file)
	for i := 0; i < b.N; i++ {
		for _, r := range records {
			countPossibleArrangements2(r.record, r.damagedGroups, 4)
		}
	}
}
