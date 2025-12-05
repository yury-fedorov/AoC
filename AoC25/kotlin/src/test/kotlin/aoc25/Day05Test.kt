package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day05Test {
    data class Range(val from:Long, val to:Long) {
        fun isIn(value: Long): Boolean = value in from..to
    }
    @Test
    fun solution() {
        val list = IOUtil.input("05-1")
        val freshRanges = mutableListOf<Range>()
        var freshCount = 0
        var isInitialization = true // till empty line, freshRanges list is initialized
        for (line in list) {
            if (line.isEmpty()) isInitialization = false
            else if (isInitialization) {
                val ab = line.split('-')
                freshRanges += Range(ab.first().toLong(), ab.last().toLong())
            } else {
                val value = line.toLong()
                if ( freshRanges.any{r -> r.isIn(value)} ) freshCount++
            }
        }
        assertEquals(3, freshCount)
    }
}