#!/usr/bin/perl

###################
# channon@iit.edu #
###################

# This file reads in a ping output and removes all but timeing info
# meant to be used with the cdfplot.py program for benchmarking
#use strict;
use warnings;

# filename
my $fnn = $ARGV[0];
# read in file
open (my $MYFILE, $fnn)
    or die "Could not open file '$fnn' $!";
while(my $line = <$MYFILE>) {
    # match file name
    if($line =~ m/.*time=(.*) ms/){   
	# write to std out
	print "$1\n";
    }
}	
