import common as c
import unittest

ND = ' '  # not defined key
A = 'A'
UP = '^'
DOWN = 'v'
LEFT = '<'
RIGHT = '>'

NUMERIC_KEYPAD = {'7': c.Point(0, 0), '8': c.Point(1, 0), '9': c.Point(2, 0),
                  '4': c.Point(0, 1), '5': c.Point(1, 1), '6': c.Point(2, 1),
                  '1': c.Point(0, 2), '2': c.Point(1, 2), '3': c.Point(2, 2),
                  '0': c.Point(1, 3), 'A': c.Point(2, 3)}

POINT_NUMERIC_KEYPAD = {v: k for k, v in NUMERIC_KEYPAD.items()}

DIRECTIONAL_KEYPAD = {UP: c.Point(1, 0), 'A': c.Point(2, 0),
                      LEFT: c.Point(0, 1), DOWN: c.Point(1, 1), RIGHT: c.Point(2, 1)}

POINT_DIRECTIONAL_KEYPAD = {v: k for k, v in DIRECTIONAL_KEYPAD.items()}

DIRECTIONS = {c.Point(-1, 0): LEFT, c.Point(0, -1): UP, c.Point(1, 0): RIGHT, c.Point(0, 1): DOWN}


def sign(value: int) -> int:
    return 0 if value == 0 else 1 if value > 0 else -1


def _navigate_to(start, end: c.Point, point_keypad: {c.Point: str}) -> str:
    p = start
    result = ""
    while p != end:
        dx, dy = sign(end.x - p.x), sign(end.y - p.y)
        found = False
        if dx != 0:
            # let's try to move on x axis
            p1 = c.Point(p.x + dx, p.y)
            digit = point_keypad.get(p1, ND)
            if digit != ND:
                p = p1
                result += DIRECTIONS[c.Point(dx, 0)]
                found = True
        if not found:
            # let's try to move on y axis
            p = c.Point(p.x, p.y + dy)
            result += DIRECTIONS[c.Point(0, dy)]
    result += A  # once properly positioned the arm, we need to press A
    return result


def _navigate(code: str, keypad: {str: c.Point}) -> str:
    position = keypad[A]
    result = ""
    point_keypad = {v: k for k, v in keypad.items()}
    for digit in code:
        target = keypad[digit]
        result += _navigate_to(position, target, point_keypad)
        position = target
    return result


def _shortest_sequence(code: str) -> str:
    # TODO - generate the shortest sequence
    return code


def _complexity(code: str, length: int) -> int:
    return int(code[:-1]) * length


def _answer1(codes: [str]) -> int:
    return sum(_complexity(code, len(_shortest_sequence(code))) for code in codes)


def _answer2(lines: [str]) -> int:
    return 0


class Day21(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        codes = c.read_lines(data)
        self.assertEqual(a1, _answer1(codes), "answer 1")
        self.assertEqual(a2, _answer2(codes), "answer 2")

    def test_complexity(self):
        seq = "<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A"
        self.assertEqual(68 * 29, _complexity("029A", len(seq)), "answer 1")

    def test_navigate_a0(self):
        pad = NUMERIC_KEYPAD
        self.assertEqual("<A", _navigate_to(pad[A], pad['0'], POINT_NUMERIC_KEYPAD), "navigate A->0")

    def test_navigate(self):
        self.assertEqual("<A^A>^^AvvvA", _navigate("029A", NUMERIC_KEYPAD), "navigate 029A")

    def test_sample(self):
        self.__solution("21-1", 0, 0)

    def test_day(self):
        self.__solution("21", 0, 0)