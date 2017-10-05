all: proj4

proj4: proj4.tex
	latex proj4
	bibtex proj4
	latex proj4
	latex proj4
	dvips -t a4 proj4.dvi
	ps2pdf -sPAPERSIZE=a4  proj4.ps

pack:
	zip xtomas32-fit.zip proj4.tex Makefile
clean:
	rm proj4.log proj4.dvi proj4.aux proj4.ps proj4.synctex.gz proj4.bb*
