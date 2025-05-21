FROM ubuntu:latest

RUN apt-get update \
    && apt-get --no-install-recommends -y install openjdk-21-jdk maven
RUN mvn --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src/kotlin
CMD mvn test

