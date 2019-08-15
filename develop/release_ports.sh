#! /bin/bash

# Abruptly closed services can leave open ports that prevent from running
# services again ("Port already in use" error). This script releases these
# ports by killing processes using them. 

fuser -k 3000/tcp # simulation
fuser -k 8000/tcp # frontend
fuser -k 8080/tcp # core

exit 0
