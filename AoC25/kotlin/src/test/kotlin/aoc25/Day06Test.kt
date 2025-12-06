package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day06Test {

    fun parseLine(line: String): List<String> = line.split("\\s+".toRegex()).filter { it.isNotBlank() }.toList()

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        val split: List<List<String>> = list.map { parseLine(it) }.toList()
        val sizes = split.map { it.size }.groupBy { it }
        assertEquals(1, sizes.size)
        var answer1 = 0L
        val n: Int = sizes.keys.first()
        for (i in 0..<n) {
            val operation = split.last()[i]
            var total = split.first()[i].toLong()
            for (j in split.drop(1).dropLast(1)) {
                val value = j[i].toLong()
                when (operation) {
                    "*" -> total *= value
                    "+" -> total += value
                }
            }
            answer1 += total
        }

        var answer2 = 0L
        val columns =
            list.last().withIndex().filter { (_, char) -> !char.isWhitespace() }.map { (index, char) -> index to char }
                .toList()
        val numbers = list.dropLast(1)
        val b: List<Int> = columns.drop(1).map { p -> p.first - 1 }.toList() + (numbers.maxOf { l -> l.length })
        val ab = columns.map { p -> p.first }.zip(b)
        for (i in ab) {
            val startIndex = i.first
            val endIndex = i.second // not included
            val operation = list.last().substring(startIndex, startIndex + 1).trim()
            var total: Long? = null
            for (j in startIndex..<endIndex) {
                val str = String(numbers.filter { l -> j < l.length }.map { l -> l[j] }.toCharArray())
                val n = str.trim()
                val value = n.toLong()
                if (total == null) total = value
                else
                    when (operation) {
                        "*" -> total *= value
                        "+" -> total += value
                    }
            }
            answer2 += total!!
        }
        return answer1 to answer2
    }

    @Test
    fun test() {
        val result = solution("06-1")
        assertEquals(4277556L, result.first)
        assertEquals(3263827L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("06")
        assertEquals(5381996914800L, result.first)
        assertEquals(9627174150897L, result.second)
    }
}