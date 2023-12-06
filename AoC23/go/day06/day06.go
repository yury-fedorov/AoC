package day06

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"math"
	"strconv"
	"strings"
)

type Day06 struct{}

type RaceResult struct {
	time     int
	distance int
}

func toSlice(text string) []int {
	found := strings.Split(strings.Split(text, ":")[1], " ")
	var result []int
	for _, num := range found {
		number, err := strconv.Atoi(num)
		if err != nil {
			continue
		}
		result = append(result, number)
	}
	return result
}

func parse(data []string) []RaceResult {
	times := toSlice(data[0])
	distances := toSlice(data[1])
	var result []RaceResult
	for i := 0; i < len(times); i++ {
		race := RaceResult{time: times[i], distance: distances[i]}
		result = append(result, race)
	}
	return result
}

func distance(chargeTime int, totalTime int) int {
	speed := chargeTime
	return speed * (totalTime - chargeTime)
}

func winningRange(record RaceResult) (int, int) {
	var ctMin = math.MaxInt
	var ctMax = math.MinInt
	for ct := 1; ct < record.time; ct++ {
		d := distance(ct, record.time)
		if d > record.distance {
			ctMin = aoc.Min(ctMin, ct)
			ctMax = aoc.Max(ctMax, ct)
		}
	}
	return ctMin, ctMax
}

func toRace2(races []RaceResult) RaceResult {
	var time, dist string
	for _, r := range races {
		time += strconv.Itoa(r.time)
		dist += strconv.Itoa(r.distance)
	}
	timeInt, err := strconv.Atoi(time)
	if err != nil {
		panic(time)
	}
	distInt, err := strconv.Atoi(dist)
	if err != nil {
		panic(distInt)
	}
	return RaceResult{time: timeInt, distance: distInt}
}

func countWinningRanges(record RaceResult) int {
	ctMin, ctMax := winningRange(record)
	return ctMax - ctMin + 1
}

func (d Day06) Solve() aoc.Solution {
	var part1, part2 int
	records := parse(aoc.ReadFile("06"))

	part1 = 1
	for _, r := range records {
		part1 *= countWinningRanges(r)
	}

	part2 = countWinningRanges(toRace2(records))

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
