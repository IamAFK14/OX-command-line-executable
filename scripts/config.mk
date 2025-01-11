export CC = gcc
export CXX = g++
export LD = gcc
export LDXX = g++
export CFLAGS = -std=c17 -g -O2 -m64
export CXXFLAGS = -std=c++17 -g -O2 -m64
export LDFLAGS = 

export SRC_DIR = $(abspath src)
export HOMEDIR = $(abspath .)
export OUTPUT = $(abspath output)