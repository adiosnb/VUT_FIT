all: proj3

proj3: proj3.tex
	latex $@
	latex $@
	dvips -t a4 proj3.dvi
	ps2pdf -sPAPERSIZE=a4  proj3.ps

pack:
	zip xtomas32-fit.zip proj3.tex Makefile oniisan.eps oniisan2.eps etiopan.eps
clean:
	rm proj3.log proj3.dvi proj3.aux proj3.ps
