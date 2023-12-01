package aoc23

import (
	"bufio"
	"fmt"
	"os"
)

type Part int

const (
	Part1 Part = 1
	Part2 Part = 2
)

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

func ReadFile(tag string) []string {
	file, err := os.Open(fmt.Sprintf("../data/%v.txt", tag))
	if err != nil {
		panic(err)
	}
	defer file.Close()
	fileScanner := bufio.NewScanner(file)
	fileScanner.Split(bufio.ScanLines)
	var result []string
	for fileScanner.Scan() {
		line := fileScanner.Text()
		result = append(result, line)
	}
	return result
}
