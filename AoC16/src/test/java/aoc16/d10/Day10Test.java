package aoc16.d10;

import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Pattern;

public class Day10Test {
    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d10/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
    }
}
