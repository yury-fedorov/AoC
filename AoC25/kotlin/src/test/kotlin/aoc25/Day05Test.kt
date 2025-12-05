package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day05Test {
    data class Range(val from: Long, val to: Long) {
        fun isIn(value: Long): Boolean = value in from..to
        fun isIntersect(r: Range) = isIn(r.from) || isIn(r.to) // at least one side within other
        fun merge(other: Range): Range = Range(Math.min(from, other.from), Math.max(to, other.to))
        fun count(): Long = to - from + 1
    }

    fun merge(r: Range, ranges: Collection<Range>): Collection<Range> {
        val newCounted = mutableListOf<Range>()
        var wasAnyMerge = false
        var newRange = r
        for (c in ranges) {
            if (newRange.isIntersect(c)) {
                newRange = newRange.merge(c)
                wasAnyMerge = true
            } else newCounted += c
        }
        if (wasAnyMerge) return merge(newRange, newCounted)
        return ranges + r
    }

    fun findIntersect(ranges: Collection<Range>): Pair<Range, Range>? {
        for (r1 in ranges) {
            for (r2 in ranges) {
                if (r1 == r2) continue
                if (r1.isIntersect(r2)) return r1 to r2
            }
        }
        return null
    }

    // answer 2
    fun countRanges(ranges: Collection<Range>): Long {
        var counted: Collection<Range> = listOf()
        ranges.forEach { r -> counted = merge(r, counted) }
        while (true) {
            val crossed = findIntersect(counted) ?: break
            val newRange = crossed.first.merge(crossed.second)
            counted = merge(newRange, counted)
        }
        return counted.sumOf { c -> c.count() }
    }

    fun solution(data: String): Pair<Int, Long> {
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
                if (freshRanges.any { r -> r.isIn(value) }) freshCount++
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
        assertEquals(339668510830757, result.second) // too high - 351272315451986, 349958946031224
    }
}