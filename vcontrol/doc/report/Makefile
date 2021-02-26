.PHONY: all clean veryclean

LATEXMK=latexmk
LATEXMKOPTS=-e '$$bibtex_use = 2; $$clean_ext = q/run.xml/'

all: upcvurep.pdf


upcvurep.pdf: upcvurep.tex local.bib upc-vu-v1.jpg
	$(LATEXMK) $(LATEXMKOPTS) -pdflatex -pdf upcvurep.tex

clean:
	\rm -f *~
	$(LATEXMK) $(LATEXMKOPTS) -c

veryclean: clean
	$(LATEXMK) $(LATEXMKOPTS) -C
