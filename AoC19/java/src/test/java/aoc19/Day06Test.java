package aoc19;

import org.junit.Test;

import java.util.*;


public class Day06Test {

    public static class Day06 {
        // Data is represented as a Map: Child -> Parent, to quickly find the parent of any object.
        private final Map<String, String> data;
        private static final String COM = "COM";

        // Cache for total orbits (depth) of an object
        private final Map<String, Integer> orbitCountCache = new HashMap<>();

        public Day06(List<String> orbitPairs) {
            this.data = new HashMap<>();
            for (String pair : orbitPairs) {
                String[] parts = pair.split("\\)");
                if (parts.length == 2) {
                    // Parent -> Child structure from the file, stored as Child -> Parent in the map
                    data.put(parts[1], parts[0]);
                }
            }
        }

        /**
         * Calculates the total number of direct and indirect orbits for a given object.
         * Implements caching (memoization) to improve performance.
         */
        private int count(String obj) {
            if (orbitCountCache.containsKey(obj)) {
                return orbitCountCache.get(obj);
            }

            int result = 0;
            // Check if the object has a parent
            if (data.containsKey(obj)) {
                // result = 1 (for the direct orbit) + total orbits of its parent
                String parent = data.get(obj);
                result = 1 + count(parent);
            }

            orbitCountCache.put(obj, result);
            return result;
        }

        /**
         * Answer 1: Calculates the total number of direct and indirect orbits for all objects.
         */
        public int answer1() {
            // Get all unique objects (keys and values in the map)
            Set<String> allObjects = new HashSet<>(data.keySet());
            allObjects.addAll(data.values());

            // Stream all objects, calculate the count for each, and sum the results
            return allObjects.stream()
                    .mapToInt(this::count)
                    .sum();
        }

        /**
         * Generates the path of objects from the given object up to COM (excluding the object itself).
         * The path is ordered from the closest parent to the furthest parent (COM's child).
         */
        private List<String> getPath(String obj) {
            List<String> path = new ArrayList<>();
            String current = obj;

            // Loop until we reach COM, which has no parent in the map.
            while (data.containsKey(current)) {
                String parent = data.get(current);
                if (parent.equals(COM)) {
                    // COM is the root and is not included in the 'path' as a jump target
                    // from the initial C++ logic (it stops when current == COM)
                    break;
                }
                path.add(parent);
                current = parent;
            }
            return path;
        }

        /**
         * Answer 2: Calculates the minimum number of orbital transfers required to move
         * from YOU to SAN (you don't count the object you are on or the object you are moving to).
         */
        public int answer2() {
            List<String> youPath = getPath("YOU");
            List<String> sanPath = getPath("SAN");

            // Reverse the paths to start from the root (COM's child) for easier common ancestor finding
            Collections.reverse(youPath);
            Collections.reverse(sanPath);

            int commonPartLength = 0;
            int minLength = Math.min(youPath.size(), sanPath.size());

            // Find the length of the common part of the path, starting from the root
            for (int i = 0; i < minLength; i++) {
                if (youPath.get(i).equals(sanPath.get(i))) {
                    commonPartLength++;
                } else {
                    break;
                }
            }

            // The total transfers is (distance from YOU to common ancestor) + (distance from SAN to common ancestor).
            // The common ancestor is at index (commonPartLength - 1) in the reversed list.
            // The formula is: (Total length of YOU path) + (Total length of SAN path) - (2 * common ancestor distance)
            return (youPath.size() - commonPartLength) + (sanPath.size() - commonPartLength);
        }
    }

    @Test
    public void solution() {
        // --- Setup (Equivalent to the C++ TEST_CASE loading data) ---
        List<String> data = IOUtil.input("Day06");
        // Assuming the file content is a list of "AAA)BBB" lines.

        if (data.isEmpty()) {
            System.out.println("Could not load data. Exiting.");
            return;
        }

        Day06 solver = new Day06(data);

        // --- SECTION "06-1" ---
        int result1 = solver.answer1();
        int expected1 = 312697; // C++ Requirement

        System.out.println("--- Answer 1 ---");
        System.out.println("Calculated: " + result1);
        System.out.println("Expected: " + expected1);
        System.out.println("Result: " + (result1 == expected1 ? "PASS" : "FAIL"));

        System.out.println("\n----------------\n");

        // --- SECTION "06-2" ---
        int result2 = solver.answer2();
        int expected2 = 466; // C++ Requirement

        System.out.println("--- Answer 2 ---");
        System.out.println("Calculated: " + result2);
        System.out.println("Expected: " + expected2);
        System.out.println("Result: " + (result2 == expected2 ? "PASS" : "FAIL"));
    }
}