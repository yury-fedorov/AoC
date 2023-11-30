package aoc23

type Solution struct {
	Part1 string
	Part2 string
}

type Day interface {
	Solve() Solution
}

func Days() []Day {
	var d01 Day01
	var d02 Day02
	return []Day{&d01, &d02}
}
