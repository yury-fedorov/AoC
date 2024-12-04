# common functions
import os.path


def open_file(data_file: str):
    path = "../data/" + data_file + ".txt"
    if not os.path.exists(path):
        # Execution from GitHub actions
        path = "./AoC24/data/" + data_file + ".txt"
    return open(path, "r")


def read_lines(data_file: str) -> []:
    with open_file(data_file) as f:
        return f.read().splitlines()