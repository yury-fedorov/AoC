import common as c
import unittest


def _read_input(data: str):
    with c.open_file(data) as f:
        return [line.strip() for line in f]


def _start(the_map: []) -> (int, int):
    for y, line in enumerate(the_map):
        x = line.find('^')
        if x != -1:
            return x, y


def _answer1(the_map: [], start_point: (int, int)) -> int:
    return 0


def _answer2(the_map: []) -> int:
    return 0


class Day06(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = _read_input(data)
        start_point = _start(the_map)
        self.assertEqual(_answer1(the_map, start_point), a1, "answer 1")
        # self.assertEqual(_answer2(the_map), a2, "answer 2")

    def test_sample(self):
        self.__solution("06-1", 41, 0)

    def test_day(self):
        self.__solution("06", 0, 0)
