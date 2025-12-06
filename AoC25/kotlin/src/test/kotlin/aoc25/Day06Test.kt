package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day06Test {

    fun parseLine(line: String): List<String> = line.split("\\s+".toRegex()).filter { it.isNotBlank() }.toList()

    fun total(numbers: Collection<Long>, operation: String): Long = when (operation) {
        "+" -> numbers.sum()
        "*" -> numbers.reduce { acc, n -> acc * n }
        else -> throw IllegalStateException()
    }

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data)
        val split: List<List<String>> = list.map { parseLine(it) }.toList()
        val sizes = split.map { it.size }.groupBy { it }
        assertEquals(1, sizes.size)
        var answer1 = 0L
        val n: Int = sizes.keys.first()
        for (i in 0..<n) {
            val operation = split.last()[i]
            val elements = split.dropLast(1).map { it[i].toLong() }
            answer1 += total(elements, operation)
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
            val elements = mutableListOf<Long>()
            for (j in startIndex..<endIndex) {
                val str = String(numbers.filter { l -> j < l.length }.map { l -> l[j] }.toCharArray())
                elements += str.trim().toLong()
            }
            answer2 += total(elements, operation)
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