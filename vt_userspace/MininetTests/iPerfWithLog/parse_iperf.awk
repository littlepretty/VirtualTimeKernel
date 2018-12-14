#!/usr/bin/awk -f

BEGIN {
        lines=0;
}
{
        ++lines;
        if (lines >= 14 && lines <= 33) {
                print $7;
        }
}
END {
}

