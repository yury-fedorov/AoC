# Advent of Code 2023 in Go

## Technological Stack & Conventions
* Go 1.20 (as 1.21 is missing on https://github.com)
* [Google Go Style Guide](https://google.github.io/styleguide/go/guide)

## TODO
1. Day10 Part2 - count properly if we are in or out of the tube.
2. Day12 Part2 - combinatorics
3. [Day17](https://adventofcode.com/2023/day/17) - Clumsy Crucible - (**2 stars missing**)
4. Day18 Part2 - Lavaduct Lagoon (big space calcs)
5. [Day20](https://adventofcode.com/2023/day/20) - Pulse Propagation (**no code for part2**)
6. Day21 Part2 - expansion of map / long running (**no code for part 2**)
7. [Day22](https://adventofcode.com/2023/day/22) - Sand Slabs (**wip**) - (**2 stars missing**)
8. [Day24 Part2](https://adventofcode.com/2023/day/24) - Never Tell Me The Odds (**no code for part 2**)
9. [Day25](https://adventofcode.com/2023/day/25) - Snowverload (slow)

## Interesting Parts
1. `Day03` - `aoc.Abs()` with generics for `int` and `int64`
2. `Day04` - `aoc.Pow()` with generics for `int` and `int64`
3. `Day05` - channels and goroutines
4. `Day20` - abstract class pattern

## Tech Debt
1. `Day05` - `Sort` and (binary) `Search`
2. `Day19` - may be scaled with goroutines (part 2 is the slowest so far)
3. [Day23](https://adventofcode.com/2023/day/23) - A Long Walk - part 2 takes almost 8h
