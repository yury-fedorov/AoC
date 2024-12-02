import day01
import day02
import day03
import unittest


def suite():
    suite = unittest.TestSuite()
    days = [day01.Day01, day02.Day02, day03.Day03]
    _IS_ALL = True  # all tests are executed or only the major ones ("test_day")
    for d in days:
        t = unittest.defaultTestLoader.loadTestsFromTestCase(d) if _IS_ALL else d("test_day")
        suite.addTest(t)
    return suite


if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())
