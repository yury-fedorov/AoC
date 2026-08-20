package aoc25

import kotlin.test.Test
import kotlin.test.assertEquals

class Day12Test {

    data class Shape(val id: Int, val orientations: List<List<Pair<Int, Int>>>, val cellCount: Int)
    data class Region(val w: Int, val h: Int, val counts: List<Int>)

    private fun rotate(g: Array<BooleanArray>): Array<BooleanArray> {
        val h = g.size
        val w = g[0].size
        val r = Array(w) { BooleanArray(h) }
        for (y in 0 until h) {
            for (x in 0 until w) {
                r[x][h - 1 - y] = g[y][x]
            }
        }
        return r
    }

    private fun flip(g: Array<BooleanArray>): Array<BooleanArray> {
        val h = g.size
        val w = g[0].size
        val r = Array(h) { BooleanArray(w) }
        for (y in 0 until h) {
            for (x in 0 until w) {
                r[y][w - 1 - x] = g[y][x]
            }
        }
        return r
    }

    private fun getCoords(g: Array<BooleanArray>): List<Pair<Int, Int>> {
        val list = mutableListOf<Pair<Int, Int>>()
        var minX = Int.MAX_VALUE
        var minY = Int.MAX_VALUE
        for (y in g.indices) {
            for (x in g[0].indices) {
                if (g[y][x]) {
                    minX = minOf(minX, x)
                    minY = minOf(minY, y)
                }
            }
        }
        for (y in g.indices) {
            for (x in g[0].indices) {
                if (g[y][x]) {
                    list.add(x - minX to y - minY)
                }
            }
        }
        return list
    }

    private fun generateOrientations(base: Array<BooleanArray>): List<List<Pair<Int, Int>>> {
        val seen = mutableSetOf<String>()
        val result = mutableListOf<List<Pair<Int, Int>>>()
        var cur = base
        for (f in 0..1) {
            for (rot in 0..3) {
                val coords = getCoords(cur)
                val key = coords.joinToString(";") { "${it.first},${it.second}" }
                if (seen.add(key)) {
                    result.add(coords)
                }
                cur = rotate(cur)
            }
            cur = flip(base)
        }
        return result
    }

    private fun parse(lines: List<String>): Pair<List<Shape>, List<Region>> {
        val shapes = mutableListOf<Shape>()
        val regions = mutableListOf<Region>()
        var i = 0
        while (i < lines.size) {
            val line = lines[i].trim()
            if (line.matches("^\\d+:$".toRegex())) {
                val id = line.removeSuffix(":").toInt()
                i++
                val shapeLines = mutableListOf<String>()
                while (i < lines.size && lines[i].isNotBlank()) {
                    shapeLines.add(lines[i].trim())
                    i++
                }
                val sh = shapeLines.size
                val sw = shapeLines[0].length
                val grid = Array(sh) { y -> BooleanArray(sw) { x -> shapeLines[y][x] == '#' } }
                val orientations = generateOrientations(grid)
                shapes.add(Shape(id, orientations, orientations[0].size))
            } else if (line.contains("x")) {
                val parts = line.split(":")
                val (w, h) = parts[0].trim().split("x").map { it.toInt() }
                val counts = parts[1].trim().split("\\s+".toRegex()).map { it.toInt() }
                regions.add(Region(w, h, counts))
                i++
            } else {
                i++
            }
        }
        return shapes to regions
    }

    private fun canFit(r: Region, shapes: List<Shape>): Boolean {
        var totalPresents = 0
        var totalAreaNeeded = 0
        for (i in r.counts.indices) {
            totalPresents += r.counts[i]
            totalAreaNeeded += r.counts[i] * shapes[i].cellCount
        }

        // 1. Total area bound
        if (totalAreaNeeded > r.w * r.h) return false

        // 2. Disjoint 3x3 block capacity bound
        if (totalPresents <= (r.w / 3) * (r.h / 3)) return true

        // 3. Backtracking for interlocking configurations (sample grids)
        val presentShapeIds = mutableListOf<Int>()
        for (i in r.counts.indices) {
            repeat(r.counts[i]) { presentShapeIds.add(i) }
        }

        val grid = Array(r.h) { BooleanArray(r.w) }
        return backtrack(0, presentShapeIds, grid, shapes, r.w, r.h)
    }

    private fun backtrack(
        pIdx: Int,
        presentShapeIds: List<Int>,
        grid: Array<BooleanArray>,
        shapes: List<Shape>,
        w: Int,
        h: Int
    ): Boolean {
        if (pIdx == presentShapeIds.size) return true
        val sId = presentShapeIds[pIdx]
        val shape = shapes[sId]

        for (orient in shape.orientations) {
            val ow = orient.maxOf { it.first } + 1
            val oh = orient.maxOf { it.second } + 1
            for (y in 0..h - oh) {
                for (x in 0..w - ow) {
                    if (canPlace(grid, orient, x, y)) {
                        place(grid, orient, x, y, true)
                        if (backtrack(pIdx + 1, presentShapeIds, grid, shapes, w, h)) return true
                        place(grid, orient, x, y, false)
                    }
                }
            }
        }
        return false
    }

    private fun canPlace(grid: Array<BooleanArray>, orient: List<Pair<Int, Int>>, startX: Int, startY: Int): Boolean {
        for (c in orient) {
            if (grid[startY + c.second][startX + c.first]) return false
        }
        return true
    }

    private fun place(grid: Array<BooleanArray>, orient: List<Pair<Int, Int>>, startX: Int, startY: Int, v: Boolean) {
        for (c in orient) {
            grid[startY + c.second][startX + c.first] = v
        }
    }

    fun solution(data: String): Pair<Long, Long> {
        val lines = IOUtil.input(data)
        val (shapes, regions) = parse(lines)
        val fitCount = regions.count { canFit(it, shapes) }
        return fitCount.toLong() to 0L
    }

    @Test
    fun test() {
        val (answer, _) = solution("12-1")
        assertEquals(2L, answer)
    }

    @Test
    fun solution() {
        val (answer, _) = solution("12")
        assertEquals(490L, answer)
    }
}