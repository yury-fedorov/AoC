package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day04Test {
    data class Position(val x: Int, val y: Int)

    val adjacentShifts = setOf<Position>(
        Position(-1, -1), Position(-1, 0), Position(-1, 1),
        Position(0, -1), Position(0, 1), Position(1, -1), Position(1, 0),
        Position(1, 1)
    )

    val roll = '@'

    fun shift(p: Position, shift: Position): Position = Position(p.x + shift.x, p.y + shift.y)

    fun at(map: List<String>, p: Position): Char {
        if (p.y < 0 || p.y >= map.size) return '.'
        val line = map[p.y]
        if (p.x < 0 || p.x >= line.length) return '.'
        return line[p.x]
    }

    fun at2(map: List<String>, p: Position, removed: Collection<Position>): Char {
        if (removed.contains(p)) return '.'
        return at(map, p)
    }

    fun countRolls(map: List<String>, p: Position, removed: Collection<Position>): Int =
        adjacentShifts.map { s -> at2(map, shift(p, s), removed) }.count { c -> c == roll }

    fun accessibleRolls(map: List<String>, removed: Collection<Position>): List<Position> {
        val result = mutableListOf<Position>()
        for (y in 0..<map.size) {
            val line = map[y]
            for (x in 0..<line.length) {
                val p = Position(x, y)
                val me = at2(map, p, removed)
                if (me != roll) continue
                val adjCount = countRolls(map, p, removed)
                if (adjCount <= 3) {
                    result += p
                }
            }
        }
        return result
    }

    fun answer1(map: List<String>): Int = accessibleRolls(map, emptySet()).size
    fun answer2(map: List<String>): Int {
        val removed = mutableSetOf<Position>()
        while (true) {
            val removable = accessibleRolls(map, removed)
            if (removable.isEmpty()) break
            removed.addAll(removable)
        }
        return removed.size
    }

    @Test
    fun test() {
        val map = IOUtil.input("04-1")
        assertEquals(13, answer1(map))
        assertEquals(43, answer2(map))
    }

    @Test
    fun solution() {
        val map = IOUtil.input("04")
        assertEquals(1346, answer1(map))
        assertEquals(8493, answer2(map))
    }
}