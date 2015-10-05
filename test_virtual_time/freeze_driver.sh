#!/bin/bash

./time_long_loop

./time_long_loop -v &
./freeze_other -p $!

