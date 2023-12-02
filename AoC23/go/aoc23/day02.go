package aoc23

import (
	"regexp"
	"strconv"
	"strings"
)

type Day02 struct{}

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
func (d Day02) parseGame(line string) game {
	match := r1.FindStringSubmatch(line)
	id, err := strconv.Atoi(match[1])
	if err != nil {
		panic(err)
	}
	var extractions []extraction
	gamesStr := strings.Split(match[2], ";")
	for _, gameStr := range gamesStr {
		e := make(extraction)
		es := strings.Split(gameStr, ",")
		for _, esi := range es {
			match = r2.FindStringSubmatch(esi)
			qty, err := strconv.Atoi(match[1])
			if err != nil {
				panic(err)
			}
			e[color(match[2])] = qty
		}
		extractions = append(extractions, e)
	}
	return game{id: id, extractions: extractions}
}

func (d Day02) rgb(r int, g int, b int) extraction {
	return extraction{red: r, green: g, blue: b}
}

func (d Day02) match(requirement, test extraction) bool {
	for _, c := range colors {
		maxQty := requirement[c]
		testQty := test[c]
		if testQty > maxQty {
			return false
		}
	}
	return true
}

func (d Day02) matchGame(requirement extraction, g game) bool {
	for _, e := range g.extractions {
		if !d.match(requirement, e) {
			return false
		}
	}
	return true
}

func (d Day02) power(set extraction) int {
	return set[red] * set[green] * set[blue]
}

func (d Day02) set(game game) extraction {
	var r, g, b int
	for _, e := range game.extractions {
		r = max(r, e[red])
		g = max(g, e[green])
		b = max(b, e[blue])
	}
	return d.rgb(r, g, b)
}

func (d Day02) Solve() Solution {
	requirement := d.rgb(12, 13, 14)
	var part1, part2 int
	for _, line := range ReadFile("02") {
		g := d.parseGame(line)
		if d.matchGame(requirement, g) {
			part1 += g.id
		}
		part2 += d.power(d.set(g))
	}
	return Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
