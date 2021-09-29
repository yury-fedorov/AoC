package aoc19;

import org.junit.Assert;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public abstract class IOUtil {
    public static List<String> input(String day ) { return inputByPath(day + ".txt"); }
    public static List<String> inputByPath(String file ) {
        try {
            return Files.readAllLines( Path.of( "/Users/fedoy/projects/AoC/AoC19/data", file ) );
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + file, e );
        }
    }
}

