%{

#include <asl_core.h>

#define YYDEBUG 1

%}

%union {
    char *variable;
	char *identifier;
	int digit;
	double db;
	exp_t *expression;
	exp_list_t *exp_list;
	statement_t *statement;
	statement_list_t *statement_list;
	func_t *function;
	func_param_list_t *func_param_list;
	func_param_t *func_param;
}

%token <digit>			        T_LITERAL_DIGIT
%token <db>				        T_LITERAL_DOUBLE
%token <identifier>		        T_FUNCTION T_GLOBAL T_FOR T_IF T_ELSE T_RETURN
						        T_INCLUDE T_CONTINUE T_BREAK T_WHILE T_DO T_TRUE T_FALSE T_NULL
						        T_PLUS T_SUB T_MUL T_DIV T_ASSIGN
						        T_EQ T_NEQ T_GE T_GT T_LE T_LT
						        T_LP T_RP T_LBRACE T_RBRACE T_SEMICOLON T_COMMA
						        T_IDENTIFIER T_LITERAL_STRING
%token <variable>               T_VARIABLE

%type <func_param>				param
%type <func_param_list>			param_list
%type <exp_list>			    call_param_list
%type <function>				function_declaration
%type <expression>				equation_exp relation_exp addition_exp
								multiplication_exp primary_exp option_exp
								primary_prime_exp primary_variable_exp primary_call_exp
								all_exp
%type <statement_list>			statement_block statement_list
%type <statement>				all_statement exp_statement return_statement continue_statement
								break_statement for_statement if_statement do_while_statement
								while_statement include_statement unlimited_statement limited_statement

%%

translation_unit: function_declaration
	| unlimited_statement
	{
		compiler_insert_global_statement_list($1);
	}
	| translation_unit unlimited_statement
	{
		compiler_insert_global_statement_list($2);
	}
	| translation_unit limited_statement
	{
		compiler_insert_global_statement_list($2);
	}
	| translation_unit function_declaration;

function_declaration: T_FUNCTION T_IDENTIFIER T_LP param_list T_RP statement_block
	{
		compiler_declare_func($2 ,$4 ,$6);
	}
	| T_FUNCTION T_IDENTIFIER T_LP T_RP statement_block
	{
		compiler_declare_func($2 ,NULL ,$5);
	};

param_list: param
	{

		$$ = compiler_insert_param_list(compiler_create_param_list(),$1);
	}
	| param_list T_COMMA param
	{
		$$ = compiler_insert_param_list($1 ,$3);
	};

param: T_IDENTIFIER T_VARIABLE T_ASSIGN primary_prime_exp
	{
		$$ = compiler_create_param($2 ,$1 ,$4);
	}
	| T_VARIABLE T_ASSIGN primary_prime_exp
	{
		$$ = compiler_create_param($1 ,NULL ,$3);
	}
	| T_IDENTIFIER T_VARIABLE
	{
	    $$ = compiler_create_param($2 ,$1 ,NULL);
	}
	| T_VARIABLE
	{
		$$ = compiler_create_param($1 ,NULL ,NULL);
	};

unlimited_statement: exp_statement
	| for_statement
	| if_statement
	| return_statement
	| include_statement
	| do_while_statement
	| while_statement;

limited_statement: break_statement
	| continue_statement;

all_statement: unlimited_statement
	| limited_statement;

exp_statement: all_exp T_SEMICOLON
	{
		$$ = compiler_create_exp_statement($1);
	};

include_statement: T_INCLUDE all_exp T_SEMICOLON
	{
		$$ = compiler_create_include_statement($2);
	};

if_statement: T_IF T_LP all_exp T_RP statement_block
	{
		$$ = compiler_create_if_statement($3 ,$5 ,NULL);
	}
	| T_IF T_LP all_exp T_RP statement_block T_ELSE statement_block
	{
		$$ = compiler_create_if_statement($3 ,$5 ,$7);
	};

for_statement: T_FOR T_LP option_exp T_SEMICOLON option_exp T_SEMICOLON option_exp T_RP statement_block
	{
		$$ = compiler_create_for_statement($3 ,$5 ,$7 ,$9);
	};

do_while_statement: T_DO statement_block T_WHILE T_LP all_exp T_RP T_SEMICOLON
	{
		$$ = compiler_create_while_statement($5 ,$2 ,1);
	};

while_statement: T_WHILE T_LP all_exp T_RP statement_block
	{
		$$ = compiler_create_while_statement($3 ,$5 ,0);
	};

break_statement: T_BREAK T_SEMICOLON
	{
		$$ = compiler_create_break_statement();
	};

continue_statement: T_CONTINUE T_SEMICOLON
	{
		$$ = compiler_create_continue_statement();
	};

return_statement: T_RETURN option_exp T_SEMICOLON
	{
		$$ =compiler_create_return_statement($2);
	};

statement_list: all_statement
	{
		$$ = compiler_create_statement_list($1);
	}
	| statement_list all_statement
	{
		$$ = compiler_insert_statement_list($1 ,$2);
	};

statement_block: T_LBRACE T_RBRACE
	{
		$$ = NULL;
	}
	| T_LBRACE statement_list T_RBRACE
	{
		$$ = $2;
	};

all_exp: equation_exp
	| T_VARIABLE T_ASSIGN all_exp
	{
		$$ = compiler_create_assign_exp($1 ,$3);
	};

equation_exp: relation_exp
	| equation_exp T_EQ relation_exp
	{
		$$ = compiler_create_binary_exp(binary_type_eq ,$1 ,$3);
	}
	| equation_exp T_NEQ relation_exp
	{
		$$ = compiler_create_binary_exp(binary_type_neq ,$1 ,$3);
	};

relation_exp: addition_exp
	| relation_exp T_LT addition_exp
	{
		$$ = compiler_create_binary_exp(binary_type_lt ,$1 ,$3);
	}
	| relation_exp T_LE addition_exp
	{
		$$ = compiler_create_binary_exp(binary_type_le ,$1 ,$3);
	}
	| relation_exp T_GT addition_exp
	{
		$$ = compiler_create_binary_exp(binary_type_gt ,$1 ,$3);
	}
	| relation_exp T_GE addition_exp
	{
		$$ = compiler_create_binary_exp(binary_type_ge ,$1 ,$3);
	};

addition_exp: multiplication_exp
	| addition_exp T_PLUS multiplication_exp
	{
		$$ = compiler_create_binary_exp(binary_type_plus ,$1 ,$3);
	}
	| addition_exp T_SUB multiplication_exp
	{
		$$ = compiler_create_binary_exp(binary_type_sub ,$1 ,$3);
	};

multiplication_exp: primary_exp
	| multiplication_exp T_DIV primary_exp
	{
		$$ = compiler_create_binary_exp(binary_type_div ,$1 ,$3);
	}
	|multiplication_exp T_MUL primary_exp
	{
		$$ = compiler_create_binary_exp(binary_type_mul ,$1 ,$3);
	};

primary_exp: primary_prime_exp
    | primary_variable_exp
    | primary_call_exp;

primary_prime_exp:T_LITERAL_STRING
	{
		$$ = compiler_create_string_exp($1);
	}
	| T_LITERAL_DIGIT
	{
		$$ = compiler_create_integer_exp($1 ,0);
	}
	| T_SUB T_LITERAL_DIGIT
    {
     	$$ = compiler_create_integer_exp($2 ,1);
    }
	| T_LITERAL_DOUBLE
	{
		$$ = compiler_create_double_exp($1 ,0);
	}
	|T_SUB T_LITERAL_DOUBLE
    {
     	$$ = compiler_create_double_exp($2 ,1);
    }
	| T_TRUE
	{
	    $$ = compiler_create_value_exp(exp_value_defined_true);
	}
	| T_FALSE
	{
	    $$ = compiler_create_value_exp(exp_value_defined_false);
	}
	| T_NULL
	{
	    $$ = compiler_create_value_exp(exp_value_defined_null);
	};

primary_variable_exp: T_VARIABLE
    {
        $$ = compiler_create_var_exp($1 ,0);
    }
    | T_SUB T_VARIABLE
    {
        $$ = compiler_create_var_exp($1 ,1);
    };

primary_call_exp: T_IDENTIFIER T_LP call_param_list T_RP
    {
        $$ = compiler_create_call_func_exp($1 ,$3, 0);
    }
    | T_SUB T_IDENTIFIER T_LP call_param_list T_RP
    {
        $$ = compiler_create_call_func_exp($1 ,$4, 1);
    };

option_exp: all_exp
	| T_LP all_exp T_RP
    {
        $$ = $2;
    }|
    {
		$$ = NULL;
	};


call_param_list: all_exp
	{
		$$ = compiler_create_exp_list($1);
	}
	| call_param_list T_COMMA all_exp
	{
		$$ = compiler_insert_exp_list($1 ,$3);
	}
	|
	{
	    $$ = NULL;
	};