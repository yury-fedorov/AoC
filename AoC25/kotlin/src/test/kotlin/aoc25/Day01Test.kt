package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day01Test {

    fun move(counter: Int, move: String): Pair<Int, Int> {
        val direction = move.take(1)
        val directionSign: Int = if (direction == "R") {
            1
        } else {
            -1
        }
        val shift: Int = move.substring(1).toInt()
        var clicks = 0
        var result = counter
        for (i in 1..shift) {
            val newResult = result + directionSign
            result = when (newResult) {
                100 -> 0
                -1 -> 99
                else -> newResult
            }
            if (result == 0) {
                // count the number of times any click causes the dial to point at 0
                clicks++
            }
        }
        return result to clicks
    }

    fun solution(case: String): Pair<Int, Int> {
        val list = IOUtil.input(case)
        var counter = 50
        var zeroCount1 = 0
        var zeroCount2 = 0
        for (aMove in list) {
            // print( "Before: $counter shift: $aMove ")
            val counterPair = move(counter, aMove)
            counter = counterPair.first
            val clicks = counterPair.second
            // println( "After: $counter clicks: $clicks")
            if (counter == 0) {
                zeroCount1++
            }
            zeroCount2 += clicks
        }
        return zeroCount1 to zeroCount2
    }

    @Test
    fun test() {
        val answer = solution("01-1")
        assertEquals(3, answer.first)
        assertEquals(6, answer.second)
    }

    @Test
    fun testR1000() {
        val result = move(50, "R1000")
        assertEquals(10, result.second)
    }

    @Test
    fun solution() {
        val answer = solution("01")
        assertEquals(1141, answer.first)
        assertEquals(6634, answer.second)
    }
}