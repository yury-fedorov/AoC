package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day05Test {
    data class Range(val from:Long, val to:Long) {
        fun isIn(value: Long): Boolean = value in from..to
        fun isIntersect(r: Range) = isIn(r.from) || isIn(r.to) // at least one side within other
        fun merge(other:Range): Range = Range(Math.min(from, other.from), Math.max(to, other.to))
        fun count() : Long = to - from + 1
    }

    fun merge(r:Range, ranges: Collection<Range>):Pair<Collection<Range>,Range?> {
        if (ranges.any{ c -> c.isIntersect(r)}) {
            // we need to merge with at least one other range
            val newCounted = mutableListOf<Range>()
            var newRange = r
            for (c in ranges) {
                if ( newRange.isIntersect(c) ) newRange = newRange.merge(c)
                else newCounted += c
            }
            newCounted.sortBy { r -> r.from } // for better debugging
            return newCounted to newRange
        }
        return ranges + r to null
    }

    // answer 2
    fun countRanges(ranges:Collection<Range>) : Long {
        var counted: Collection<Range> = listOf() // modified from original
        for (r in ranges) {
            var range = r
            while (true) {
                val p = merge(range, counted)
                counted = p.first
                if (p.second == null) break
                range = p.second!!
            }
        }
        return counted.sumOf { c -> c.count()}
    }

    fun solution(data:String) : Pair<Int,Long> {
        val list = IOUtil.input(data)
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
        return freshCount to countRanges(freshRanges)
    }

    @Test
    fun test() {
        val result = solution("05-1")
        assertEquals(3, result.first)
        assertEquals(14, result.second)
    }

    @Test
    fun solution() {
        val result = solution("05")
        assertEquals(513, result.first)
        assertEquals(0, result.second) // too high - 351272315451986
    }
}