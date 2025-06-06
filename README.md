# NaturalLanguageCalculator
Formal Languages and Compilers Project


## Tutorial LEX and YACC

### Step 1: Install Required Tools

Make sure the required packages are installed:

```bash
sudo apt update
sudo apt install flex bison build-essential
```

### Step 2: Create files

Create the lex.l LEx file 
Create the parse.y YACC file
Create the Makefile (it will automatise build)


### Step 3: Build the project

```bash
make
```

An excutable called **parser** will be generated

### Step 4: Run the files

Run the parser 

```bash
./parser
```

Type input
```bash
Twenty five plus twenty five
```

You should see the result printed
```bash
Fifty
```

