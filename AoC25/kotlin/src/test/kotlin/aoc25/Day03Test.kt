package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day03Test {

    fun largestVoltage(bank: String): Int = optimizeBank(bank, 2).toInt()
    fun largestVoltage2(bank: String): Long = optimizeBank(bank, 12).toLong()

    @Test
    fun largestTest() {
        assertEquals(98, largestVoltage("987654321111111"))
        assertEquals(89, largestVoltage("811111111111119"))
    }

    fun optimizeBank(bank: String, minLength: Int): String {
        if (minLength == 1) return bank.max().toString()
        // we may try to trunk head starting always with highest
        var highest: Char = bank.max()
        while (true) {
            val firstHighest = bank.indexOf(highest)
            val highestStart = bank.drop(firstHighest)
            if (highestStart.length == minLength)
                return highestStart
            else if (highestStart.length > minLength) {
                return highestStart.first() + optimizeBank(highestStart.drop(1), minLength - 1)
            } else {
                // starting with highest does not permit to generate enough length, we scale down
                highest = bank.filter { c -> c < highest }.max()
            }
        }
    }

    fun solution(case: String): Pair<Int, Long> {
        val list = IOUtil.input(case)
        val answer1 = list.sumOf { bank -> largestVoltage(bank) }
        val answer2 = list.sumOf { bank -> largestVoltage2(bank) }
        return answer1 to answer2
    }

    @Test
    fun testTough() {
        val a = "2221332232313335223223326323322224151322223433133252223132133333322233214222322312232523263323423153"
        val ao = optimizeBank(a, 12)
        assertEquals(12, ao.length)
    }

    @Test
    fun test() {
        val answer = solution("03-1")
        assertEquals(357, answer.first)
        assertEquals(3121910778619L, answer.second)
    }

    @Test
    fun solution() {
        val answer = solution("03")
        assertEquals(17244, answer.first)
        assertEquals(171435596092638L, answer.second)
    }
}