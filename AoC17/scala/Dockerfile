FROM sbtscala/scala-sbt:eclipse-temurin-alpine-24.0.1_9_1.11.7_3.7.4
# <JDK version>_<sbt version>_<Scala version>

RUN sbt --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src
CMD sbt test

