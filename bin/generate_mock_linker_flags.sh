#!/bin/bash

grep "^TEST_DOUBLE(" $1  | \
    awk -F"(" '{print $2}' | \
    awk -F"," '{print $3}' | \
    sed 's/^[ \t]*/--wrap=/' | \
    tr "\n" ", " |\
    sed 's/,$/ /;s/^/-Wl,/'
