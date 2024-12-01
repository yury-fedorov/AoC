FROM ubuntu:24.10

RUN apt-get update \
    && apt-get --no-install-recommends -y install git cmake make clang

RUN clang --version

RUN git config --global http.sslverify false

ARG src=/usr/src/aoc

COPY . $src

# RUN cd $src; rm CMakeCache.txt; rm -rf CMakeFiles; rm -rf _deps

WORKDIR $src
RUN cmake .
RUN make all
CMD ["./aoc20"]
