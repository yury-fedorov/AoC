package aoc25

import kotlin.math.abs
import kotlin.test.Test
import kotlin.test.assertEquals

class Day09Test {

    data class Point(val x: Int, val y: Int) {}

    fun area(a: Point, b: Point): Long = (abs(a.x - b.x) + 1L) * (abs(a.y - b.y) + 1L)

    fun parse(line:String):Point {
        val ab = line.split(",").map { n -> n.toInt() }
        return Point(ab[0], ab[1])
    }

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).map{l -> parse(l)}
        val answer1 = list.flatMap { a -> list.map{ b -> area(a, b) } }.max()
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