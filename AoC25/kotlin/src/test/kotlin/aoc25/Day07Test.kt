package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day07Test {

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        // val splitterCount = list.sumOf { line -> line.filter { c -> c == '^' }.length }.toLong() // fast approx
        var answer1 = 0L
        var sources = mutableSetOf( list.first().indexOf('S') )
        // var paths = sources.map{ p -> listOf(p)}.toMutableSet() // part 2
        var paths = sources.associateWith { p -> 1L }
        for (line in list.drop(1)) {
            var newSources = mutableSetOf<Int>()
            var hitSplitterCount = mutableSetOf<Int>()
            var newPaths = mutableMapOf<Int,Long>()
            for (src in sources) {
                val ch = line[src]
                // val selected = paths.filter{ l -> l.last() == src }
                if (ch == '^') {
                    val splits = listOf(src - 1, src + 1)
                    newSources.addAll(splits)
                    hitSplitterCount += src
                    // part 2
                    for (split:Int in splits) {
                        val count:Long = newPaths.getOrDefault(split,0L)
                        val prevCount:Long = paths[src]!!
                        newPaths[split] = (count + prevCount)
                    }
                    // newPaths.addAll( selected.map{ s -> s + (src + 1) } )
                } else {
                    newSources += src
                    val count:Long = newPaths.getOrDefault(src,0L)
                    val prevCount:Long = paths[src]!!
                    newPaths[src] = (count + prevCount)
                }
            }
            answer1 += hitSplitterCount.size
            sources = newSources
            paths = newPaths
        }
        // val lastLineCount = sources.size.toLong()
        // return answer1 to paths.size.toLong()
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
        assertEquals(0L, result.second) // position 5377
    }
}