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

    fun countRolls(map: List<String>, p: Position): Int =
        adjacentShifts.map { s -> at(map, shift(p, s)) }.count { c -> c == roll }

    fun accessibleRolls(map: List<String>): List<Position> {
        val result = mutableListOf<Position>()
        for (y in 0..<map.size) {
            val line = map[y]
            for (x in 0..<line.length) {
                val p = Position(x, y)
                val me = at(map, p)
                if (me != roll) continue
                val adjCount = countRolls(map, p)
                if (adjCount <= 3) {
                    result += p
                }
            }
        }
        return result
    }

    @Test
    fun test() {
        val map = IOUtil.input("04-1")
        assertEquals(13, accessibleRolls(map).size)
    }


    @Test
    fun solution() {
        val map = IOUtil.input("04")
        assertEquals(1346, accessibleRolls(map).size)
    }
}