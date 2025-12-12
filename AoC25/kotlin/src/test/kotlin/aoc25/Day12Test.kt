package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day12Test {
    // rotation
    // flipping

    // Presents can be rotated and flipped as necessary to make them fit in the available space, but they have to always
    // be placed perfectly on the grid. Shapes can't overlap (that is, the # part from two different presents can't go
    // in the same place on the grid), but they can fit together (that is, the . part in a present's shape's diagram
    // does not block another present from occupying that space on the grid).
    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        for (line in list) { // TODO
        }
        return 0L to 0L
    }

    @Test
    fun test() {
        val result = solution("12-1") // TODO
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("12") // TODO
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }
}