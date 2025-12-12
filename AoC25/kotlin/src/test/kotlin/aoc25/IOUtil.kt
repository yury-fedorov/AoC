package aoc25

import java.io.IOException
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths

object IOUtil {
    fun input(day: String): List<String> = inputByPath("$day.txt")

    fun inputByPath(file: String): List<String> {
        try {
            val rootDir = System.getProperty("user.dir")
            var path = Paths.get(rootDir).toAbsolutePath()
            if (path.toString().contains("AoC25")) {
                // Out of container
                while (true) {
                    val path1 = path.parent
                    if (!path1.toString().contains("AoC25")) break
                    path = path1
                }
            } else {
                // Dockerized path
                path = Path.of("/usr/src/aoc")
            }
            return Files.readAllLines(Path.of(path.toString(), "data", file))
        } catch (e: IOException) {
            throw RuntimeException("Error reading file: $file", e)
        }
    }

    const val isRunSlow = false
}
