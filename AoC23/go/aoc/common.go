package aoc

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
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

func ReadFile(tag string) []string {
	file, err := os.Open(fmt.Sprintf("../data/%v.txt", tag))
	if err != nil {
		file, err = os.Open(fmt.Sprintf("../../data/%v.txt", tag))
		if err != nil {
			panic(err)
		}
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

func Ifelse[T any](condition bool, ok T, ko T) T {
	if condition {
		return ok
	}
	return ko
}

func Abs[I int64 | int](x I) I {
	return Ifelse(x >= 0, x, -x)
}

func Pow[I int64 | int](x, y I) I {
	return I(math.Pow(float64(x), float64(y)))
}

func Atoi(number string) int {
	result, err := strconv.Atoi(number)
	if err != nil {
		panic(number)
	}
	return result
}

func ToSlice(text string) []int64 {
	var result []int64
	for _, textNumber := range strings.Split(text, " ") {
		result = append(result, int64(Atoi(textNumber)))
	}
	return result
}
