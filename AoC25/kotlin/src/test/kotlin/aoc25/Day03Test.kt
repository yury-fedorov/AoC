package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day03Test {

    fun largestVoltage(bank:String): Int {
        return "${bank[0]}${bank[1]}".toInt()
    }

    @Test
    fun largestTest() {
        assertEquals(98, largestVoltage("987654321111111"))
    }

    fun solution(case: String): Pair<Int, Int> {
        val list = IOUtil.input(case)
        val answer1 = list.sumOf { bank -> largestVoltage(bank) }
        return answer1 to 0
    }

    @Test
    fun test() {
        val answer = solution("03-1")
        assertEquals(357, answer.first)
        assertEquals(0, answer.second)
    }

    @Test
    fun solution() {
        val answer = solution("03")
        assertEquals(0, answer.first)
        assertEquals(0, answer.second)
    }
}