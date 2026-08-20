package aoc25

import kotlin.math.abs
import kotlin.math.max
import kotlin.math.min
import kotlin.test.Test
import kotlin.test.assertEquals

class Day09Test {

    data class Point(val x: Int, val y: Int)
    data class Line(val a: Point, val b: Point) {
        val isHorizontal = a.y == b.y
        val minX = min(a.x, b.x)
        val maxX = max(a.x, b.x)
        val minY = min(a.y, b.y)
        val maxY = max(a.y, b.y)
    }

    fun area(a: Point, b: Point): Long = (abs(a.x - b.x) + 1L) * (abs(a.y - b.y) + 1L)

    private fun isInsideOrOnBoundary(px: Double, py: Double, edges: List<Line>): Boolean {
        // Boundary check
        val epsilon = 1e-9
        for (e in edges) {
            if (e.isHorizontal) {
                if (abs(e.a.y - py) < epsilon && px >= e.minX - epsilon && px <= e.maxX + epsilon) return true
            } else {
                if (abs(e.a.x - px) < epsilon && py >= e.minY - epsilon && py <= e.maxY + epsilon) return true
            }
        }
        // Ray casting (to the right)
        var crossings = 0
        for (e in edges) {
            if (!e.isHorizontal && e.a.x > px && py >= e.minY && py < e.maxY) {
                crossings++
            }
        }
        return crossings % 2 == 1
    }

    private fun isValidRectangle(p1: Point, p2: Point, edges: List<Line>): Boolean {
        val minX = min(p1.x, p2.x)
        val maxX = max(p1.x, p2.x)
        val minY = min(p1.y, p2.y)
        val maxY = max(p1.y, p2.y)

        // 1. Center must be inside or on polygon boundary
        val midX = (minX + maxX) / 2.0
        val midY = (minY + maxY) / 2.0
        if (!isInsideOrOnBoundary(midX, midY, edges)) return false

        // 2. No polygon edge may strictly cross the rectangle interior
        for (e in edges) {
            if (e.isHorizontal) {
                val ey = e.a.y
                if (ey in (minY + 1) until maxY && max(minX, e.minX) < min(maxX, e.maxX)) {
                    return false
                }
            } else {
                val ex = e.a.x
                if (ex in (minX + 1) until maxX && max(minY, e.minY) < min(maxY, e.maxY)) {
                    return false
                }
            }
        }
        return true
    }

    fun parse(line: String): Point {
        val (x, y) = line.split(",").map { it.trim().toInt() }
        return Point(x, y)
    }

    fun solution(data: String): Pair<Long, Long> {
        val points = IOUtil.input(data).filter { it.isNotBlank() }.map { parse(it) }
        val edges = points.indices.map { i -> Line(points[i], points[(i + 1) % points.size]) }

        var answer1 = 0L
        var answer2 = 0L

        for (i in points.indices) {
            val a = points[i]
            for (j in i + 1 until points.size) {
                val b = points[j]
                val rectArea = area(a, b)
                if (rectArea > answer1) answer1 = rectArea
                if (rectArea > answer2 && isValidRectangle(a, b, edges)) {
                    answer2 = rectArea
                }
            }
        }

        return answer1 to answer2
    }

    @Test
    fun test() {
        val result = solution("09-1")
        assertEquals(50L, result.first)
        assertEquals(24L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("09")
        assertEquals(4725826296L, result.first)
        assertEquals(1637556834L, result.second)
    }
}