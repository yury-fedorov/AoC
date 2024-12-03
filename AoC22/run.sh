TAG=yury.fedorov/aoc22
docker rmi $TAG --force
docker build -t $TAG .
docker run $TAG