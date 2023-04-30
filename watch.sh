#!/bin/bash
set -euxo pipefail
echo mandelbrot.cpp | entr bash -c '
	set -eu
	clear
	date
	echo
	make mandelbrot
	echo -e "\n\e[32mbuild successful\e[0m\n"
  #echo rendering ...
	#time ./mandelbrot 300 100 12 || echo -e "\n\e[31mfail\e[0m\n"
  #ffmpeg -i "mandelbrot-%03d.bmp" -r 60 mandelbrot.mp4
'
	
