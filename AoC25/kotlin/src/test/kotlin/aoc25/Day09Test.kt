package aoc25

import kotlin.math.abs
import kotlin.math.min
import kotlin.math.max
import kotlin.test.Test
import kotlin.test.assertEquals

class Day09Test {

    data class Point(val x: Int, val y: Int) {}
    typealias Points = Collection<Point>
    typealias Line = Pair<Point,Point>
    typealias Shape = Collection<Line>
    typealias Rectangle = Pair<Point,Point> // min - max
    fun area(a: Point, b: Point): Long = (abs(a.x - b.x) + 1L) * (abs(a.y - b.y) + 1L)
    /*
    fun isGreenRed(a:Point,b:Point, shape:Shape): Boolean {
        val minX = min(a.x, b.x)
        val maxX = max(a.x, b.x)
        val minY = min(a.y, b.y)
        val maxY = min(a.y, b.y)
        val rect = Rectangle(Point(minX, minY), Point(maxX, maxY))
        shape.any{  }
    }
    */

    fun grid(shape:Shape): Pair<List<Int>, List<Int>> {
        val points = shape.flatMap{ l -> listOf(l.first, l.second) }
        val x = points.map{ p -> p.x }.toSortedSet().toList()
        val y = points.map{ p -> p.y }.toSortedSet().toList()
        return Pair(x,y)
    }

    fun parse(line:String):Point {
        val ab = line.split(",").map { n -> n.toInt() }
        return Point(ab[0], ab[1])
    }

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).map{l -> parse(l)}
        val answer1 = list.flatMap { a -> list.map{ b -> area(a, b) } }.max()
        val shape = list.zip(list.drop(1) + list.take(1))
        val grid = grid(shape)
        return answer1 to 0L
    }

    @Test
    fun test() {
        val result = solution("09-1")
        assertEquals(50L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("09")
        assertEquals(4725826296L, result.first)
        assertEquals(0L, result.second)
    }
}