#! /bin/bash

function run {
    $1 > >(sed "s/^/[$2] /") 2>&1 &
}

docker-compose up -d

cd ../simulation
npm install

cd ../frontend
npm install

cd ../core
run "./gradlew run" "core"

cd ../simulation
run "npm start" "simulation"

cd ../frontend
run "npm run build" "frontend"
run "npm run server" "server"

wait