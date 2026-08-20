package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day11Test {

    typealias Node = String
    typealias Links = Pair<Node, Set<Node>>
    typealias Graph = Map<Node, Set<Node>>

    val YOU = "you"
    val SVR = "svr"
    val OUT = "out"

    fun parse(line: String): Links {
        val fromOther = line.split(':')
        val to = fromOther.last().trim().split("\\s+".toRegex()).toSet()
        return Links(fromOther.first().trim(), to)
    }

    fun countPaths(graph: Graph, node: Node, mask: Int, memo: MutableMap<Pair<Node, Int>, Long> = mutableMapOf()): Long {
        var nextMask = mask
        if (node == "dac") nextMask = nextMask or 1
        if (node == "fft") nextMask = nextMask or 2

        if (node == OUT) {
            return if (nextMask == 3) 1L else 0L
        }

        val key = node to nextMask
        memo[key]?.let { return it }

        var total = 0L
        val nextList = graph[node] ?: emptySet()
        for (next in nextList) {
            total += countPaths(graph, next, nextMask, memo)
        }
        memo[key] = total
        return total
    }

    fun createGraph(data: String): Graph = IOUtil.input(data).filter { it.isNotBlank() }.associate { parse(it) }

    fun answer1(graph: Graph): Long = countPaths(graph, YOU, 3)

    fun answer2(graph: Graph): Long = countPaths(graph, SVR, 0)

    @Test
    fun test() {
        val g1 = createGraph("11-1")
        assertEquals(5L, answer1(g1))
        val g2 = createGraph("11-2")
        assertEquals(2L, answer2(g2))
    }

    @Test
    fun solution() {
        val graph = createGraph("11")
        assertEquals(643L, answer1(graph))
        assertEquals(417190406827152L, answer2(graph))
    }
}