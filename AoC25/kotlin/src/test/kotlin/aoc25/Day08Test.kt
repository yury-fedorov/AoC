package aoc25

import kotlin.collections.associateWith
import kotlin.math.pow
import kotlin.math.sqrt
import kotlin.test.Test
import kotlin.test.assertEquals

class Day08Test {

    data class Point(val x: Int, val y: Int, val z: Int) {}
    typealias Graph = Pair<Point, Point>
    typealias Graphs = Collection<Graph>

    fun p2(n: Number): Double = n.toDouble().pow(2.0)

    fun distance(a: Point, b: Point): Double = sqrt(p2(a.x - b.x) + p2(a.y - b.y) + p2(a.z - b.z))

    fun parse(line: String): Point {
        val xyz = line.split(",").map { n -> n.toInt() }
        return Point(xyz[0], xyz[1], xyz[2])
    }

    fun compare(a: Point, b: Point): Int {
        val x = a.x.compareTo(b.x)
        if (x != 0) return x
        val y = a.y.compareTo(b.y)
        if (y != 0) return y
        return a.z.compareTo(b.z)
    }

    fun sort(a: Point, b: Point): Graph = if (compare(a, b) < 0) Graph(a, b) else Graph(b, a)

    // fun withDistance(a: Point, list: Collection<Point>): Map<Point, Double> = list.associateWith { p -> distance(a, p) }

    fun nClosest(data: String, n: Int): Graphs {
        val list = IOUtil.input(data).map { l -> parse(l) }.toList()
        val pairs = list.flatMap { a -> list.filter { b -> a != b }.map { b -> sort(a, b) } }.toSet()
        val pairsDistance = pairs.associateWith { p -> distance(p.first, p.second) }
        val distances = pairsDistance.values.toSet().sorted().toList()
        val result = mutableListOf<Pair<Point, Point>>()
        for (d in distances) {
            if (result.size >= n) break;
            result.addAll(pairsDistance.filter { p -> p.value == d }.map { p -> p.key })
        }
        return result
    }

    fun contains(point: Point, graphs: Graphs): Boolean = graphs.any { g -> point === g.first || point === g.second }

    fun group(graphs: Collection<Graph>): Collection<Graphs> {
        val result = mutableListOf<Set<Graph>>()
        for (graph in graphs) {
            val s = result.find { g -> contains(graph.first, g) || contains(graph.second, g) }
            if (s != null) {
                result.remove(s)
                val s1 = result.findLast { g -> contains(graph.first, g) || contains(graph.second, g) }
                if (s1 != null) {
                    // merging s and s1
                    result.remove(s1)
                    result += (s + s1 + graph)
                } else
                    result += (s + graph)
            } else {
                result += setOf(graph)
            }
        }
        return result
    }

    fun solution(data: String, n: Int): Pair<Long, Long> {
        val graphs = nClosest(data, n)
        val g = group(graphs)
        val biggest = g.map { c: Collection<Graph> -> c.size }.sortedDescending().take(3)
        val answer1 = biggest.reduce { acc, n -> acc * n }
        return answer1.toLong() to 0L
    }

    // @Test
    fun test() {
        val result = solution("08-1", 10)
        assertEquals(40L, result.first)
        assertEquals(0L, result.second)
    }

    // @Test
    fun solution() {
        val result = solution("08", 1_000)
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }
}