FROM ubuntu:latest

RUN apt-get update \
    && apt-get --no-install-recommends -y install git cmake make clang clang-tidy
RUN clang --version

RUN git config --global http.sslverify false
ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src
RUN cmake .
RUN make all
CMD ["./aoc17"]
