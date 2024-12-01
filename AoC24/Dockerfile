FROM python:latest

# RUN python3 --version
# RUN pip install requests beautifulsoup4
# RUN git config --global http.sslverify false

ARG src=/usr/src/aoc

COPY . $src

WORKDIR $src/python

CMD python3 01.py
