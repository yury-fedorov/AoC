package day03

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"unicode"
)

type Day03 struct{}

const empty rune = '.'

func at(engine []string, x, y int) rune {
	if x < 0 || y < 0 || x >= len(engine) || y >= len(engine[0]) {
		return empty
	}
	line := []rune(engine[y])
	return line[x]
}

func isPartialSymbol(ch rune) bool {
	return ch != empty && !unicode.IsDigit(ch)
}

func isPartialNumber(engine []string, startX, endX, y int) bool {
	for dy := -1; dy <= 1; dy++ {
		for xi := startX - 1; xi <= endX; xi++ {
			ch := at(engine, xi, y+dy)
			if isPartialSymbol(ch) {
				return true
			}
		}
	}
	return false
}

type partNumber struct {
	number, x0, x1, y int
}

func identifyPartNumbers(engine []string) []partNumber {
	maxY := len(engine)
	maxX := len(engine[0])
	var result []partNumber
	for y := 0; y < maxY; y++ {
		const NotStarted int = -1
		var startAt = NotStarted
		line := []rune(engine[y])
		for x := 0; x <= maxX; x++ {
			chi := at(engine, x, y)
			isDigit := unicode.IsDigit(chi)
			if startAt == NotStarted && isDigit {
				startAt = x
			} else if startAt != NotStarted && !isDigit {
				// x - is the index next to the end of the number
				// so the number is around: [startAt, x)
				if isPartialNumber(engine, startAt, x, y) {
					partialNumberText := string(line[startAt:x])
					partialNumber, err := strconv.Atoi(partialNumberText)
					if err != nil {
						panic(partialNumber)
					}
					result = append(result, partNumber{
						number: partialNumber,
						x0:     startAt,
						x1:     x - 1,
						y:      y,
					})
				}
				startAt = NotStarted
			}

		}
	}
	return result
}

func sumPartNumbers(list []partNumber) int {
	var result int
	for _, pn := range list {
		result += pn.number
	}
	return result
}

func abs(x int) int {
	if x >= 0 {
		return x
	}
	return -x
}

func inRange(x, min, max int) bool {
	return x >= min && x <= max
}

func isAdjacent(n partNumber, x, y int) bool {
	return abs(n.y-y) <= 1 && inRange(x, n.x0-1, n.x1+1)
}

func sumGearRatios(engine []string, numberList []partNumber) int {
	var result int
	const Star rune = '*'
	maxY := len(engine)
	maxX := len(engine[0])
	for y := 0; y < maxY; y++ {
		for x := 0; x < maxX; x++ {
			chi := at(engine, x, y)
			if chi == Star {
				// A gear is any * symbol that is adjacent to exactly two part numbers.
				var found []partNumber
				for _, pni := range numberList {
					if isAdjacent(pni, x, y) {
						found = append(found, pni)
					}
				}
				if len(found) == 2 {
					result += found[0].number * found[1].number
				}
			}
		}
	}
	return result
}

func (d Day03) Solve() aoc.Solution {
	var part1, part2 int
	engine := aoc.ReadFile("03")
	partNumberList := identifyPartNumbers(engine)
	part1 = sumPartNumbers(partNumberList)
	part2 = sumGearRatios(engine, partNumberList)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
