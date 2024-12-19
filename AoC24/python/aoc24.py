import day01
import day02
import day03
import day04
import day05
import day06
import day07
import day08
import day09
import day10
import day11
import day12
import day13
import day14
import day15
import day16
import day17
import day18
import day19
import unittest


def suite():
    suite = unittest.TestSuite()
    days = [day01.Day01, day02.Day02, day03.Day03, day04.Day04, day05.Day05, day06.Day06, day07.Day07, day08.Day08,
            day09.Day09, day10.Day10, day11.Day11, day12.Day12, day13.Day13, day14.Day14, day15.Day15, day16.Day16,
            day17.Day17, day18.Day18, day19.Day19]
    _IS_ALL = True  # all tests are executed or only the major ones ("test_day")
    for d in days:
        t = unittest.defaultTestLoader.loadTestsFromTestCase(d) if _IS_ALL else d("test_day")
        suite.addTest(t)
    return suite


if __name__ == '__main__':
    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(suite())
