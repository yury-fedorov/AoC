package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day02Test {

    fun isValid1(id: String): Boolean {
        // you can find the invalid IDs by looking for any ID which is made only of some sequence of digits repeated twice.
        // So, 55 (5 twice), 6464 (64 twice), and 123123 (123 twice) would all be invalid IDs.
        if (id.length % 2 == 1) return true
        val halfLength = id.length / 2
        val left = id.take(halfLength)
        val right = id.substring(halfLength)
        return left != right
    }

    @Test
    fun invalidIds() {
        assertEquals(true, isValid1("101"))
        assertEquals(false, isValid1("55"))
        assertEquals(false, isValid1("6464"))
        assertEquals(false, isValid1("123123"))
    }

    fun solution(data: String, isValid: (id: String) -> Boolean): List<Long> {
        var invalid = mutableListOf<Long>()
        val ranges = IOUtil.input(data).first().split(",")
        for (range in ranges) {
            val list = range.split("-")
            val min = list[0].toLong()
            val max = list[1].toLong()
            for (i in min..max) {
                val id = i.toString()
                if (!isValid(id)) {
                    invalid += i
                    // println(id)
                }
            }
        }
        return invalid
    }

    @Test
    fun answer1Sample() {
        val invalidIdList: List<Long> = solution("02-1") { id: String -> isValid1(id) }
        val answer: Long = invalidIdList.sum()
        assertEquals(1227775554L, answer)
    }

    @Test
    fun solution1() {
        val invalidIdList: List<Long> = solution("02") { id: String -> isValid1(id) }
        val answer: Long = invalidIdList.sum()
        assertEquals(40214376723L, answer)
    }

    fun isValid2(id: String): Boolean {
        // an ID is invalid if it is made only of some sequence of digits repeated at least twice.
        // So, 12341234 (1234 two times), 123123123 (123 three times), 1212121212 (12 five times),
        // and 1111111 (1 seven times) are all invalid IDs.
        val maxN = id.length
        for (numberOfChuncks in 2..maxN) {
            if (id.length % numberOfChuncks != 0) continue
            val chunckSize = id.length / numberOfChuncks
            val chunks = id.chunked(chunckSize)
            val left = chunks[0]
            val invalid = chunks.all { it == left }
            if (invalid) return false
        }
        return true
    }

    @Test
    fun invalid2Ids() {
        assertEquals(false, isValid2("12341234"))
        assertEquals(false, isValid2("123123123"))
        assertEquals(false, isValid2("1212121212"))
        assertEquals(false, isValid2("1111111"))
    }

    @Test
    fun answer2Sample() {
        val invalidIdList: List<Long> = solution("02-1") { id: String -> isValid2(id) }
        val answer: Long = invalidIdList.sum()
        assertEquals(4174379265L, answer)
    }

    @Test
    fun solution2() {
        val invalidIdList: List<Long> = solution("02") { id: String -> isValid2(id) }
        val answer: Long = invalidIdList.sum()
        assertEquals(50793864718L, answer)
    }
}