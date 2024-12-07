import common as c
import unittest


def _parse_equation(line: str) -> (int, []):
    a = line.split(': ')
    test_value = int(a[0])
    numbers = [int(n) for n in a[1].split()]
    return test_value, numbers


def _is_equation3(test_value: int, partial: int, numbers: [], is_concat: bool) -> bool:
    def concat(a: int, b: int) -> int:
        return int(str(a) + str(b))

    if partial > test_value:
        return False
    if len(numbers) == 0:
        return partial == test_value
    if len(numbers) == 1:
        a = numbers[0]
        if (test_value == (a + partial)) or (test_value == (a * partial)):
            return True
        return is_concat and test_value == concat(partial, a)
    elif len(numbers) >= 2:
        a = numbers[0]
        n = numbers[1:]
        if _is_equation3(test_value, partial + a, n, is_concat):
            return True
        if partial > 0 and test_value / partial >= a:
            # The check helps to avoid overflow
            mp = partial * a if partial > 0 else a
            if _is_equation3(test_value, mp, n, is_concat):
                return True
        if is_concat:
            if partial > 0 and _is_equation3(test_value, concat(partial, a), n, is_concat):
                return True
    return False


def _is_equation(line: str, is_concat: bool) -> bool:
    test_value, numbers = _parse_equation(line)
    return _is_equation3(test_value, 0, numbers, is_concat)


def _answer1(lines: []) -> int:
    return sum([_parse_equation(l)[0] for l in lines if _is_equation(l, False)])


def _answer2(lines: []) -> int:
    return sum([_parse_equation(l)[0] for l in lines if _is_equation(l, True)])


class Day07(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(_answer1(lines), a1, "answer 1")
        self.assertEqual(_answer2(lines), a2, "answer 2")

    def test_a1(self):
        self.assertEqual(_is_equation("292: 11 6 16 20", False), True, "answer 1")

    def test_a2(self):
        good = ["7290: 6 8 6 15", "156: 15 6", "192: 17 8 14"]
        for tc in good:
            if not _is_equation(tc, True):
                self.fail(f"answer2 for: {tc}")

    def test_sample(self):
        self.__solution("07-1", 3749, 11387)

    def test_day(self):
        self.__solution("07", 5030892084481, 91377448644679)
