package aoc25

import kotlin.math.max
import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.time.measureTime

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

    fun optimizeBank(bank: String): String {
        val minLength = 12
        var reducedBank = bank
        var freqMap = bank.groupingBy { it }.eachCount()
        while (true) {
            val count = freqMap.values.sum()
            val smallest = freqMap.keys.min()
            val smallestCount = freqMap[smallest]!!
            if ((count - smallestCount) <= minLength) {
                break
            }
            freqMap = freqMap.filter { p -> p.key != smallest }
            reducedBank = reducedBank.filter { c -> c != smallest }
        }
        // now we may reduce lowest from beginning
        val smallest = reducedBank.min()
        while (reducedBank.length > minLength ){
            val i = reducedBank.indexOf(smallest)
            // reducedBank = reducedBank.take(i) + reducedBank.substring(i+1)
            if ( i != 0 ) break
            reducedBank = reducedBank.drop(1)
        }
        return reducedBank
    }

    // optimized by performance
    fun largestVoltage21(bank: String): Long = largestVoltage2(optimizeBank(bank))

    // super slow
    fun largestVoltage2(bank: String): Long {
        var result = 0L
        var resultString = "000000000000"
        val n = bank.length - 1
        for (i1 in 0..n - 11) {
            if (bank[i1] <= resultString[0]) continue
            for (i2 in i1 + 1..n - 10) {
                for (i3 in i2 + 1..n - 9) {
                    for (i4 in i3 + 1..n - 8) {
                        for (i5 in i4 + 1..n - 7) {
                            for (i6 in i5 + 1..n - 6) {
                                for (i7 in i6 + 1..n - 5) {
                                    for (i8 in i7 + 1..n - 4) {
                                        for (i9 in i8 + 1..n - 3) {
                                            for (i10 in i9 + 1..n - 2) {
                                                for (i11 in i10 + 1..n - 1) {
                                                    for (i12 in i11 + 1..n) {
                                                        val seq =
                                                            listOf(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12)
                                                        // if (!isIndexesOk(seq)) continue
                                                        val newResultString =
                                                            seq.map { i -> bank[i] }.toCharArray().joinToString("")
                                                        val newResult = newResultString.toLong()
                                                        if (result < newResult) {
                                                            result = newResult
                                                            resultString = newResultString
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
        }
        return result
    }

    fun isIndexesOk(list: List<Int>): Boolean =
        list.zip(list.drop(1)).all { p -> p.first < p.second }

    fun solution(case: String): Pair<Int, Long> {
        val list = IOUtil.input(case)
        val answer1 = list.sumOf { bank -> largestVoltage(bank) }
        // val answer2 = list.sumOf { bank -> largestVoltage21(bank) }
        var answer2 = 0L
        for (bank in list) {
            println(bank)
            val timeTaken = measureTime {
                val da2 = largestVoltage21(bank)
                answer2 += da2
            }
            println(timeTaken)
        }
        return answer1 to answer2
    }

    @Test
    fun testTough() {
        val a = "2221332232313335223223326323322224151322223433133252223132133333322233214222322312232523263323423153"
        val ao = optimizeBank(a)
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
        assertEquals(0, answer.second)
    }
}