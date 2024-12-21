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

    def try_move(dx, dy: int) -> (bool, c.Point | None, str | None):
        p1 = c.Point(p.x + dx, p.y + dy)
        digit = point_keypad.get(p1, ND)
        if digit == ND: return False, None, None
        return True, p1, DIRECTIONS[c.Point(dx, dy)]

    dx, dy = end.x - p.x, end.y - p.y
    is_x_major = abs(dx) > abs(dy)
    while p != end:
        dx, dy = end.x - p.x, end.y - p.y
        if is_x_major:
            is_x_priority = dx != 0
        else:
            is_x_priority = dy == 0
        dx, dy = sign(dx), sign(dy)
        if is_x_priority:
            # let's try to move on x axis
            ok, p1, dr = try_move(dx, 0)
            if not ok:
                ok, p1, dr = try_move(0, dy)
        else:
            # let's try to move on y axis
            ok, p1, dr = try_move(0, dy)
            if not ok:
                ok, p1, dr = try_move(dx, 0)
        p = p1
        result += dr
    result += A  # once properly positioned the arm, we need to press A
    return result


def _distance(a, b: str, keypad: {str: c.Point}) -> int:
    if a == b: return 0
    pa = keypad[a]
    pb = keypad[b]
    return abs(pa.x - pb.x) + abs(pa.y - pb.y)


def _navigate_to_rec(start, end: c.Point, point_keypad: {c.Point: str}) -> str:
    if start == end:
        return "A"
    dx, dy = sign(end.x - start.x), sign(end.y - start.y)

    def try_move(dx, dy: int) -> c.Point | None:
        if dx == 0 and dy == 0: return None
        p1 = c.Point(start.x + dx, start.y + dy)
        digit = point_keypad.get(p1, ND)
        if digit == ND: return None
        return p1

    def try_shift(dx, dy: int) -> str | None:
        s = c.Point(sign(dx), sign(dy))
        p1 = try_move(s.x, s.y)
        if p1 is None: return None
        return DIRECTIONS[s] + _navigate_to_rec(p1, end, point_keypad)

    seq_x = try_shift(dx, 0)
    seq_y = try_shift(0, dy)
    seq = [s for s in [seq_x, seq_y] if s is not None]

    def path(s: seq) -> int:
        length = 0
        for i, si in enumerate(s[:-1]):
            length += _distance(si, s[i + 1], DIRECTIONAL_KEYPAD)
        return length

    seq.sort(key=path)
    return seq[0]


def _navigate(code: str, keypad: {str: c.Point}) -> str:
    position = keypad[A]
    result = ""
    point_keypad = {v: k for k, v in keypad.items()}
    for digit in code:
        target = keypad[digit]
        result += _navigate_to_rec(position, target, point_keypad)
        position = target
    return result


def _shortest_sequence(code: str) -> str:
    seq = _navigate(code, NUMERIC_KEYPAD)  # high levels of radiation
    seq1 = _navigate(seq, DIRECTIONAL_KEYPAD)  # -40 degrees
    seq2 = _navigate(seq1, DIRECTIONAL_KEYPAD)
    return seq2


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
        self.assertEqual(68 * 29, _complexity("029A", len(seq)), "complexity")
        self.assertEqual(len(seq), len(_shortest_sequence("029A")), "shortest sequence len")

    def test_navigate_a0(self):
        pad = NUMERIC_KEYPAD
        self.assertEqual("<A", _navigate_to(pad[A], pad['0'], POINT_NUMERIC_KEYPAD), "navigate A->0")

    def test_navigate(self):
        self.assertEqual("<A^A>^^AvvvA", _navigate("029A", NUMERIC_KEYPAD), "navigate 029A")

    def test_sample(self):
        self.__solution("21-1", 0, 0)

    # 157692 - too high
    def test_day(self):
        self.__solution("21", 0, 0)
