# Running application natively

## Requirements

- Docker
- Node
- NPM
- Java 8

## Docker

Run `MongoDB` and `RabbitMQ` via `Docker`

```
cd develop
docker compose up -d
```

## Core

```
cd core
./gradlew run
```

After modification of source files manual restart is needed.

## Simulation

```
cd simulation
npm start
```

Modification of source files will trigger service reload.

## Forontend

Start building source files:

```
cd frontend
npm start
```

Serve builded files and static resources:

```
npm run server
```

Modification of source files will trigger web application refresh.
