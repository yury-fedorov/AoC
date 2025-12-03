package aoc25

import kotlin.math.max
import kotlin.test.Test
import kotlin.test.assertEquals

class Day03Test {

    fun largestVoltage(bank:String): Int {
        var result = 0
        for ( a in 0 .. bank.length - 2) {
            for ( b in a + 1 .. bank.length - 1) {
                result = max(result, "${bank[a]}${bank[b]}".toInt())
            }
        }
        return result
    }

    @Test
    fun largestTest() {
        assertEquals(98, largestVoltage("987654321111111"))
        assertEquals(89, largestVoltage("811111111111119"))
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
        assertEquals(17244, answer.first)
        assertEquals(0, answer.second)
    }
}