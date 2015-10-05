#!/bin/bash

set +m
./time_long_loop

./time_long_loop -v &
./freeze_other -p $!

