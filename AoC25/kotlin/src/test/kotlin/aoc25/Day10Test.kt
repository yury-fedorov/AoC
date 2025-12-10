package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day10Test {

    typealias Button = List<Int>

    data class Machine(val start: String, val buttons: List<Button>, val numbers: List<Int>) {}

    fun parse(line: String): Machine {
        val startRegex = Regex("^\\[(.*?)\\]")
        val buttonsRegex = Regex("\\(([^)]+)\\)")
        val numbersRegex = Regex("\\{([^}]+)\\}$")
        val startMatch = startRegex.find(line)
        val startString = startMatch?.groupValues?.get(1)
        val buttons = buttonsRegex.findAll(line)
            .map { r -> r.groupValues.get(1).split(',').map { it.toInt() } }.toList()
        val numbersMatch = numbersRegex.find(line)
        val numbersContent = numbersMatch?.groupValues?.get(1)
        val numbers = numbersContent?.split(',')?.map { it.toInt() }
        return Machine(start = startString!!, buttons = buttons, numbers = numbers!!)
    }

    fun shortest(machine: Machine): Long = 0L

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).map { l -> parse(l) }
        var answer1 = 0L
        for (machine in list) {
            answer1 += shortest(machine)
        }
        return 0L to 0L
    }

    @Test
    fun test() {
        val result = solution("10-1")
        assertEquals(7L, result.first) // TODO
        assertEquals(0L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("10")
        assertEquals(0L, result.first) // TODO
        assertEquals(0L, result.second)
    }
}