all: proj2

proj2: proj2.tex
	latex $@
	latex $@
	dvips -t a4 proj2.dvi
	ps2pdf proj2.ps

clean:
	rm proj2.ps proj2.dvi proj2.aux proj2.log
