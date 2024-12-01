package aoc16.d10;

import aoc16.common.IOUtil;
import org.junit.Test;

import java.util.*;
import java.util.function.Function;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day10Test {

    final HashMap<Integer, PairII> botResultMap = new HashMap<>();

    static PairDI out(String d, String n) {
        return new PairDI(d.equals("bot") ? Destination.BOT : Destination.OUTPUT, Integer.parseInt(n));
    }

    static Collection<PairSI> sources(ValueMap vmap, BotMap bmap, final int bot) {
        final var result = new ArrayList<PairSI>();
        result.addAll(vmap.entrySet().stream().filter(e -> e.getValue() == bot)
                .map(e -> new PairSI(Source.VALUE, e.getKey())).collect(Collectors.toList()));
        result.addAll(bmap.entrySet().stream()
                .filter(e -> (e.getValue().a().destination() == Destination.BOT && e.getValue().a().id() == bot)
                        || (e.getValue().b().destination() == Destination.BOT && e.getValue().b().id() == bot))
                .map(e -> new PairSI(Source.BOT, e.getKey())).collect(Collectors.toList()));
        return result;
    }

    PairII botOut(ValueMap vmap, BotMap bmap, final int botId) {
        if (botResultMap.containsKey(botId)) return botResultMap.get(botId);
        final var sources = sources(vmap, bmap, botId);
        assertEquals("sources.size() == 2", 2, sources.size());
        final var outValues = new ArrayList<Integer>();
        for (final var s : sources) {
            if (s.source() == Source.VALUE) outValues.add(s.id());
            else {
                final var sourceBotId = s.id();
                final var sourceBotOut = botOut(vmap, bmap, sourceBotId);
                final var setup = bmap.get(sourceBotId);
                if (setup.a().destination() == Destination.BOT && setup.a().id() == botId) {
                    outValues.add(sourceBotOut.a());
                } else { // eventual check
                    outValues.add(sourceBotOut.b());
                }
            }
        }
        var result = (outValues.get(0) < outValues.get(1))
                ? new PairII(outValues.get(0), outValues.get(1))
                : new PairII(outValues.get(1), outValues.get(0));
        botResultMap.put(botId, result);
        return result;
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d10");

        final var patternValue = Pattern.compile("^value (\\d+) goes to bot (\\d+)$");
        final var patternBot = Pattern.compile("^bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)$");

        final var valueBotMap = new ValueMap();
        final var botOutMap = new BotMap();
        for (final var l : input) {
            var matcher = patternValue.matcher(l);
            if (matcher.find()) {
                valueBotMap.put(Integer.parseInt(matcher.group(1)), Integer.parseInt(matcher.group(2)));
            } else {
                matcher = patternBot.matcher(l);
                assertTrue("last pattern", matcher.find());
                botOutMap.put(Integer.parseInt(matcher.group(1)),
                        new PairDIDI(out(matcher.group(2), matcher.group(3)),
                                out(matcher.group(4), matcher.group(5))));
            }
        }
        final var ba = valueBotMap.get(61);
        final var bb = valueBotMap.get(17);
        final var bl = new HashSet<Integer>();
        bl.add(ba);
        bl.add(bb);
        Optional<Integer> answer1 = Optional.empty();
        final var processed = new HashSet<Integer>();
        while (!bl.isEmpty()) {
            final var curBot = bl.iterator().next();
            final var bo = botOut(valueBotMap, botOutMap, curBot);
            if (bo.a() == 17 && bo.b() == 61) {
                answer1 = Optional.of(curBot);
                break;
            }
            bl.remove(curBot);
            processed.add(curBot); // works also without it but in this way it is faster
            final var conf = botOutMap.get(curBot);
            if (conf.a().destination() == Destination.BOT) {
                final var id = conf.a().id();
                if (!processed.contains(id)) bl.add(id);
            }
            if (conf.b().destination() == Destination.BOT) {
                final var id = conf.b().id();
                if (!processed.contains(id)) bl.add(id);
            }
        }
        assertEquals("answer 1", 118, answer1.get().intValue());

        long answer2 = 1;
        final Function<PairDI, Boolean> isOutput
                = (p) -> p.destination() == Destination.OUTPUT && p.id() >= 0 && p.id() <= 2;
        for (final var e : botOutMap.entrySet()) {
            final var pp = e.getValue();
            if (isOutput.apply(pp.a())) {
                answer2 *= botOut(valueBotMap, botOutMap, e.getKey()).a();
            }
            if (isOutput.apply(pp.b())) {
                answer2 *= botOut(valueBotMap, botOutMap, e.getKey()).b();
            }
        }
        assertEquals("answer 2", 143153, answer2);
    }

    enum Destination {BOT, OUTPUT}

    enum Source {BOT, VALUE}

    record PairDI(Destination destination, int id) {
    }

    record PairSI(Source source, int id) {
    }

    record PairII(int a, int b) {
    }

    record PairDIDI(PairDI a, PairDI b) {
    }

    public static class BotMap extends HashMap<Integer, PairDIDI> {
    }

    public static class ValueMap extends HashMap<Integer, Integer> {
    }
}
