package aoc16.common;

import org.junit.Assert;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public abstract class IOUtil {

    public static List<String> input(String day ) { return  inputByPath(day + "/input.txt"); }

    public static List<String> inputByPath(String path ) {
        final var url = IOUtil.class.getClassLoader().getResource( path );
        Assert.assertNotNull(url);
        try {
            return Files.readAllLines( Path.of(url.getPath()) );
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + path, e );
        }
    }
}