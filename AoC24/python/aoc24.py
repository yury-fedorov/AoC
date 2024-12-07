import day01
import day02
import day03
import day04
import day05
import day06
import day07
import day08
import unittest


def suite():
    suite = unittest.TestSuite()
    days = [day01.Day01, day02.Day02, day03.Day03, day04.Day04, day05.Day05, day06.Day06, day07.Day07, day08.Day08]
    _IS_ALL = True  # all tests are executed or only the major ones ("test_day")
    for d in days:
        t = unittest.defaultTestLoader.loadTestsFromTestCase(d) if _IS_ALL else d("test_day")
        suite.addTest(t)
    return suite


if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())
