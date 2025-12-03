package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day03Test {

    @Test
    fun test() {
        val answer = solution("01-1")
        assertEquals(3, answer.first)
        assertEquals(6, answer.second)
    }

    @Test
    fun solution() {
        val answer = solution("01")
        assertEquals(1141, answer.first)
        assertEquals(6634, answer.second)
    }
}