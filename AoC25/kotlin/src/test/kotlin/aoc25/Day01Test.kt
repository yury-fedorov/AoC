package aoc25

import kotlin.math.absoluteValue
import kotlin.test.Test
import kotlin.test.assertEquals

class Day01Test {

    fun move(counter: Int, move: String) : Pair<Int, Int> {
        val direction = move.substring(0, 1)
        val directionSign : Int = if ( direction == "R" ) { 1 } else { -1 }
        val shift : Int = move.substring(1).toInt()
        val rowResult : Int = counter + (directionSign * shift)
        val modResult = rowResult / 100
        val newCounter = rowResult % 100
        val isNegative = rowResult < 0
        val clicks = modResult.absoluteValue
        if (isNegative) {
            return newCounter + 100 to clicks + 1
        }
        return newCounter to clicks
    }

    @Test
    fun test() {
        val list = IOUtil.input("01-1")
        var counter = 50
        var zeroCount = 0
        for (aMove in list) {
            val counterPair = move(counter, aMove)
            counter = counterPair.first
            if (counter == 0) {
                zeroCount++
            }
        }
        assertEquals(3, zeroCount)
    }

    @Test
    fun solution() {
        val list = IOUtil.input("01")
        var counter = 50
        var zeroCount1 = 0
        var zeroCount2 = 0
        for (aMove in list) {
            println( "Before: $counter shift: $aMove")
            val counterPair = move(counter, aMove)
            counter = counterPair.first
            var clicks = counterPair.second
            println( "After: $counter clicks: $clicks")
            if (counter == 0) {
                zeroCount1++
                clicks--
            }
            zeroCount2 += clicks
        }
        // assertEquals(1141, zeroCount1)
        assertEquals(0, zeroCount1+zeroCount2)
    }
}