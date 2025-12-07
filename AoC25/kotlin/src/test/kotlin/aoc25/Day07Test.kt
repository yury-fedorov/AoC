package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day07Test {

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        val answer1 = list.sumOf { line -> line.filter { c -> c == '^' }.length }.toLong() // fast approx
        return answer1 to 0L
    }

    @Test
    fun test() {
        val result = solution("07-1") // TODO
        assertEquals(21L, result.first) // max - 22
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("07") // TODO
        assertEquals(0L, result.first) // max - 1640
        assertEquals(0L, result.second)
    }
}