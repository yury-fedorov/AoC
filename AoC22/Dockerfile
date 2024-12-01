FROM ubuntu:latest

RUN apt-get update \
    && apt-get --no-install-recommends -y install git cmake make clang

# RUN clang --version

RUN git config --global http.sslverify false

ARG src=/usr/src/aoc

COPY . $src
RUN cd cpp; rm CMakeCache.txt; rm -rf CMakeFiles; rm -rf _deps

WORKDIR $src/cpp

RUN cmake .
RUN make all

CMD ["./aoc22"]
