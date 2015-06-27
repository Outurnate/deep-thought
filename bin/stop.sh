#!/bin/bash
kill `cat $1/game.pid`
rm -rf ./$1/*
rmdir $1
