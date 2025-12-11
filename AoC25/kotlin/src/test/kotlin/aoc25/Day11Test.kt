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

    fun explore(graph: Graph, from: Node, path: Path, found: MutableList<Path>, toFilter: Boolean) {
        val nextList = graph[from] ?: return
        for (next in nextList) {
            val path1 = path + next
            if (next == OUT) {
                if (toFilter)
                    if (path1.contains("dac") && path1.contains("fft"))
                        // to be added to the result
                    else
                        continue // we skip this path
                found += path1
            } else if (path.contains(next)) {
                // we have already been in this node
            } else {
                explore(graph, next, path1, found, toFilter)
            }
        }
    }

    fun createGraph(data:String): Graph = IOUtil.input(data).associate { parse(it) }

    fun answer1(graph: Graph): Int {
        val answer1 = mutableListOf<Path>()
        explore(graph, YOU, listOf(), answer1, false)
        return answer1.size
    }

    fun answer2(graph: Graph): Int {
        val answer2 = mutableListOf<Path>()
        explore(graph, "svr", listOf(), answer2, true)
        return answer2.size
    }


    @Test
    fun test() {
        val g1 = createGraph("11-1")
        assertEquals(5, answer1(g1))
        val g2 = createGraph("11-2")
        assertEquals(2, answer2(g2))
    }

    // @Test
    fun solution() {
        val graph = createGraph("11")
        assertEquals(643, answer1(graph))
        assertEquals(0, answer2(graph))
    }
}