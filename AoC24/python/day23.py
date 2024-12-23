import common as c
import unittest
from itertools import combinations


def _target(name: str) -> bool: return name.startswith("t")


def _answer1(links: {}) -> int:
    first_computer = [name for name in links.keys() if _target(name) and len(links[name]) >= 2]
    result = set({})
    for a in first_computer:
        candidates = list(set([l for l in links[a] if len(links[l]) >= 2]))
        for i, b in enumerate(candidates):
            for j, c in enumerate(candidates):
                if i < j:
                    if c in links[b]:
                        cs = [a, b, c]
                        cs.sort()
                        cst = tuple(cs)
                        result.add(cst)
    return len(result)


def _is_set(nodes: [str], links: {}) -> bool:
    for i, a in enumerate(nodes):
        for j, b in enumerate(nodes):
            if i < j:
                if not b in links[a]:
                    return False
    return True


def _largest(head: str, links: {}) -> {str}:
    full = list(links[head])
    full.append(head)
    n = len(full)
    if _is_set(full, links): return set(full)
    while n > 2:
        n -= 1
        for ci in combinations(full, n):
            if _is_set(ci, links):
                return set(ci)
    return {head}


def _answer2(links: {}) -> str:
    result = []
    for h in links.keys():
        ri = list(_largest(h, links))
        if len(ri) > len(result):
            result = ri
    result.sort()
    return ",".join(result)


class Day23(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)

        links = {}

        def add_link(from_c, to_c: str):
            if from_c in links:
                links[from_c].add(to_c)
            else:
                links[from_c] = {to_c}

        for l in lines:
            a, b = l.split("-")
            add_link(a, b)
            add_link(b, a)

        self.assertEqual(a1, _answer1(links), "answer 1")
        self.assertEqual(a2, _answer2(links), "answer 2")

    def test_sample(self):
        self.__solution("23-1", 7, "co,de,ka,ta")

    # dn,dp,ff,gq,kw,ou,qc,qz,tf,us,xy,ya - not right answer
    def test_day(self):
        self.__solution("23", 1304, "ao,es,fe,if,in,io,ky,qq,rd,rn,rv,vc,vl")
