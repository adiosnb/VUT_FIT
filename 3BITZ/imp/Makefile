all: manual

manual: doc.tex
	latex doc
	latex doc
	dvips -t a4 doc.dvi
	ps2pdf -sPAPERSIZE=a4  doc.ps

clean:
	rm -f doc.log doc.dvi doc.aux doc.ps doc.synctex.gz doc.b* doc.toc

zip:
	make
	zip -r xtomas32.zip doc.tex doc.pdf crc/
