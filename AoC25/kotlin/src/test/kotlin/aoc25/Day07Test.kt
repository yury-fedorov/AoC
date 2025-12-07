package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day07Test {

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        val splitterCount = list.sumOf { line -> line.filter { c -> c == '^' }.length }.toLong() // fast approx
        var answer1 = 0L
        var sources = mutableSetOf( list.first().indexOf('S') )
        for (line in list.drop(1)) {
            var newSources = mutableSetOf<Int>()
            var hitSplitterCount = mutableSetOf<Int>()
            for (src in sources) {
                val ch = line[src]
                if (ch == '^') {
                    newSources += src - 1
                    newSources += src + 1
                    hitSplitterCount += src
                } else {
                    newSources += src
                }
            }
            answer1 += hitSplitterCount.size
            sources = newSources
        }
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
        assertEquals(1516L, result.first) // max - 1640
        assertEquals(0L, result.second)
    }
}