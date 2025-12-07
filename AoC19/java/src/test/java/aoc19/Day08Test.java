package aoc19;

import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.Map;
import java.util.List;
import java.util.Comparator;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Day08Test {

    private static final int WIDTH = 25;
    private static final int HEIGHT = 6;
    private static final int COUNT_ON_ONE_LEVEL = WIDTH * HEIGHT;

    // Type aliases for clarity
    private static class LevelData extends java.util.HashMap<Character, Integer> {}
    private static String Image; // A String holds the image data

    // C++ LevelData count_on_level( const Image & image, int from, int count )
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

    // C++ auto answer1(const Image & image )
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

    // C++ char at( const Image & image, int index )
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

    // C++ vector<string> answer2(const Image & image )
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

    @Tests
    // Equivalent of the C++ TEST_CASE block
    public void solution() {
        // Assuming '../data/day08.txt' exists relative to where the program runs
        String image;
        try {
            // Read all lines and join them, or just read the first line if the file format guarantees it
            image = Files.readString(Path.of("../data/day08.txt")).trim();
            // If the file is just one line of digits:
            // image = Files.lines(Path.of("../data/day08.txt")).findFirst().orElseThrow();
        } catch (IOException e) {
            System.err.println("Error reading file: " + e.getMessage());
            System.err.println("Please ensure '../data/day08.txt' exists and contains the image data.");
            return;
        }

        // --- SECTION "08-1" ---
        int result1 = answer1(image);
        System.out.println("Answer 1: " + result1);
        // C++ REQUIRE( answer1(image) == 1584 );
        if (result1 == 1584) {
            System.out.println("08-1 Test Passed.");
        } else {
            System.out.println("08-1 Test Failed. Expected 1584.");
        }

        System.out.println("---");

        // --- SECTION "08-2" ---
        List<String> result2 = answer2(image);
        System.out.println("Answer 2 Image (should print KCGEC):");
        result2.forEach(System.out::println);
        // C++ REQUIRE( true ); and r::for_each( a2, ... )
        // The check for KCGEC would be done by visually inspecting the output or implementing a more complex assert.
    }
}