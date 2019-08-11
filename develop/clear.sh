#! /bin/bash

docker-compose down -v

echo "Clear storage directory"
if ls storage/* 1> /dev/null 2>&1; then
  rm storage/*
fi
