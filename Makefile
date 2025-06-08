all:
	flex -l calc-lex.l
	bison -vd calc-parser.y
	gcc calc-parser.tab.c table.c -lfl -lm -o natLangCalc

clean:
	rm -f natLangCalc lex.yy.c calc-parser.tab.c calc-parser.tab.h *.o