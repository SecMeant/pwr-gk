#!/bin/sh

if test "$#" -eq 0; then
	echo "Error: give me some file to compile."
	exit 1;
fi

pdflatex -file-line-error -interaction nonstopmode -halt-on-error -file-line-error $@
kill -1 $(pgrep mupdf)
