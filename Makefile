all:
	flex -l calc-lex.l;
	yacc -vd calc-parser.y;
	gcc lex.yy.c y.tab.c table.c -ll -o natLangCalc