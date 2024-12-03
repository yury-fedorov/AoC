package day02

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"regexp"
	"strconv"
	"strings"
)

type color string

const (
	red   color = "red"
	blue  color = "blue"
	green color = "green"
)

var colors = []color{red, blue, green}

type extraction map[color]int

type game struct {
	id          int
	extractions []extraction
}

var r1 = regexp.MustCompile(`Game (\d+): (.+)`)
var r2 = regexp.MustCompile(`(\d+) ([a-z]+)`)

// Game 6: 3 blue, 10 green, 2 red; 5 green; 6 blue, 3 red
func parseGame(line string) game {
	match := r1.FindStringSubmatch(line)
	id := aoc.Atoi(match[1])
	var extractions []extraction
	gamesStr := strings.Split(match[2], ";")
	for _, gameStr := range gamesStr {
		e := make(extraction)
		es := strings.Split(gameStr, ",")
		for _, esi := range es {
			match = r2.FindStringSubmatch(esi)
			e[color(match[2])] = aoc.Atoi(match[1])
		}
		extractions = append(extractions, e)
	}
	return game{id: id, extractions: extractions}
}

func rgb(r int, g int, b int) extraction {
	return extraction{red: r, green: g, blue: b}
}

func match(requirement, test extraction) bool {
	for _, c := range colors {
		maxQty := requirement[c]
		testQty := test[c]
		if testQty > maxQty {
			return false
		}
	}
	return true
}

func matchGame(requirement extraction, g game) bool {
	for _, e := range g.extractions {
		if !match(requirement, e) {
			return false
		}
	}
	return true
}

func power(set extraction) int {
	return set[red] * set[green] * set[blue]
}

func set(game game) extraction {
	var r, g, b int
	for _, e := range game.extractions {
		r = aoc.Max(r, e[red])
		g = aoc.Max(g, e[green])
		b = aoc.Max(b, e[blue])
	}
	return rgb(r, g, b)
}

func Solve(lines []string) (int, int) {
	requirement := rgb(12, 13, 14)
	var part1, part2 int
	for _, line := range lines {
		g := parseGame(line)
		if matchGame(requirement, g) {
			part1 += g.id
		}
		part2 += power(set(g))
	}
	return part1, part2
}

type Day02 struct{}

func (d Day02) Solve() aoc.Solution {
	lines := aoc.ReadFile("02")
	part1, part2 := Solve(lines)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
