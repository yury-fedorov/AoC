FROM ubuntu:23.10

RUN apt-get update \
    && apt-get --no-install-recommends -y install git cmake make clang

RUN git config --global http.sslverify false

ARG src=/usr/src/aoc22

COPY . $src

RUN cd $src; rm CMakeCache.txt; rm -rf CMakeFiles; rm -rf _deps

RUN cd $src; cmake . ; make all

CMD ["/usr/src/aoc22/aoc22"]
