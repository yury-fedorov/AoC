package aoc25

import kotlin.math.max
import kotlin.test.Test
import kotlin.test.assertEquals

class Day03Test {

    fun largestVoltage(bank: String): Int {
        var result = 0
        for (a in 0..bank.length - 2) {
            for (b in a + 1..bank.length - 1) {
                result = max(result, "${bank[a]}${bank[b]}".toInt())
            }
        }
        return result
    }

    @Test
    fun largestTest() {
        assertEquals(98, largestVoltage("987654321111111"))
        assertEquals(89, largestVoltage("811111111111119"))
    }

    fun largestVoltage2(bank: String): Long {
        var result = 0L
        val n = bank.length - 1
        for (i1 in 0..n - 11) {
            for (i2 in i1..n - 10) {
                for (i3 in i2..n - 9) {
                    for (i4 in i3..n - 8) {
                        for (i5 in i4..n - 7) {
                            for (i6 in i5..n - 6) {
                                for (i7 in i6..n - 5) {
                                    for (i8 in i7..n - 4) {
                                        for (i9 in i8..n - 3) {
                                            for (i10 in i9..n - 2) {
                                                for (i11 in i10..n - 1) {
                                                    for (i12 in i11..n) {
                                                        val seq =
                                                            listOf(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12)
                                                        if (!isIndexesOk(seq)) continue
                                                        val newResult =
                                                            seq.map { i -> bank[i] }.toCharArray().joinToString("")
                                                                .toLong()
                                                        result = max(result, newResult)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return result
    }

    fun isIndexesOk(list: List<Int>): Boolean =
        list.zip(list.drop(1)).all { p -> p.first < p.second }

    fun solution(case: String): Pair<Int, Long> {
        val list = IOUtil.input(case)
        val answer1 = list.sumOf { bank -> largestVoltage(bank) }
        val answer2 = list.sumOf { bank -> largestVoltage2(bank) }
        return answer1 to answer2
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
        assertEquals(0, answer.second)
    }
}