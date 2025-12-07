YEAR=17
cd AoC$YEAR.scala
TAG=yury.fedorov/aoc$YEAR
docker rmi $TAG --force
docker buildx build -t $TAG .
docker run $TAG
