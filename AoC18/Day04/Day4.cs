using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace AdventOfCode2018.Day04
{
    public enum LogEvent
    {
        GuardBeginsShift,
        FallsAsleep,
        WakesUp,
        Unknown
    }

    public struct LogRecord
    {
        public DateTime _time;
        public LogEvent _logEvent;
        readonly string _originalLogRecord;
        public string _record; // pure record

        public LogRecord(string record)
        {
            _originalLogRecord = record;

            _time = DateTime.MinValue;
            _record = null;
            _logEvent = LogEvent.Unknown;

            // [1518-11-01 00:00] Guard #10 begins shift

            var pattern = @"\[1518-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+)\] (.+)";
            var matches = Regex.Matches(record, pattern);
            foreach (Match match in matches)
            {
                var month = Convert.ToInt32(match.Groups[1].Value);
                var day = Convert.ToInt32(match.Groups[2].Value);
                var hour = Convert.ToInt32(match.Groups[3].Value);
                var minute = Convert.ToInt32(match.Groups[4].Value);
                _time = new DateTime(2018, month, day, hour, minute, 0);
                _record = match.Groups[5].Value;
                if (record.Contains("Guard"))
                    _logEvent = LogEvent.GuardBeginsShift;
                else if (record.Contains("falls asleep"))
                    _logEvent = LogEvent.FallsAsleep;
                else if (record.Contains("wakes up"))
                    _logEvent = LogEvent.WakesUp;

                break;
            }
            if (_logEvent == LogEvent.Unknown) throw new Exception("unexpected record");
        }

        public int GuardId()
        {
            if (_logEvent != LogEvent.GuardBeginsShift) throw new Exception("not available");
            var pattern = @"Guard #([0-9]+) begins shift";
            var matches = Regex.Matches(_record, pattern);
            foreach (Match match in matches)
            {
                return Convert.ToInt32(match.Groups[1].Value);
            }
            throw new Exception("unexpected error");
        }
    }

    public class Day4
    {
        public IEnumerable<LogRecord> ReadLog(IEnumerable<string> log)
        {
            return log.Select(r => new LogRecord(r)).OrderBy(r => r._time);
        }

        public IDictionary<int, int> GuardSleep(IEnumerable<LogRecord> log)
        {
            var guardSleep = new Dictionary<int, int>();
            int? currentGuard = null; // unknown
            LogRecord? previous = null;
            foreach (var record in log)
            {
                switch (record._logEvent)
                {
                    case LogEvent.GuardBeginsShift:
                        currentGuard = record.GuardId();
                        break;
                    case LogEvent.FallsAsleep:
                        break;
                    case LogEvent.WakesUp:
                        ClassicAssert.True(previous.Value._logEvent == LogEvent.FallsAsleep);
                        var slept = record._time - previous.Value._time;
                        if (guardSleep.ContainsKey(currentGuard.Value))
                        {
                            guardSleep[currentGuard.Value] += slept.Minutes;
                        }
                        else
                            guardSleep.Add(currentGuard.Value, slept.Minutes);
                        break;
                }
                previous = record;
            }
            return guardSleep;
        }

        public Tuple<int, ICollection<int>> MostSleptMinutes(IEnumerable<LogRecord> log, int id)
        {
            var minuteSlept = new Dictionary<int, int>();
            for (int m = 0; m < 60; m++) minuteSlept.Add(m, 0);

            int? currentGuard = null; // unknown
            LogRecord? previous = null;
            foreach (var record in log)
            {
                switch (record._logEvent)
                {
                    case LogEvent.GuardBeginsShift:
                        currentGuard = record.GuardId();
                        break;
                    case LogEvent.FallsAsleep:
                        break;
                    case LogEvent.WakesUp:
                        ClassicAssert.True(previous.Value._logEvent == LogEvent.FallsAsleep);
                        ClassicAssert.True(previous.Value._time.Minute < record._time.Minute);
                        if (currentGuard.Value != id) continue;
                        for (int minute = previous.Value._time.Minute;
                            minute < record._time.Minute; minute++)
                            minuteSlept[minute] += 1;
                        break;
                }
                previous = record;
            }
            var maxSleptTimes = minuteSlept.Values.Max();
            return
                Tuple.Create(maxSleptTimes,
                    minuteSlept.Where(p => p.Value == maxSleptTimes)
                    .Select(p => p.Key).ToList() as ICollection<int>);
        }
    }

    public class Day4Test
    {
        private readonly Day4 day4 = new Day4();

        [TestCase("Day04/sample.txt", 10, 24, 99, 45)]
        [TestCase("Day04/input.txt", 1993, 36, 2137, 50)]
        public void TestStar1(string file, int guardId, int minute, int guard2, int minute2)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var log = day4.ReadLog(lines).ToArray();
            var map = day4.GuardSleep(log);
            var sleptMost = map.Values.Max();
            var id = map.Where(p => p.Value == sleptMost).Select(p => p.Key).Single();
            ClassicAssert.AreEqual(guardId, id, "wrong guard id");
            var minuteCalculated = day4.MostSleptMinutes(log, id).Item2.Single();
            ClassicAssert.AreEqual(minute, minuteCalculated, "wrong minute");

            var guards = map.Keys;
            var guardMostSleptMinute = guards.Select(g => Tuple.Create(g, day4.MostSleptMinutes(log, g)))
                .ToArray();
            var minute2c = guardMostSleptMinute.Max(t => t.Item2.Item1);
            var guard2c = guardMostSleptMinute.Where(t => t.Item2.Item1 == minute2c).Single();
            ClassicAssert.AreEqual(guard2, guard2c.Item1, "wrong guard");
            ClassicAssert.AreEqual(minute2, guard2c.Item2.Item2.Single(), "wrong minute");
            // Assert.AreEqual(0, guard2 * minute2, "response 2");
        }
    }
}
