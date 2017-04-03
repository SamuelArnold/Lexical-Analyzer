//Sam Arnold
//CSCI 434, Spring 2017
//Description:  This is the main file and contains:
// Main function and Syntax analyzer

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
// Token codes //
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define RESERVEDWORD 27
#define COMMA 28
#define QUOTE 29
#define LITERAL 30

// Variables
char lexeme[100],
prevChar,
nextChar;

int charClass,
lexLen,
token,
nextToken;
FILE *in_fp, *fopen();
// Function declarations
void addChar();
void getChar();
void getNonBlank();
int lex();
bool GetIfReservedWord(char ch);
bool isLiterial();
//ofstream myfile;
//Character classes

void expr();
void error();
void Valid();
void term();
void factor();
void Check_Beginning();
void NewSynt();
void Check_Declarer();
void Check_Output();
bool static ErrorFlag = false;
int Line = 0,
L_Par = 0,
R_Par = 0;


using namespace std;


int main()
{
	// Variable 

	bool exit = false;					 //exit 
	int len,							// length of lexeme?
		programnum;						// Name of program number 
	string prepend = "program",			// program name prepend
		   end     = ".dat",			// ending of program
		   myprogram;					// Full name of input file

	// delete file 
	if (remove("Output.dat") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");

	//get input for file name
	for (;;) {
		// check for number
		cout << "Please enter a number ('12' by default) : " << flush;
		cin >> programnum;
		// Sanatize bad input
		if (std::cin.fail()) {
			std::cerr << "Sorry, I cannot read that. Please try again." << endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		// input was correct
		std::cout << "You have entered: " << programnum << " Thank you! " << endl;
		break;
	}

	myprogram = prepend + std::to_string(programnum) + end;
	cout << "Name of the program you have entered: " << myprogram << endl <<endl;
	// Open the input data file and process its contents "program.dat"
	if ((in_fp = fopen(myprogram.c_str(), "r")) == NULL) {
		printf("ERROR - cannot open  \n");
	}
	else {
		cout << "____________________________________________________________________________\n";
		cout << "Sam Arnold\nCSCI 434, Spring 2017\n";
		cout << "This program Takes Input From File Broken into Lexemes, Tokens, And Symbols " << endl;
		cout << "____________________________________________________________________________\n\n";
		cout << "--------------\n\n";
		cout << " || TABLE ||\n\n";
		cout << "--------------\n\n";

		Line = 0;


		Line = 0;
		getChar();
		// check if the beggining is okay before loop
		Check_Beginning();
		do {
			if (ErrorFlag == false) {

				// next token please
				lex();

				// new syntax script
				NewSynt();
				//expr();
			}
			else
				break;

			// check to make sure not at end of file
		} while (nextToken != EOF);

		//Error Thrown If Factor went wrong
		if (ErrorFlag == false)
			Valid();
		if (ErrorFlag != false)
			error();
	}



	// End of Program
	system("pause");
	return 0;
}
void Check_Beginning() {
	string Fullword ;
	char lexemeWord ;
	lex();
		// Convert to string to test. 
		for (int i = 0; i < lexLen; i++)
		{
			lexemeWord = lexeme[i];
			Fullword += lexemeWord;
		}
	
		if ((lexeme == "BEGIN") || (Fullword == "BEGIN"))
			cout << endl;
		//ELSE: Does not begin error
		else {
			ErrorFlag = true;
			cout << "\n**********************************" << endl;
			cout << "INVALID. PROGRAM DOES NOT BEGIN WITH <BEGIN> " << endl; 
			cout << "\n**********************************" << endl;
			
		}
	}

//}
void NewSynt() {
	Check_Declarer();

}
void Check_Declarer() {
	// Check for invalid Characters
	if (nextToken == -1) {
		cout << "\n**********************************" << endl; 
		cout << "\nERROR INVALID CHARACTER AT LINE :" << Line << endl; 
		cout << "\n**********************************" << endl;
		ErrorFlag = true;
	}

	//Check for correct characters
	if ((nextToken == RESERVEDWORD) || (nextToken == COMMA) || (nextToken == IDENT) || (lexeme == "OUTPUT") || (lexeme == "INPUT")) {
		//Stil Valid CHK_Declartion;
		if (lexeme == "OUTPUT") {
			if (ErrorFlag == false) {
				lex();
				Check_Output();
			}
		}
		else
			lex();
	}
	else
		// check if syntax for Declartion
		if ((nextToken == QUOTE) || (isLiterial() == true) || (nextToken = LITERAL) || (nextToken = EOF))
			cout << "";
		else {
			// DOES not follow Declaration
			ErrorFlag = true;
			cout << "\n**********************************" << endl;
			cout << "ERROR : PROGRAM DOES NOT Follow STANDARD FOR DECLARATION AT LINE " <<Line  << endl;
			cout << "\n**********************************" << endl;
		}
}

void Check_Output() {
	if ((nextToken == RESERVEDWORD) || (nextToken == IDENT)
		|| (nextToken == QUOTE) || (isLiterial() == true) || (nextToken = LITERAL)) {
		//while valid
		lex();
	}
	else
		ErrorFlag = true;
}
void expr()
{
	term();
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
}
void error() // Error Message
{
	cout << "____________________________________________________________________________\n\n";
	cout << "******************************************************************************\n\n";
	printf("Input is Invalid: \n\n");
	cout << "******************************************************************************\n";
	cout << "____________________________________________________________________________\n\n";
}

void Valid() // Validate Message
{
	cout << "____________________________________________________________________________\n\n";
	cout << "******************************************************************************\n\n";
	printf("Input is valid: \n\n");
	cout << "******************************************************************************\n";
	cout << "____________________________________________________________________________\n\n";
}

void term() // Evaluate Term
{
	factor();
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		lex();
		factor();
	}
}
void factor() // Evaluate Factor
{
	if ((nextToken == IDENT) || (nextToken == INT_LIT))
		lex();
	else if (nextToken == LEFT_PAREN)
	{
		lex();
		expr();
		if (nextToken == RIGHT_PAREN) {
			lex();
		}
		else
			ErrorFlag = true;
	}
	else 	ErrorFlag = true;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses            and return the token */
int lookup(char ch)
{
	switch (ch) {
	case '(':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: LEFT_PAREN ";
		//myfile << "Line #: " << endl << endl;
		nextToken = LEFT_PAREN;
		L_Par += 1;
		break;

	case ')':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: RIGHT_PAREN ";
		nextToken = RIGHT_PAREN;
		R_Par += 1;
		break;
	case '+':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: ADD_OP ";
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: SUB_OP ";
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: MULT_OP ";
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: DIV_OP ";
		nextToken = DIV_OP;
		break;
	case ',':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: COMMA ";
		nextToken = COMMA;
		break;
	case '"':
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: QUOTE ";
		nextToken = QUOTE;
		break;
	case '\n':
		Line += 1;
		cout << "Line";
		break;
	default:
		addChar();
		cout << left << setw(32) << setfill(' ') << "Type: EOF_OP ";
		nextToken = EOF;
		break;
	}
	return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}

	else
		printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */

void getChar()
{
	if (nextChar == '\n') {
		Line += 1;
		cout << "____________________________________________________________________________\n";
		cout << "Line " << Line << endl;
		cout << "____________________________________________________________________________\n\n";
	}

	if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
	while (isspace(nextChar))
		getChar();
}
// ***************************************************** /
/* lex - a simple lexical analyzer for arithmetic expressions */

bool isReserved() {
	// check if lexeme is a reserved word 
	bool reserveme = false;
	string Fullword; 
	char lexemeWord;
	for (int i = 0; i < lexLen; i++) 
	{
	    lexemeWord = lexeme[i];
		Fullword += lexemeWord;
	}
	if (Fullword == "Begin" || Fullword == "BEGIN" || Fullword == "END" || Fullword == "End" || Fullword == "CHAR" || Fullword == "STRING"
		|| Fullword == "String" || Fullword == "EQ" || Fullword == "Eq" || Fullword == "NEG" || Fullword == "Neg" || Fullword == "Output"
		|| Fullword == "OUTPUT" || Fullword == "Char" || Fullword == "Input" || Fullword == "INPUT" || Fullword == "END" || Fullword == "end")
		reserveme = true;

	return reserveme;
}

bool isLiterial(){
	// checks if lexme is literial word
	bool myAnswer = false; 
	int parenthesiscount = 0; 

	string Fullword;
	char lexemeWord;
	// convert char / string
	for (int i = 0; i < lexLen; i++)
	{
		lexemeWord = lexeme[i];
		Fullword += lexemeWord;
	}
	// look for parenthesis
	for (int i =0; i <  Fullword.length(); i++)
		if (Fullword[i] == '"')
			parenthesiscount += 1;
	//cout << "Count" << parenthesiscount;

	if ( nextChar  == '"')
		myAnswer = true;

	if (parenthesiscount > 0)
		myAnswer = true; 
	return myAnswer;
}

int lex()
{
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */

	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		// chenck for reserved word
		if (isReserved() == true) {
			cout << left << setw(32) << setfill(' ') << "Type: RESERVEDWORD ";
			nextToken = RESERVEDWORD;
		}
		else if (isLiterial() == true) {
			cout << left << setw(32) << setfill(' ') << "Type: <Letter Sequence> ";
			nextToken = LITERAL;
		}

		else {
			// change INdentifier
			cout << left << setw(32) << setfill(' ') << "Type: Letter ";
			nextToken = IDENT;
		}
		break;
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		cout << left << setw(32) << setfill(' ') << "Type: Digit";
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		cout << left << setw(32) << setfill(' ') << "Type: EOF ";
		if (L_Par != R_Par)
			ErrorFlag = true;
		break;
	} /* End of switch */



	// Create file
	ofstream myfile;					 //output file
	myfile.open("Output.dat", std::fstream::in | std::fstream::out | std::fstream::app);


	cout << left << setw(12) << setfill(' ') << "     Line is: " << Line;
	cout << left << setw(15) << setfill(' ') << "     Token is: " << nextToken;
	cout << left << setw(25) << setfill(' ') << "     Lexeme is: " << lexeme;
	if (nextToken == EOF) {
			myfile << "____________________________________________________________________________\n";
			myfile << "Sam Arnold\nCSCI 434, Spring 2017\n";
			myfile << "____________________________________________________________________________\n\n";
		}
    myfile << left  << left << setw(12) << setfill(' ') << "     Line is: " << Line ;
	myfile << left << setw(15) << setfill(' ') << "     Token is: " << nextToken ;
	myfile << left << setw(25) << setfill(' ') << "     Lexeme is: " << lexeme << endl;

	cout << "\n";
	return nextToken;
} /* End of function lex */