FROM python:latest
RUN pip install numpy
ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src/python
CMD python3 aoc24.py
