%{
#include <asl_core.h>
#include <asl_language_parser.h>

void yyerror(char *);
int yywrap(void){
	return 1;
}
void yyerror(char *s){
	exception_fatal_exc("File:%s on line %d compile failed!" ,global_compiler.input_item->filename,global_compiler.input_item->scan_line);
}

%}

%start COMMENT

%%

<INITIAL>"function"				  	return T_FUNCTION;
<INITIAL>"global"					return T_GLOBAL;
<INITIAL>"for"						return T_FOR;
<INITIAL>"if"						return T_IF;
<INITIAL>"else"						return T_ELSE;
<INITIAL>"break"					return T_BREAK;
<INITIAL>"continue"					return T_CONTINUE;
<INITIAL>"return"					return T_RETURN;
<INITIAL>"do"						return T_DO;
<INITIAL>"while"					return T_WHILE;
<INITIAL>"include"					return T_INCLUDE;
<INITIAL>"true"                     return T_TRUE;
<INITIAL>"false"                    return T_FALSE;
<INITIAL>"null"                     return T_NULL;
<INITIAL>","						return T_COMMA;
<INITIAL>"+"						return T_PLUS;
<INITIAL>"-"						return T_SUB;
<INITIAL>"/"						return T_DIV;
<INITIAL>"*"						return T_MUL;
<INITIAL>"="						return T_ASSIGN;
<INITIAL>"=="						return T_EQ;
<INITIAL>"!="						return T_NEQ;
<INITIAL>">"						return T_GT;
<INITIAL>">="						return T_GE;
<INITIAL>"<"						return T_LT;
<INITIAL>"<="						return T_LE;
<INITIAL>"=<"						return T_LE;
<INITIAL>"("						return T_LP;
<INITIAL>")"						return T_RP;
<INITIAL>"{"						return T_LBRACE;
<INITIAL>"}"						return T_RBRACE;
<INITIAL>";"						return T_SEMICOLON;


<INITIAL>[A-Za-z_][A-Za-z_0-9]*		{
    char *s;
    int len;
    len = strlen(yytext);
    s = compiler_find_token(yytext,len);
    if(is_empty(s)){
        s = (char *) memory_alloc(sizeof(yytext));
        strcpy(s, yytext);
        compiler_insert_token(s,len);
	}
	yylval.identifier = s;
	return T_IDENTIFIER;
}

<INITIAL>$[A-Za-z_][A-Za-z_0-9]*		{
    char *s;
    int len;
    len = strlen(yytext);
    s = compiler_find_token(yytext,len);
    if(is_empty(s)){
        s = (char *) memory_alloc(sizeof(yytext));
        strcpy(s, yytext);
        compiler_insert_token(s,len);
	}
	yylval.variable = s;
	return T_VARIABLE;
}

<INITIAL>([1-9][0-9]*)|"0"			{
	int digit;
	sscanf(yytext,"%d",&digit);
	yylval.digit = digit;
	return T_LITERAL_DIGIT;
}

<INITIAL>[0-9]+\.[0-9]+				{
	double db;
	sscanf(yytext,"%lf",&db);
	return T_LITERAL_DOUBLE;
}

<INITIAL>L?\"(\\.|[^\\"])*\"		{
	yylval.identifier = util_substring(yytext ,1 ,strlen(yytext) -2);
	return T_LITERAL_STRING;
}

<INITIAL>[ \t\r]					;

<INITIAL>"\n"						{
	global_compiler.input_item->scan_line++;
}

<INITIAL>("#"|"//")[^\n]*			{
	BEGIN COMMENT;
}

<INITIAL>.							{
	global_compiler.input_item->scan_line++;
	yyerror(yytext);
}

<COMMENT>\n							{
	global_compiler.input_item->scan_line++;
	BEGIN INITIAL;
}

<COMMENT>.							;
