all: proj5

proj5: proj5.tex
	pdflatex $^
	pdflatex $^

clean:
	rm -rf proj5.aux proj5.log proj5.nav proj5.out proj5.snm proj5.toc

zip:
	zip xtomas32-fit.zip Makefile proj5.tex
