package aoc25

import kotlin.math.abs
import kotlin.math.round
import kotlin.test.Test
import kotlin.test.assertEquals

class Day10Test {

    typealias Button = List<Int>

    data class Machine(val goal: String, val buttons: List<Button>, val numbers: List<Int>)

    fun parse(line: String): Machine {
        val goalRegex = Regex("^\\[(.*?)\\]")
        val buttonsRegex = Regex("\\(([^)]+)\\)")
        val numbersRegex = Regex("\\{([^}]+)\\}$")
        val goalString = goalRegex.find(line)?.groupValues?.get(1)
        val buttons = buttonsRegex.findAll(line)
            .map { r -> r.groupValues[1].split(',').map { it.trim().toInt() } }.toList()
        val numbersMatch = numbersRegex.find(line)
        val numbersContent = numbersMatch?.groupValues?.get(1)
        val numbers = numbersContent?.split(',')?.map { it.trim().toInt() }
        return Machine(goal = goalString!!, buttons = buttons, numbers = numbers!!)
    }

    // Part 1: Fast Bitmask BFS
    fun shortest(machine: Machine): Long {
        val n = machine.goal.length
        var targetMask = 0
        for (i in 0 until n) {
            if (machine.goal[i] == '#') targetMask = targetMask or (1 shl i)
        }
        val buttonMasks = machine.buttons.map { b ->
            var m = 0
            for (i in b) if (i < n) m = m or (1 shl i)
            m
        }

        val queue = ArrayDeque<Pair<Int, Int>>()
        val visited = BooleanArray(1 shl n)
        queue.add(0 to 0)
        visited[0] = true

        while (queue.isNotEmpty()) {
            val (mask, dist) = queue.removeFirst()
            if (mask == targetMask) return dist.toLong()
            for (bm in buttonMasks) {
                val next = mask xor bm
                if (!visited[next]) {
                    visited[next] = true
                    queue.add(next to dist + 1)
                }
            }
        }
        return 0L
    }

    // Part 2: Gaussian Elimination + Free Variable Search
    fun solvePart2(machine: Machine): Long {
        val m = machine.numbers.size
        val k = machine.buttons.size
        val mat = Array(m) { DoubleArray(k + 1) }

        for (j in 0 until k) {
            for (counter in machine.buttons[j]) {
                if (counter < m) mat[counter][j] = 1.0
            }
        }
        for (i in 0 until m) mat[i][k] = machine.numbers[i].toDouble()

        var row = 0
        val pivotCol = IntArray(m) { -1 }
        val isPivotCol = BooleanArray(k)

        for (col in 0 until k) {
            if (row >= m) break
            var maxRow = row
            for (r in row + 1 until m) {
                if (abs(mat[r][col]) > abs(mat[maxRow][col])) maxRow = r
            }
            if (abs(mat[maxRow][col]) < 1e-9) continue

            val tmp = mat[row]
            mat[row] = mat[maxRow]
            mat[maxRow] = tmp

            val pivotVal = mat[row][col]
            for (c in col..k) mat[row][c] /= pivotVal

            for (r in 0 until m) {
                if (r != row && abs(mat[r][col]) > 1e-9) {
                    val factor = mat[r][col]
                    for (c in col..k) mat[r][c] -= factor * mat[row][c]
                }
            }
            pivotCol[row] = col
            isPivotCol[col] = true
            row++
        }

        val rank = row
        for (r in rank until m) {
            if (abs(mat[r][k]) > 1e-6) return 0L
        }

        val freeCols = (0 until k).filter { !isPivotCol[it] }
        val maxVal = IntArray(k) { j ->
            machine.buttons[j].filter { it < m }.minOfOrNull { machine.numbers[it] } ?: 0
        }

        var bestSum = Long.MAX_VALUE

        fun searchFree(idx: Int, freeVals: IntArray) {
            if (idx == freeCols.size) {
                var currentSum = freeVals.sum().toLong()
                if (currentSum >= bestSum) return

                for (r in 0 until rank) {
                    var v = mat[r][k]
                    for (i in freeCols.indices) {
                        v -= mat[r][freeCols[i]] * freeVals[i]
                    }
                    val rounded = round(v).toLong()
                    if (abs(v - rounded) > 1e-6 || rounded < 0) return
                    currentSum += rounded
                    if (currentSum >= bestSum) return
                }
                bestSum = minOf(bestSum, currentSum)
                return
            }

            val col = freeCols[idx]
            val limit = maxVal[col]
            for (v in 0..limit) {
                freeVals[idx] = v
                searchFree(idx + 1, freeVals)
            }
        }

        searchFree(0, IntArray(freeCols.size))
        return if (bestSum == Long.MAX_VALUE) 0L else bestSum
    }

    fun solution(data: String): Pair<Long, Long> {
        val list = IOUtil.input(data).filter { it.isNotBlank() }.map { parse(it) }
        var answer1 = 0L
        var answer2 = 0L
        for (machine in list) {
            answer1 += shortest(machine)
            answer2 += solvePart2(machine)
        }
        return answer1 to answer2
    }

    @Test
    fun test() {
        val result = solution("10-1")
        assertEquals(7L, result.first)
        assertEquals(33L, result.second)
    }

    @Test
    fun solution() {
        val result = solution("10")
        assertEquals(432L, result.first)
        assertEquals(18011L, result.second)
    }
}