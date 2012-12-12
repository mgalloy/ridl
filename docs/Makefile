RST2MAN=rst2man.py

RST2LATEX=rst2latex.py
LATEX=xelatex
LATEX_OPTIONS=-halt-on-error

.PHONY: all clean

all: ridl.1 ridl_manual.pdf


ridl.1: ridl.rst
	$(RST2MAN) ridl.rst > ridl.1

ridl_manual.pdf: ridl_manual.rst
	$(RST2LATEX) ridl_manual.rst > ridl_manual.tex
	$(LATEX) $(LATEX_OPTIONS) ridl_manual.tex
	$(LATEX) $(LATEX_OPTIONS) ridl_manual.tex


clean:
	rm -f ridl.1
	rm -f ridl_manual.{out,aux,log,tex,pdf}