package aoc25

import kotlin.math.pow
import kotlin.math.sqrt
import kotlin.test.Test
import kotlin.test.assertEquals

class Day08Test {

    data class Point(val x: Int, val y: Int, val z: Int) {}

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

    fun sort(a: Point, b: Point): Pair<Point, Point> = if (compare(a, b) < 0) Pair(a, b) else Pair(b, a)

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).map { l -> parse(l) }.toList()
        for (a in list) {
            for (b in list) {
                if (a == b) continue
            }
        }
        return 0L to 0L
    }

    @Test
    fun test() {
        val result = solution("08-1")
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("08")
        assertEquals(0L, result.first)
        assertEquals(0L, result.second)
    }
}