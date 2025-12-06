package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day06Test {

    fun parseLine(line: String): List<String> = line.split("\\s+".toRegex()).filter { it.isNotBlank() }.toList()

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        val splitted :List<List<String>> = list.map { parseLine(it) }.toList()
        val sizes = splitted.map{ it.size }.groupBy { it }
        assertEquals(1, sizes.size)
        var answer1 = 0L
        val n : Int = sizes.keys.first()
        for (i in 0 ..< n ) {
            val operation = splitted.last()[i]
            var total = splitted.first()[i].toLong()
            for ( j in splitted.drop(1).dropLast(1) ) {
                val value = j[i].toLong()
                when (operation) {
                    "*" -> total *= value
                    "+" -> total += value
                }
            }
            answer1 += total
        }
        return answer1 to 0L
    }

    @Test
    fun test() {
        val result = solution("06-1")
        assertEquals(4277556L, result.first)
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("06")
        assertEquals(5381996914800L, result.first)
        assertEquals(0L, result.second)
    }
}