package aoc16.d08;
import org.javatuples.Pair;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashSet;

public class Day08Test {
    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d08/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        final var screen = new HashSet<Pair<Integer,Integer>>(); // only on are there
        assertEquals("answer 1", -1, screen.size() );
    }
}
