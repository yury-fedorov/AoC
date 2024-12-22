# eg. "run 24"
YEAR=$1
cd AoC$YEAR
TAG=yury.fedorov/aoc$YEAR
docker rmi $TAG --force
docker buildx build -t $TAG .
docker run $TAG
cd ..
