#!/bin/bash
set -euxo pipefail
echo mandelbrot.cpp | entr bash -c '
	set -eu
	clear
	date
	echo
	make mandelbrot
	echo -e "\n\e[32mbuild successful\e[0m\n"
	./mandelbrot 60 20 15 || echo -e "\n\e[31mfail\e[0m\n"
'
	
