package aoc16.common;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

public abstract class IOUtil {

    public static List<String> input(String day) {
        return inputByPath(day + "/input.txt");
    }

    public static List<String> inputByPath(String file) {
        try {
            final var rootDir = System.getProperty("user.dir");
            var path = Paths.get(rootDir).toAbsolutePath();
            if (path.toString().contains("AoC16")) {
                // Out of container
                while (true) {
                    var path1 = path.getParent();
                    if (!path1.toString().contains("AoC16")) break;
                    path = path1;
                }
            } else {
                // Dockerized path
                path = Path.of("/usr/src/aoc");
            }
            return Files.readAllLines(Path.of(path.toString(), "src/test/resources/", file));
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + file, e);
        }
    }
}