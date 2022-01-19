#!/usr/bin/python
# Created   : Sun 26 Jul 2009 12:12:40 AM PDT
# Modified  : Sun 26 Jul 2009 12:12:40 AM PDT
# Author    : Gautam Iyer <gi1242@users.sourceforge.net>
#
# Split a directory into sized directories. Source is left untouched, and
# hardlinks are created in the destination.

import os
import sys
from sys import stdout, stderr, argv
from os import path
from getopt import getopt, GetoptError

# Functions {{{1
def die(msg):
    print (ER+msg+RE)
    sys.exit(1)

def usage():
    print """\
dsplit [options] src dst

Splits src into CD/DVD sized sub-directories. Output is in vol-%2d subdirs of
dst, and by default the files are hard-linked to the source (so they consume no
space).

OPTIONS

    -s size, --volume-size=size
	Size of each volume in MB (default 4400MB)

    --for-dvd	Set volume size for DVD (4400MB)
    --for-dvd2	Set volume size for double layer DVD (8080MB)
    --for-cd	Set volume size for CD (680MB)
    --for-bdr   Set volume size for BDR (23800MB)
   
    -v		Verbose (print all subdirectories included in volumes)
    -V		Very verbose (print all files included too)"""
    sys.exit(0)

def newvolume( output, volume, volsize, thisvolfiles, thisvoldirs ):
    if volume != 0:
	stdout.write( ( BD+"Total: %.2f MB (%d files, %d dirs)\n"+RE ) % \
		( volsize / MB, thisvolfiles, thisvoldirs ) )

    if volume >= 0:
	volume += 1
	volsize = 0

	stdout.write( (BD + "Volume %02d:" + RE +"%s") % \
		(volume, verbose and "\n" or "" ) )
	voldir = path.join( output, "vol-%02d" % volume, "assets" )

	return (voldir, volume, volsize)

def addfile( targetdir, root, f ):
    if veryverbose:
	stdout.write( (IT + "%s " + RE) % (f) )
    os.link( path.join( root, f), path.join( targetdir, f ) )

#}}}

# Code starts here
global BD, UL, IT, ER, RE

BD="\033[36m"	# Cyan
UL="\033[32m"	# Green
IT="\033[33m"	# Yellow
ER="\033[31m"	# Red
RE="\033[0m"	# Normal

global verbose, veryverbose
verbose=False
veryverbose=False

MB=1024*1024.0
maxSize=4400 * MB
cdsize=680*MB
dvdsize=4400*MB
dvd2size=8080*MB
bdrsize=23800*MB

try:
    (popts, argv) = getopt( argv[1:], 's:vVh',
	    ['volume-size=', 'for-cd', 'for-dvd', 'for-dvd2'] )
except GetoptError, opt:
    die(str(opt));

for (opt, value) in popts:
    if opt == '-s' or opt == '--volume-size':
	maxSize = float(value)*MB;
    elif opt == '--for-cd':
	maxSize = cdsize
    elif opt == '--for-dvd':
	maxSize = dvdsize
    elif opt == '--for-dvd2':
	maxSize = dvd2size
    elif opt == '--for-bdr':
        maxSize = bdrsize
    elif opt == '-v':
	verbose=True
    elif opt == '-V':
	verbose = True
	veryverbose = True
    elif opt == '-h':
	usage()
#end for (opt,value)

if len(argv) < 2:
    die( "USAGE: dsplit [options] src dst. (Use -h for help)" )
else:
    input = argv[0]
    output = argv[1]

if path.exists( output ):
    die( "%s already exists." % output )

(voldir, volume, volsize) = newvolume( output, 0, 0, 0, 0 )
thisvolfiles = 0
thisvoldirs = 0
for root, dirs, files in os.walk(input, True):
    if verbose:
	stdout.write( "  %s: " % (root) )

    thisdirfiles=0
    thisdirsize=0

    if root.find( input ) != 0:
	die( "%s isn't a subdir of %s" % (root, input) )

    targetdir = path.join( voldir, root.replace( input, "", 1 ) )

    for f in files:
	# File size without following symlinks.
	s = os.lstat( path.join( root, f ) ).st_size

	if s > maxSize:
	    stdout.write( (ER + "%s (%.2f MB) " + RE) % (f, s / MB) )
	    continue

	elif s + volsize > maxSize:
	    if verbose:
		stdout.write( ("%d files ("+UL+"%.2f MB"+RE+").\n") % \
			(thisdirfiles, thisdirsize/MB) )
	    thisdirfiles = 0
	    thisdirsize = 0

	    (voldir, volume, volsize) = \
		    newvolume( output, volume, volsize, thisvolfiles, \
			thisvoldirs )
	    targetdir = path.join( voldir, root.replace( input, "", 1 ) )
	    thisvolfiles = 0
	    thisvoldirs = 0

	    if verbose:
		stdout.write( "  %s: " % (root) )

	if thisdirfiles == 0:
	    thisvoldirs += 1
	    if not path.isdir( targetdir ):
		os.makedirs( targetdir )
		
	addfile( targetdir, root, f )
	thisvolfiles += 1
	thisdirfiles += 1
	thisdirsize += s
	volsize += s
    #end for f
    if verbose:
	stdout.write( ("%d files ("+UL+"%.2f MB"+RE+").\n") % \
		(thisdirfiles, thisdirsize/MB) )

#end for (...) in os.walk
newvolume( output, -1, volsize, thisvolfiles, thisvoldirs )