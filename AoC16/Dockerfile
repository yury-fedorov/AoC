FROM ubuntu:24.10

RUN apt-get update \
    && apt-get --no-install-recommends -y install openjdk-17-jdk maven
RUN mvn --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src
CMD mvn test

