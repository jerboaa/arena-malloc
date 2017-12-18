#!/bin/bash
# Build the 64bit version
sudo docker build -f Dockerfile.centos7 -t docker.io/jerboaa/arena-malloc:1-centos7-64bit .
# Build the 32bit version
sudo docker build -f Dockerfile.centos7-32bit -t docker.io/jerboaa/arena-malloc:1-centos7-32bit .
