package aoc16.d11;

import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.*;

public class Day11Test {

    static boolean isChip( int obj ) { return obj > 0; }
    static boolean isGenerator( int obj ) { return obj < 0; }
    enum LiftDirection { DOWN, UP }

    // if a chip is ever left in the same area as another RTG, and it's not connected to its own RTG, the chip will be fried
    static boolean isFried( Collection<Integer> area ) {
        final var chips = new LinkedList<Integer>();
        final var generators = new LinkedList<Integer>();
        for ( final var e : area ) {
            if ( area.contains( -e ) ) { /* we remove all paired chips and generators */ }
            else if ( isChip(e) ) chips.add( e );
            else if ( isGenerator(e) ) generators.add( e );
        }
        // now we check what is remained: we cannot have a chip and another generator
        return ( !chips.isEmpty() && !generators.isEmpty() );
    }

    final static int MAX_FLOOR = 3;

    static int firstNonEmptyFloor(List<Collection<Integer>> floors) {
        for ( int curFloor = 0; curFloor <= MAX_FLOOR; curFloor++ ) {
            if (!floors.get(curFloor).isEmpty()) return curFloor;
        }
        throw new IllegalStateException( "we will always at least one nont empty floor" );
    }

    static Collection< LiftDirection > directions( List<Collection<Integer>> floors, final int elevatorFloor ) {
        final var directions = new ArrayList<>( List.of(LiftDirection.values()));
        if ( elevatorFloor == 0 ) directions.remove( LiftDirection.DOWN );
        if ( elevatorFloor == MAX_FLOOR ) directions.remove( LiftDirection.UP );
        // we remove from possible options also all empty floors at down floors
        if ( elevatorFloor <= firstNonEmptyFloor(floors) ) directions.remove( LiftDirection.DOWN );
        return directions;
    }

    static int nextFloor( int floor, LiftDirection d ) { return floor + ( d == LiftDirection.UP ? 1 : -1 ); }


    static Collection< Pair< LiftDirection, Collection<Integer> > > options( List<Collection<Integer>> floors, final int floor ) {
        final var results = new LinkedList< Pair< LiftDirection, Collection<Integer> > >();

        final var options = new ArrayList< Collection<Integer> >(); // resource optimal
        final var floorNow = floors.get(floor);
        for ( final var a : floorNow ) {
            for ( final var b : floorNow ) {
                if ( b < a ) continue; // we need pairs one time
                final var inLift = a.equals(b) ? Set.of(a) : Set.of(a, b);
                final var floorAfter = new HashSet<>( floorNow );
                floorAfter.removeAll( inLift );
                // any combination which not fry in lift and not fry anything on current floor after lift is gone
                if ( !isFried( inLift ) && !isFried( floorAfter ) ) options.add( inLift );
            }
        }

        for ( final var d : directions( floors, floor ) ) {
            final var nextFloorNow = new HashSet<>( floors.get(  nextFloor( floor, d ) ) );
            for ( final var inLift : options ) {
                // generators are never going down
                if ( d == LiftDirection.DOWN &&
                        inLift.stream().filter( (e) -> isGenerator(e) ).findAny().isPresent() ) continue;

                final var nextFloorAfter = new HashSet<>( nextFloorNow );
                nextFloorAfter.addAll(inLift);
                if ( !isFried( nextFloorAfter ) ) results.add( Pair.with( d, inLift ) );
            }
        }
        // IMPORTANT - to check if we arrive in lift with a combination - can it fry in conjunction (lift, floor)?
        return results;
    }

    @Test
    public void solution() {
        final List<Collection<Integer>> floors = new ArrayList<>(); // generator - negative, corresponding chip positive
        if ( true ) {
            // initialization: thulium - 1, plutonium - 2, strontium - 3, promethium - 4, ruthenium - 5
            floors.add( List.of( -1, 1, -2, -3 ) );
            floors.add( List.of( 2, 3 ) );
            floors.add( List.of( -4, 4, -5, 5 ) );
            floors.add( new LinkedList<>() );
        } else {
            floors.add( List.of( 1, 2 ) ); // HM, LM
            floors.add( List.of( -2 ) ); // HG
            floors.add( List.of( -1 ) ); // LG
            floors.add( new LinkedList<>() );
        }

        var paths = Set.of( Pair.with( 0, floors ) );
        int step = 0;
        final var history = new HashMap<String, Integer>();
        while ( !paths.isEmpty() ) {
            final var paths1 = new HashSet< Pair<Integer, List< Collection<Integer>>> >();
            for ( final var path : paths ) {
                final int elevator = path.getValue0();
                final var state = path.getValue1(); // all flows

                for ( final var o : options( state, elevator ) ) {
                    final var nextFloor = nextFloor( elevator, o.getValue0() );
                    final var newState = move( state, elevator, nextFloor, o.getValue1() );
                    if ( !history.containsKey( print( newState, nextFloor ) ) ) // we do not repeat the history
                        paths1.add( Pair.with( nextFloor, newState ) );
                }
                history.put( print( state, elevator ), step );
            }
            step++;
            if ( paths1.stream()
                    .filter( (p) -> p.getValue0() == MAX_FLOOR && firstNonEmptyFloor( p.getValue1() ) == MAX_FLOOR )
                    .findAny().isPresent() )
                break;
            Assert.assertFalse( "failed to find a solution: " + step, paths1.isEmpty() );
            paths = paths1;
        }

        Assert.assertEquals( "answer 1", 31, step );
        Assert.fail( "no solution yet" );
    }

    static String print( List<Collection<Integer>> floors, int floor ) {
        final var b = new StringBuilder();
        b.append( floor ).append( '/' );
        for ( final var f : floors ) {
            final var fo = new ArrayList<>(f);
            fo.sort( Comparator.naturalOrder() );
            for ( final var e : fo ) b.append( e ).append( ' ' );
            b.append( ';' );
        }
        return b.toString();
    }

    static List<Collection<Integer>> move(
            List<Collection<Integer>> floors, int curFloor, int nextFloor, Collection<Integer> elevator) {
        final List<Collection<Integer>> result = new ArrayList<>();
        for ( int fi = 0; fi <= MAX_FLOOR; fi++ ) {
            final var f = floors.get(fi);
            final var fs = new HashSet<>( f );
            if ( curFloor == fi ) fs.removeAll( elevator );
            else if ( nextFloor == fi ) fs.addAll( elevator );
            if ( fi == curFloor || fi == nextFloor )
                Assert.assertTrue( "check",Math.abs( f.size() - fs.size() ) == elevator.size() );
            result.add( fs );
        }
        return result;
    }
}
