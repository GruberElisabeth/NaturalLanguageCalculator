all:
	flex -l calc-lex.l;
	bison -vd calc-parser.y;
	gcc calc-parser.tab.c -lfl -lm -o calc-parser
