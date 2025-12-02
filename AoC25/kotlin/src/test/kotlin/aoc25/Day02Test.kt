package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day02Test {

    fun isValid(id: String): Boolean {
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
        assertEquals(true, isValid("101"))
        assertEquals(false, isValid("55"))
        assertEquals(false, isValid("6464"))
        assertEquals(false, isValid("123123"))
    }

    fun solution(data: String): List<Long> {
        var invalid = mutableListOf<Long>()
        val ranges = IOUtil.input(data).first().split(",")
        for ( range in ranges ) {
            val list = range.split("-")
            val min = list[0].toLong()
            var max = list[1].toLong()
            for ( i in min .. max ) {
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
        val invalidIdList : List<Long> = solution("02-1")
        val answer : Long = invalidIdList.sum()
        assertEquals(1227775554L, answer)
    }

    @Test
    fun solution() {
        val invalidIdList : List<Long> = solution("02")
        val answer : Long = invalidIdList.sum()
        assertEquals(40214376723L, answer)
    }

}