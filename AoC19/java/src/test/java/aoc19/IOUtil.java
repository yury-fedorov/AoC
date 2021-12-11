package aoc19;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

public abstract class IOUtil {
    public static List<String> input(String day ) { return inputByPath(day + ".txt"); }
    public static List<String> inputByPath(String file ) {
        try {
            final var rootDir = System.getProperty("user.dir");
            var path = Paths.get( rootDir ).toAbsolutePath();
            while ( true ) {
                var path1 = path.getParent();
                if ( !path1.toString().contains("AoC19") ) break;
                path = path1;
            }
            return Files.readAllLines( Path.of( path.toString(),"data", file ) );
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + file, e );
        }
    }
}

