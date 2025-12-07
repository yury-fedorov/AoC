package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day07Test {

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        var answer1 = 0L
        var sources = mutableSetOf(list.first().indexOf('S'))
        var paths = sources.associateWith { p -> 1L }
        for (line in list.drop(1)) {
            var newSources = mutableSetOf<Int>()
            var hitSplitterCount = mutableSetOf<Int>()
            var newPaths = mutableMapOf<Int, Long>()
            val addToPaths = { src: Int, split: Int ->
                val count: Long = newPaths.getOrDefault(split, 0L)
                val prevCount: Long = paths[src]!!
                newPaths[split] = (count + prevCount)
            }
            for (src in sources) {
                val ch = line[src]
                if (ch == '^') {
                    val splits = listOf(src - 1, src + 1)
                    newSources.addAll(splits)
                    hitSplitterCount += src
                    splits.forEach { split -> addToPaths(src, split) } // part 2
                } else {
                    newSources += src
                    addToPaths(src, src)
                }
            }
            answer1 += hitSplitterCount.size
            sources = newSources
            paths = newPaths
        }
        return answer1 to paths.values.sum()
    }

    @Test
    fun test() {
        val result = solution("07-1")
        assertEquals(21L, result.first) // max - 22
        assertEquals(40L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("07")
        assertEquals(1516L, result.first) // max - 1640
        assertEquals(1393669447690L, result.second) // position 5377
    }
}