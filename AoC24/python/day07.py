import common as c
import unittest


def _parse_equation(line: str) -> (int, []):
    a = line.split(': ')
    test_value = int(a[0])
    numbers = [int(n) for n in a[1].split()]
    return test_value, numbers


def _is_equation3(test_value: int, partial: int, numbers: []) -> bool:
    if partial > test_value:
        return False
    if len(numbers) == 1:
        a = numbers[0]
        return (test_value == (a + partial)) or (test_value == (a * partial))
    elif len(numbers) >= 2:
        a = numbers[0]
        n = numbers[1:]
        if _is_equation3(test_value, partial + a, n):
            return True
        if partial > 0 and test_value / partial >= a:
            # The check helps to avoid overflow
            if _is_equation3(test_value, partial * a, n):
                return True
    return False


def _is_equation(line: str) -> bool:
    test_value, numbers = _parse_equation(line)
    return _is_equation3(test_value, 0, numbers)


def _answer1(lines: []) -> int:
    return sum([_parse_equation(l)[0] for l in lines if _is_equation(l)])


def _answer2(lines: []) -> int:
    return 0


class Day07(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(_answer1(lines), a1, "answer 1")
        # self.assertEqual(_answer2(lines), a2, "answer 2")

    def test_wrong(self):
        self.assertEqual(_is_equation("292: 11 6 16 20"), True, "answer 1")

    def test_sample(self):
        self.__solution("07-1", 3749, 0)

    def test_day(self):
        self.__solution("07", 5030892084481, 0)
