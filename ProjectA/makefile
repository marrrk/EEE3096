.RECIPEPREFIX +=
CC = gcc
CFLAGS = -Wall -lm -lrt -lwiringPi -lpthread

PROG = bin/*
OBJS = obj/*

default:
    mkdir -p bin obj
    $(CC) $(CFLAGS) -c src/project.c -o obj/project
    $(CC) $(CFLAGS) obj/project -o bin/project

run:
   sudo ./bin/project

clean:
   rm $(PROG) $(OBJS)
