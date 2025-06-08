all:
	flex -l calc-lex.l;
	bison -vd calc-parser.y;
	gcc y.tab.c table.c -lfl -lm -o natLangCalc
