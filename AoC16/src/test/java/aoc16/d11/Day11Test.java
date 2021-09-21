package aoc16.d11;

import aoc16.common.Config;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.*;
import java.util.stream.Collectors;

public class Day11Test {
    // generator - negative, corresponding chip positive
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
        // we remove from possible options also all empty floors at down floors
        else if ( elevatorFloor <= firstNonEmptyFloor(floors) ) directions.remove( LiftDirection.DOWN );
        if ( elevatorFloor == MAX_FLOOR ) directions.remove( LiftDirection.UP );
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
                        inLift.stream().anyMatch(Day11Test::isGenerator)) continue;

                final var nextFloorAfter = new HashSet<>( nextFloorNow );
                nextFloorAfter.addAll(inLift);
                if ( !isFried( nextFloorAfter ) ) results.add( Pair.with( d, inLift ) );
            }
        }
        return results;
    }

    @Test
    public void solution() {
        // initialization: thulium - 1, plutonium - 2, strontium - 3, promethium - 4, ruthenium - 5
        var floors = initFloors( List.of(-1, 1, -2, -3), List.of(2, 3), List.of(-4, 4, -5, 5) );
        Assert.assertEquals( "answer 1", 31, solve(floors) );
        if ( Config.isFast() ) return; // 521 seconds
        final var floor0 = new ArrayList<>( floors.get(0) );
        // elerium - 6, dilithium - 7
        floor0.addAll( List.of( 6, -6, 7, -7 ) );
        final var floors2 = new ArrayList<>(floors);
        floors2.set(0, floor0 );
        Assert.assertEquals( "answer 2", 55, solve(floors2) );
    }

    static Optional<Pair<Integer, List< Collection<Integer>>>> next(
            Pair< Integer, List< Collection<Integer>> > now,
            Pair< LiftDirection, Collection<Integer> > option,
            Set<String> history ) {
        final int curFloor = now.getValue0();
        final var state = now.getValue1();
        final var nextFloor = nextFloor( curFloor, option.getValue0() );
        final var newState = move( state, curFloor, nextFloor, option.getValue1() );
        if ( !history.contains( print( newState, nextFloor ) ) ) // we do not repeat the history
            return Optional.of( Pair.with( nextFloor, newState ) );
        return Optional.empty();
    }

    private int solve(List<Collection<Integer>> floors) {
        var paths = Set.of( Pair.with( 0, floors) );
        int step = 0;
        final var history = new HashSet<String>();
        while ( !paths.isEmpty() ) {
            final var paths1 = new HashSet< Pair<Integer, List< Collection<Integer>>> >();
            for ( final var path : paths ) {
                final int elevator = path.getValue0();
                final var state = path.getValue1(); // all flows

                paths1.addAll( options( state, elevator ).stream().map( (o) -> next( path, o, history ) )
                        .filter(Optional::isPresent).map(Optional::get).collect( Collectors.toList() ) );

                history.add( print( state, elevator ) );
            }
            step++;
            if (paths1.parallelStream()
                    .anyMatch( (p) -> p.getValue0() == MAX_FLOOR && firstNonEmptyFloor( p.getValue1() ) == MAX_FLOOR ))
                break;
            Assert.assertFalse( "failed to find a solution: " + step, paths1.isEmpty() );
            paths = paths1;
        }
        return step;
    }

    static List<Collection<Integer>> initFloors(List<Integer> floor1, List<Integer> floor2, List<Integer> floor3) {
        return List.of( floor1, floor2, floor3, Collections.emptyList() );
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
                Assert.assertEquals("check", Math.abs(f.size() - fs.size()), elevator.size());
            result.add( fs );
        }
        return result;
    }
}