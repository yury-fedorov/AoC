package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day10Test {

    typealias Button = List<Int>

    data class Machine(val goal: String, val buttons: List<Button>, val numbers: List<Int>) {}

    fun parse(line: String): Machine {
        val goalRegex = Regex("^\\[(.*?)\\]")
        val buttonsRegex = Regex("\\(([^)]+)\\)")
        val numbersRegex = Regex("\\{([^}]+)\\}$")
        val goalString = goalRegex.find(line)?.groupValues?.get(1)
        val buttons = buttonsRegex.findAll(line)
            .map { r -> r.groupValues[1].split(',').map { it.toInt() } }.toList()
        val numbersMatch = numbersRegex.find(line)
        val numbersContent = numbersMatch?.groupValues?.get(1)
        val numbers = numbersContent?.split(',')?.map { it.toInt() }
        return Machine(goal = goalString!!, buttons = buttons, numbers = numbers!!)
    }

    fun init(target: String): String = ".".repeat(target.length)

    fun push(start: String, button: Button): String {
        val state = start.toCharArray()
        button.forEach { i ->
            state[i] = if (state[i] == '.') {
                '#'
            } else {
                '.'
            }
        }
        return String(state)
    }

    fun pushSequence(init: String, machine: Machine, sequence: Collection<Int>): String {
        var state = init
        for (b in sequence.map { i -> machine.buttons[i] }) state = push(state, b)
        return state
    }
    /*
        fun shortest1(machine: Machine, state: String, pushesDone: Long, pushesToDo: Long): Long? {
            val states = mutableSetOf(state)
            for ( i in 0 .. pushesToDo ) {
                val next = mutableSetOf<String>()
                for (b in machine.buttons) {
                    val newState = push(state, b)
                    if (newState === machine.goal) return pushesDone + 1
                    if (!states.contains(newState) )
                        next += newState
                }
            }
            return next.map { state: String -> shortest(machine, state, pushesDone + 1, pushesToDo - 1) }
                .filter { it != null }.minOfOrNull { it!! }
        }
    */

    fun shortest(machine: Machine, state: String, pushesDone: Long, pushesToDo: Long): Long? {
        if (pushesToDo <= 0L) return null
        val next = mutableSetOf<String>()
        for (b in machine.buttons) {
            val newState = push(state, b)
            if (newState === machine.goal) return pushesDone + 1
            next += newState
        }
        return next.map { state: String -> shortest(machine, state, pushesDone + 1, pushesToDo - 1) }
            .filter { it != null }.minOfOrNull { it!! }
    }

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).map { l -> parse(l) }
        var answer1 = 0L
        val maxPushes = 5L
        for (machine in list) {
            val r = shortest(machine, init(machine.goal), 0L, maxPushes)
            if (r == null) {
                println("For ${machine.goal} $maxPushes pushes are not enough")
            }
            answer1 += r!!
        }
        return answer1 to 0L
    }

    @Test
    fun testSequence() {
        val machine = parse(IOUtil.input("10-1").first())
        val state = pushSequence(init(machine.goal), machine, listOf(0, 1, 2))
        assertEquals(machine.goal, state)
    }

    // @Test
    fun test() {
        val result = solution("10-1")
        assertEquals(7L, result.first) // TODO
        assertEquals(0L, result.second)
    }

    // @Test
    fun solution() {
        val result = solution("10")
        assertEquals(0L, result.first) // TODO
        assertEquals(0L, result.second)
    }
}