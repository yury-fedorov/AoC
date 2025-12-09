package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day11Test {

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        for (line in list) { // TODO
        }
        return 0L to 0L
    }

    @Test
    fun test() {
        val result = solution("01-1") // TODO
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("01") // TODO
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }
}