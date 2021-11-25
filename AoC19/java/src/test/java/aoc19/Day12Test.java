package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day12Test {
    // records cannot be inhereted but a lot of functionality further are similar, common abstraction is wanted
    record Position( int x, int y, int z){}
    record Velocity( int x, int y, int z){}
    class Moon { 
        Position position; 
        Velocity velocity; 
        Moon(Position p, Velocity v) { position = p; velocity = v; } 
    }
    static int dv( int a, int b ) { return (int)Math.signum( b - a ); }
    static Velocity velocity( Position a, Position b ) { 
        return new Velocity( dv(a.x, b.x), dv(a.y, b.y), dv(a.z, b.z) ); 
    }
    static Velocity sum( Velocity a, Velocity b ) { 
        return new Velocity( a.x + b.x, a.y + b.y, a.z + b.z ); 
    }
    static Velocity inverse( Velocity v ) { return new Velocity( -v.x, -v.y, -v.z ); }
    static Position move( Position a, Velocity b ) { 
        return new Position( a.x + b.x, a.y + b.y, a.z + b.z ); 
    }
    static long totalSystemEnergy(List<Moon> moons) {
        return moons.stream().mapToLong( Day12Test::totalMoonEnergy ).sum();
    }
    static long totalMoonEnergy(Moon m) {
        var potentialEnergy = Math.abs(m.position.x) + Math.abs(m.position.y) + Math.abs(m.position.z);
        var kineticEnergy = Math.abs(m.velocity.x) + Math.abs(m.velocity.y) + Math.abs(m.velocity.z);
        return potentialEnergy * kineticEnergy;
    }

    static final int [][] DEMO = { {-1,0,2}, {2,-10,-7}, {4,-8,8}, {3,5,-1} };
    static final int [][] TASK = { {6,-2,-7}, {-6,-7,-4}, {9,11, 0}, {-3,-4, 6} };

    static final Velocity VELOCITY_ZERO = new Velocity(0,0,0);

    static List<Moon> init( int [][] data ) {
        return Stream.of( data ).map( p -> new Moon(new Position( p[0], p[1], p[2]), VELOCITY_ZERO ) )
            .collect(Collectors.toList());
    }

    @Test
    public void solution() {

        final var moons = init(DEMO);
        final var STEPS = 10; // 1_000;
        for ( var step = 0; step < 10; step++ ) {
            for ( int i = 0; i < moons.size(); i++ ) {
                for ( int j = i + 1; j < moons.size(); j++ ) {
                    var a = moons.get(i);
                    var b = moons.get(j);
                    var dv = velocity(a.position, b.position);
                    a.velocity = sum( a.velocity, dv ); // no way
                    b.velocity = sum( b.velocity, inverse(dv)); // no way
                }
            }
    
            for ( var m : moons ) {
                m.position = move( m.position, m.velocity );
            }
        }

        assertEquals( "answer 1", 0, totalSystemEnergy(moons) ); // 4967 - too low
        assertEquals( "answer 2", 0, 2 );
    }
}
