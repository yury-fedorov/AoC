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

_DIRECTIONS = [(0,-1), (1,0), (0,1), (-1,0)] 

def _answer1(the_map: [], start_point: (int, int)) -> int:
    
    def is_in(p:(int, int)) -> bool:
        return True
    
    path ={start_point}
    p=start_point
    d=_DIRECTIONS[0]

    while is_in(p):
        x0, y0 = p
        dx, dy = d
        p1 = x0+dx, y0+dy
        if is_wall(p1):
            d = rotate(d) 
        if is_in(p):
            path.add(p)
        else:
            return len(path) 


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
