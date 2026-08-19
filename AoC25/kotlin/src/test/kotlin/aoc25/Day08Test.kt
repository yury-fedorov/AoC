package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day08Test {

    data class Point(val x: Int, val y: Int, val z: Int)
    data class Edge(val u: Int, val v: Int, val distSq: Long)

    class DSU(val n: Int) {
        val parent = IntArray(n) { it }
        val size = IntArray(n) { 1 }
        var components = n

        fun find(i: Int): Int {
            if (parent[i] == i) return i
            parent[i] = find(parent[i])
            return parent[i]
        }

        fun union(u: Int, v: Int): Boolean {
            val rootU = find(u)
            val rootV = find(v)
            if (rootU != rootV) {
                parent[rootV] = rootU
                size[rootU] += size[rootV]
                components--
                return true
            }
            return false
        }
    }

    fun parse(line: String): Point {
        val (x, y, z) = line.split(",").map { it.trim().toInt() }
        return Point(x, y, z)
    }

    fun solution(data: String, n: Int): Pair<Long, Long> {
        val points = IOUtil.input(data).filter { it.isNotBlank() }.map { parse(it) }
        val edges = mutableListOf<Edge>()
        for (i in points.indices) {
            val a = points[i]
            for (j in i + 1 until points.size) {
                val b = points[j]
                val dx = (a.x - b.x).toLong()
                val dy = (a.y - b.y).toLong()
                val dz = (a.z - b.z).toLong()
                edges.add(Edge(i, j, dx * dx + dy * dy + dz * dz))
            }
        }
        edges.sortBy { it.distSq }

        // Part 1: Connect the first n closest pairs
        val dsu1 = DSU(points.size)
        for (i in 0 until minOf(n, edges.size)) {
            dsu1.union(edges[i].u, edges[i].v)
        }
        val compSizes = (0 until points.size)
            .filter { dsu1.find(it) == it }
            .map { dsu1.size[it].toLong() }
            .sortedDescending()
        val answer1 = compSizes.take(3).reduce { acc, s -> acc * s }

        // Part 2: Connect until all junction boxes form a single circuit
        val dsu2 = DSU(points.size)
        var answer2 = 0L
        for (edge in edges) {
            if (dsu2.union(edge.u, edge.v)) {
                if (dsu2.components == 1) {
                    answer2 = points[edge.u].x.toLong() * points[edge.v].x.toLong()
                    break
                }
            }
        }

        return answer1 to answer2
    }

    @Test
    fun test() {
        val result = solution("08-1", 10)
        assertEquals(40L, result.first)
        assertEquals(25272L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("08", 1_000)
        assertEquals(54600L, result.first)
        assertEquals(107256172L, result.second)
    }
}