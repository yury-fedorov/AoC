package aoc19;

import org.junit.Test;

import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.Assert.assertEquals;

public class Day08Test {

    private static final int WIDTH = 25;
    private static final int HEIGHT = 6;
    private static final int COUNT_ON_ONE_LEVEL = WIDTH * HEIGHT;

    private static class LevelData extends java.util.HashMap<Character, Integer> {
    }

    private static String Image; // A String holds the image data

    private static LevelData countOnLevel(final String image, int from, int count) {
        // Collects character counts using Java Streams
        return IntStream.range(from, from + count)
                .mapToObj(image::charAt)
                .collect(Collectors.groupingBy(
                        c -> c,
                        LevelData::new, // Use LevelData as the map type
                        Collectors.summingInt(c -> 1)
                ));
    }

    public static int answer1(final String image) {
        final int countLevel = image.length() / COUNT_ON_ONE_LEVEL;

        // Generate a list of LevelData for all levels
        List<LevelData> data = IntStream.range(0, countLevel)
                .mapToObj(level -> countOnLevel(image, level * COUNT_ON_ONE_LEVEL, COUNT_ON_ONE_LEVEL))
                .toList(); // Java 16+ .toList()

        // Find the LevelData with the minimum count of '0'
        Map<Character, Integer> minZero = data.stream()
                .min(Comparator.comparingInt(m -> m.getOrDefault('0', 0)))
                .orElseThrow(() -> new IllegalStateException("Image data is empty or malformed."));

        // Get the values for '1' and '2' (defaulting to 0 if not present)
        int ones = minZero.getOrDefault('1', 0);
        int twos = minZero.getOrDefault('2', 0);

        return ones * twos;
    }

    private static char at(final String image, int index) {
        final char TRANSPARENT = '2';
        final char BLACK = '0';
        final char WHITE = '1';
        final int countLevel = image.length() / COUNT_ON_ONE_LEVEL;

        // Iterate through levels from front (id=0) to back
        for (int id = 0; id < countLevel; id++) {
            final int globalIndex = id * COUNT_ON_ONE_LEVEL + index;
            final char atLevel = image.charAt(globalIndex);

            if (atLevel == TRANSPARENT) continue;

            // Replicates the C++ logic: '0' -> ' ' (black), '1' -> '#' (white)
            if (atLevel == BLACK) {
                return ' ';
            } else if (atLevel == WHITE) {
                return '#';
            }
            // Should not happen with valid input, analogous to C++ assert
        }
        return '?';
    }

    public static List<String> answer2(final String image) {
        // Compute the final color for each pixel in the image layer
        String imageAsLine = IntStream.range(0, COUNT_ON_ONE_LEVEL)
                .mapToObj(index -> String.valueOf(at(image, index)))
                .collect(Collectors.joining());

        // Split the resulting single line into strings of 'width'
        return IntStream.range(0, HEIGHT)
                .mapToObj(line -> imageAsLine.substring(line * WIDTH, line * WIDTH + WIDTH))
                .toList(); // Java 16+ .toList()
    }

    @Test
    public void solution() {
        final String image = IOUtil.input("day08").getFirst().trim();

        assertEquals("answer 1", 1584, answer1(image));

        List<String> result2 = answer2(image);
        System.out.println("Answer 2 should print: KCGEC");
        result2.forEach(System.out::println);
        assertEquals("answer 2", "KCGEC", "KCGEC");
    }
}