package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day11Test {

    typealias Node = String
    typealias Links = Pair<Node, Set<Node>>
    typealias Graph = Map<Node, Set<Node>>
    typealias Path = List<Node>

    val YOU = "you"
    val OUT = "out"

    fun parse(line: String): Links {
        val fromOther = line.split(':')
        val to = fromOther.last().trim().split(' ').toSet()
        return Links(fromOther.first(), to)
    }

    fun explore(graph: Graph, from: Node, path: Path, found: MutableList<Path>) {
        val nextList = graph[from] ?: return
        for (next in nextList) {
            val path1 = path + next
            if (next == OUT) {
                found += path1
            } else if (path.contains(next)) {
                // we have already been in this node
            } else {
                explore(graph, next, path1, found)
            }
        }
    }

    fun solution(data: String): Pair<Long, Long> {
        val graph = IOUtil.input(data).associate { parse(it) }
        val result = mutableListOf<Path>()
        explore(graph, YOU, listOf(), result)
        return result.size.toLong() to 0L
    }

    @Test
    fun test() {
        val result = solution("11-1") // TODO
        assertEquals(5L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("11") // TODO
        assertEquals(643L, result.first)
        assertEquals(0L, result.second)
    }
}