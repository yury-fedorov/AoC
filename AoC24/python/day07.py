import common as c
import unittest

def _parse_equation(line: str) -> (int, []):
    a = line.split(': ')
    test_value = int(a[0])
    numbers = [int(n) for n in a[1].split()]
    return test_value, numbers

def _is_equation2(test_value: int, numbers: []) -> bool:
    if len(numbers) == 2:
        a = numbers[0]
        b = numbers[1]
        return (test_value == (a + b)) or (test_value == (a * b))
    elif len(numbers) > 2:
        a = numbers[0]
        n = numbers[1:]
        # for sum
        tvs = test_value - a
        if tvs >= 0:
            if _is_equation2(tvs, n):
                return True
        # for multiplication
        remainder = test_value % a
        if remainder == 0:
            tvm = test_value / a
            if _is_equation2(tvm, n):
                return True
    return False

def _is_equation(line: str) -> bool :
    test_value, numbers = _parse_equation(line)
    return _is_equation2(test_value, numbers)

def _answer1(lines:[]) -> int:
    return sum([_parse_equation(l)[0] for l in lines if _is_equation(l)])

def _answer2(lines:[]) -> int:
    return 0


class Day07(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(_answer1(lines), a1, "answer 1")
        # self.assertEqual(_answer2(lines), a2, "answer 2")

    def test_sample(self):
        self.__solution("07-1", 3749, 0)

    def test_day(self):
        self.__solution("07", 0, 0)
