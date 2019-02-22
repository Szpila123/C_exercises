#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "lists.h"

#define max_len 100

//Structre for passing args to SentenceEntered
typedef struct{
		GtkWidget *text;
		GtkWidget *set_entry;
	} text_and_set;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
		
//Funcion from gtk_window.c
extern void Show_error( GtkWidget *window, char *info );

//Function called after entering set and formula 
void SentenceEntered( GtkWidget *widget, void *data );

//Function getting the set power from set info ( set info has fixed template see README )
static relations *RelationsInfo( GtkWidget *widget );

//Counting newlines in text till the nth character
static int CountNewlines( char *text, int pos );

//Analyzes the formula and if needed constructs a counterexample
static void Analyze( GtkWidget *window, char* formula, relations *rels );

//Checks if formula is correct
static int Check_formula( GtkWidget *window, char* formula, relations *rels );

//Converts the number from string to int and checks if its nonzero and no more than max, if it is calls Show_error on window
static bool Convert_num_and_check( char* text, int start, int max, GtkWidget *window );

//Check if the character is a digit, big letter or small letter (if its a letter that is an operator returns false)
static int IsDigitOrNum( char c );

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void SentenceEntered( GtkWidget *widget, void *data )
{
	text_and_set *txt_and_set = data;

	char formula[max_len + 2];

//Extracting formula from data
	strcpy( formula, gtk_entry_get_text( GTK_ENTRY( txt_and_set->text ) ) );


//Extracting set and relations info
	relations *rels = RelationsInfo( txt_and_set->set_entry );
	if( rels == NULL ) return;

//Checking if formula is correct:
	if( Check_formula( gtk_widget_get_toplevel( widget ), formula, rels ) ){
		FreeRelationsSpace( rels );
		return;
	}

//Analizing formula
	Analyze( gtk_widget_get_toplevel( widget ), formula,  rels );
	

//Cleaning memory
	FreeRelationsSpace( rels );
	return;
}
	

static relations* RelationsInfo( GtkWidget *set_entry )
{
	
	GtkTextIter start, end;
	relations *rels = malloc( sizeof( relations ) );
	if( rels == NULL ){
		Show_error( gtk_widget_get_toplevel( set_entry ),
			"Error occured during memory reservation" );
		return NULL;
	}
		
	*rels = (relations) { 0, 0, NULL };
	int pos = 0;
	char error[100];
	
//Getting string from text_view frame 
	GtkTextBuffer *buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW( set_entry ) );
	gtk_text_buffer_get_bounds( buffer, &start, &end );
	char *set = gtk_text_buffer_get_text( buffer, &start, &end, FALSE );

//Checking if input format is right
	while( set[pos] != '\0' || pos == 0 ){
		//First entry should be a set power
		if( pos == 0 ){
			//Skipping to first entered number (the power of the set)
			while( ( set[pos] < '0' || set[pos] > '9' )  && set[pos] != '\0' ) pos++;
			//Checking if a number exist and if it contains digits only
			for( ; set[pos] != '\n' ; pos++ )
				if( set[pos] < '0' || set[pos] > '9' ){
					if( set[pos] == '\0' ){
						snprintf( error, 100, "Line %d: Please enter the power of set in the first line and pleace a newline at the end"
							, CountNewlines( set, pos )  );
						Show_error( gtk_widget_get_toplevel( set_entry ), error );
					}
					else{
						snprintf( error, 100, "Line %d: Set power should be a number"
							 , CountNewlines( set, pos )  );
						Show_error( gtk_widget_get_toplevel( set_entry ), error );
					}
					free(rels);
					free(set);
					return NULL;
				}
			//Skipping to next entered thing and checking if its a relation name
			while( set[pos] == ' ' || set[pos] == '\n' ) pos++;
			if( set[pos] < 'A' || set[pos] > 'Z' ){
				snprintf( error, 100, "Line %d: Please enter the first relation's name (big letter)"
					, CountNewlines( set, pos )  );
				Show_error( gtk_widget_get_toplevel( set_entry ), error );
				free(set);
				free(rels);
				return NULL;
			}
		}
		//Checking format of relation entries
		else{
			//If relation name is entered it should be placed in a separate newline
			if( set[pos] >= 'A' && set[pos] <= 'Z' ){
				if( set[pos] == 'E' || set[pos] == 'V' ){
					snprintf( error, 100, "Line %d: This relation name is reserved"
						, CountNewlines( set, pos )  );
					Show_error( gtk_widget_get_toplevel( set_entry ), error );
					free(set);
					free(rels);
					return NULL;
				}
				for( int a = -1 ; a < 2 ; a += 2 )
					if( set[pos+a] != '\n' && set[pos+a] != '\0' ){
						snprintf( error, 100, "Line %d: Every relation name should be placed as a single character in a separate line"
							, CountNewlines( set, pos )  );
						Show_error( gtk_widget_get_toplevel( set_entry ), error );
						free(set);
						free(rels);
						return NULL;
					}
			}
			//If reletion element is entered it should be two numbers separated with space
			else if( set[pos] >=  '0' && set[pos] <= '9' ){
				while( set[pos] >= '0' && set[pos] <= '9' ) pos++;
				if( set[pos] != ' ' ){
					snprintf( error, 100, "Line %d: Relation elements should be separated with a space" 
						, CountNewlines( set, pos )  );
					Show_error( gtk_widget_get_toplevel( set_entry ), error );
					free(set);
					free(rels);
					return NULL;
				}	
				pos++; //Skipping through space
				if( set[pos] < '0' || set[pos] > '9'){
					snprintf( error, 100, "Line %d: Second number in one relation element is missing or numbers are separated with two spaces"
						, CountNewlines( set, pos )  );
					Show_error( gtk_widget_get_toplevel( set_entry ), error );
					free(set);
					free(rels);
					return NULL;
				}
				while( set[pos] >= '0' && set[pos] <= '9' ) pos++;
				if( set[pos] != '\n' && set[pos] != '\0' ){
					snprintf( error, 100, "Line %d: Please end every relation element entry with newline"
						, CountNewlines( set, pos )  );
					Show_error( gtk_widget_get_toplevel( set_entry ), error );
					free(set);
					free(rels);
					return NULL;
				}

			}
			//If a character is none of above or newline or space it is unrecognized
			else if( set[pos] != '\n' && set [pos] != ' ' ){
				snprintf( error, 100, "Line %d: Unrecognized character entered"
					, CountNewlines( set, pos )  );
				Show_error( gtk_widget_get_toplevel( set_entry ), error );
				free(set);
				free(rels);
				return NULL;
			}
			//Move only if the end of the file isnt reached
			if( set[pos] != '\0' )pos++;
		}
	}

	pos = 0;

//Getting set power
	while( set[pos] > '9' || set[pos] < '0' ) pos ++;
	for( ;  set[pos] != '\0' && set[pos] != '\n' ; pos ++ ){
			rels->power *= 10; 
			rels->power += set[pos] - '0';
			if( rels->power > 1000 ){
				Show_error( gtk_widget_get_toplevel( set_entry ), "Power of the set is too big" );
				free(set);
				free(rels);
				return NULL;
			}
	}

//Counting all relations ( counting big letters in set )
	for( int i = pos + 1 ; set[i] != '\0' ; i ++ )
		if( set[i] >= 'A' && set[i] <= 'Z' )
			rels->num_of_rels ++;

//Creating array for all relations
	rels->rel = calloc( rels->num_of_rels, sizeof( lhead ) );
	if( rels->rel == NULL ){
		Show_error( gtk_widget_get_toplevel( set_entry ), "Error occured during memory reservation" );
		free(set);
		free(rels);
		return NULL;
	}
	for( int i = 0 ; i < rels->num_of_rels ; i++ ) rels->rel[i].elem = NULL;

//Adding all relations names to array and relations elements to lists
	int num_of_add_rels = 0;
	int a, b;
	while(  set[pos] != '\0' ){
		//Big letter means that new relation will be entered
		if( pos[set] >= 'A' && pos[set] <= 'Z' ){
			//Checking if the character isnt already in use
			for( int i = 0 ; i < num_of_add_rels ; i ++ )
				if( set[pos] == rels->rel[i].name ){
					snprintf( error, 100, "Line %d: Second relation with the same name- %c"
						, CountNewlines( set, pos ), set[pos]  );
					Show_error( gtk_widget_get_toplevel( set_entry ), error );
					free(set);
					FreeRelationsSpace( rels );
					return NULL;	
				}
			//Adding name of the relation and all its elements	
			rels->rel[num_of_add_rels].name = pos[set];
			pos++;
			while( ( set[pos] < 'A' || set[pos] > 'Z' ) && set[pos] != '\0' ){	
				//Skipping to first digit and saving two numbers
				if( set[pos] < '0' || set[pos] > '9' ) pos++;
				else{
					a = 0;
					b = 0;
					while( set[pos] != ' ' ){
						a *= 10;
						a += set[pos] - '0';
						pos++;
						//Checking if a isnt too big
						if( a > rels->power ){
							snprintf( error, 100, "Line %d: First element is bigger than a set power"
								, CountNewlines( set, pos ) );
							Show_error( gtk_widget_get_toplevel( set_entry ), 
								error);
							free(set);
							FreeRelationsSpace( rels );
							return NULL;
						}
					}
					pos++;
					while( set[pos] >= '0' && set[pos] <= '9' ){
						b *= 10;
						b += set[pos] - '0';
						pos++;
						//Checking if b isnt too big
						if( b > rels->power ){
							snprintf( error, 100, "Line %d: Second element is bigger than a set power"
								, CountNewlines( set, pos ) );
							Show_error( gtk_widget_get_toplevel( set_entry ), 
								error);
							free(set);
							FreeRelationsSpace( rels );
							return NULL;
						}
					}
					//Checking if numbers arent zeros 
					if(  a == 0 || b == 0 ){
						snprintf( error, 100, "Line %d: Element cant be a 0", CountNewlines( set, pos ) );	
						Show_error( gtk_widget_get_toplevel( set_entry ), 
							error);
						free(set);
						FreeRelationsSpace( rels );
						return NULL;	
					}	
					if( AddElem( &(rels->rel[num_of_add_rels].elem), a, b )){
						Show_error( gtk_widget_get_toplevel( set_entry ), 
							"Error occured during memory reservation" );
						free(set);
						FreeRelationsSpace( rels );
						return NULL;	
					}
				}
			}		
		num_of_add_rels++;
		}			
		else pos ++;
	}

	free(set);
	return rels;
}

static int CountNewlines( char* text, int pos )
{
	int newlines = 0;
	for( int i = 0 ; i < pos ; i ++ )
		if( text[i] == '\n' )	newlines ++;
	return newlines + 1 ;
}


static int Check_formula( GtkWidget *window, char* formula, relations *rels )
{
	int pos = 0;
	lnode* variables = NULL;
	lnode* parenthesis = NULL;
	char error[100];

	//Skipping to the first character
	while( formula[pos] == ' ' ) pos++;

	//Checking if formula is empty
	if( formula[pos] == '\0' ){
		Show_error( window, "Formula is empty" );	
		return 1;
	}
	
	//Checking syntax
	for( int pos = 0 ; formula[pos] != '\0' ; pos ++ ){
	//Checking quantifiers and adding tied variables
		if( formula[pos] == 'E' || formula[pos] == 'V' ){
			if( formula[pos+1] < 'a' || formula[pos+1] > 'z' || formula[pos+1] == 'v' ){
				snprintf( error, 100, "Formula chracter %d: Expected variable", pos+2);
				Show_error( window, error );	
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}
			pos++;
			if( CheckListElem( variables, (int) formula[pos], 0 ) ){
				snprintf( error, 100, "Formula character %d: Second variable with the same name %c ", pos+1, formula[pos] );
				Show_error( window, error );
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}

			if( AddElem( &variables, (int) formula[pos], 0 ) ){
				Show_error( window, "Error occured during memory reservation" );
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}
		}

	//Checking brackets placement
		else if( formula[pos] == '(' ){
			int brackets = 1, temp = pos;
			bool empty = true;
			//Looking for closing bracket
			while( brackets  && formula[temp] != '\0' ){
				temp++;
				if( formula[temp] == '(' ) brackets++;
				else if( formula[temp] == ')' ) brackets--;
				else if( formula[temp] >= 'A' && formula[temp] <= 'Z' && formula[temp] != 'E' && formula[temp] != 'V' ) empty = false;
			}
			//Checking if bracket was found and if its non empty
			if( brackets || empty ){
				if( brackets ) snprintf( error, 100, "Formula: Bracket on position %d isnt closed", pos+1 );
				else snprintf( error, 100, "Formula: Brackets on positions %d to %d dont contain any relations", pos+1, temp+1 );
				Show_error( window, error );
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}
			if( AddElem( &parenthesis, temp, 0 ) ){
				Show_error( window, "Error occured during memory reservation" );
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}

		}
		//Checking if closing bracket has its opening
		else if( formula[pos] == ')' ){
			if( !CheckListElem( parenthesis, pos, 0 ) ){
				snprintf( error, 100, "Formula: Bracket on position %d dosnt have opening bracket", pos+1 );
				Show_error( window, error );
				FreeMemory( variables );
				FreeMemory( parenthesis );
				return 1;
			}
		}


		//When the next character is a number or variable it has to be attached to relation (sth like xRx / 123R1)
		else if( formula[pos] >= '0' && formula[pos] <= '9' || formula[pos] >= 'a' && formula[pos] <= 'z' && formula[pos] != 'v' ){
			//Checking if its a number and if its correct
			if( formula[pos] >= '0' && formula[pos] <= '9' ){
				if( Convert_num_and_check( formula, pos, rels->power, window  ) ){
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
				while( formula[pos] >= '0' && formula[pos] <= '9' ) pos++;
			}
			//If its not a number it has to be a variable, checking if it was declared
			else{
				if( !CheckListElem( variables, (int) formula[pos], 0 ) ){
					snprintf( error, 100, "Formula: variable on position %d isnt declared", pos+1);
					Show_error( window, error );
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
				pos++;
			}
			//Next there should be a relations name
			if( formula[pos] < 'A' || formula[pos] > 'Z' || formula[pos] == 'E' || formula[pos] == 'V' ){
				snprintf( error, 100, "Formula: expected relation name on position %d", pos+1);
				Show_error( window, error );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
			//Checking if the relation was declared
			else{
				bool found = false;
				for( int i = 0 ; i < rels->num_of_rels ; i++ )
					if( rels->rel[i].name == formula[pos] ){
						found = true;
						break;
					}	
				if( !found ){
					snprintf( error, 100, "Formula: on position %d relations name wasnt declared", pos+1 );
					Show_error( window, error );
					FreeMemory( variables );
					FreeMemory( parenthesis );
					return 1;
					}
			}
			pos++;
			//The second variable or number goes same as before...
			if( formula[pos] >= '0' && formula[pos] <= '9' ){
				if( Convert_num_and_check( formula, pos, rels->power, window  ) ){
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
				while( formula[pos+1] >= '0' && formula[pos+1] <= '9' ) pos++;
			}
			else{
				if( !CheckListElem( variables, (int) formula[pos], 0 ) ){
					if( formula[pos] > 'z' || formula[pos] < 'a' || formula[pos] == 'v')
						snprintf( error, 100, "Formula: expected variable or number on position %d", pos+1);
					else snprintf( error, 100, "Formula: variable on position %d isnt declared", pos+1);
					Show_error( window, error );
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
			}
		}
		// The negation should be placed before relation element or opening bracket
		else if( formula[pos] == '~' ){
			while( formula[pos] == ' ' ) pos++;
			if( formula[pos] == '(' ) pos --;
			else if( formula[pos] >= 'a' && formula[pos] <= 'z' || formula[pos] >= '0' && formula[pos] <= '9' ) pos--;
			else{
				snprintf( error, 100, "Formula: on position %d expected bracket or relation after negations sign", pos+1);
				Show_error( window, error );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
		}
		// All other logical symbols have two arguments (placed before and after the symbol): relation elem or formula in brackets
		else if( formula[pos] == '<' || formula [pos] == '=' || formula[pos] == '&' || formula[pos] == 'v' ){
			int beg = pos-1;
			int end = pos + 1;
			if( formula[pos] == '<' ){
				if( formula[pos+1] != '=' || formula[pos+2] != '>' ){
					snprintf( error, 100, "Formula: on position %d unrecognized character", pos+1 );
					Show_error( window, error );
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
				end = pos + 3;
				pos+=2;
			}
			else if( formula[pos] == '=' ){
				if( formula[pos+1] != '>' ){
					snprintf( error, 100, "Formula: on position %d unrecognized character", pos+1 );
					Show_error( window, error );
					FreeMemory( parenthesis );
					FreeMemory( variables );
					return 1;
				}
				end = pos + 2;
				pos+=1;
			}
			//Checking what is placed before the operator
			while( formula[beg] == ' ' && beg > 0 ) beg--;
			if( beg == 0 ){
				Show_error( window, "Formula starts with an oparator" );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
			if( ( formula[beg] != ')' && (formula[beg] < '0' || formula[beg] > '9') && (formula[beg] < 'a' || formula[beg] > 'z') )
			   || formula[beg] == 'v' ){
				snprintf( error, 100, "Formula: on position %d operators letf argument is wrong", pos+1);
				Show_error( window, error );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
			if( formula[beg] >= 'a' && formula[beg] <= 'z' && (formula[beg-1] == 'E' || formula[beg-1] == 'V' ) ){
				snprintf( error, 100, "Formula: on position %d declaration as operators left argument", pos+1 );
				Show_error( window, error );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
			//Checking what is placed after the operator
			while( formula[end] == ' ' ) end ++;
			if( formula[end] == '\0' ){
				Show_error( window, "Formula: expected second argument for the last operator" );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
			if( (formula[end] != '~' &&  formula[end] != '(' && (formula[end] < '0' || formula[end] > '9') && (formula[end] < 'a' || formula[end] > 'z') )
		 	   || formula[end] == 'v' ){
				snprintf( error, 100, "Formula: on position %d operators right argument is wrong", pos+1);
				Show_error( window, error );
				FreeMemory( parenthesis );
				FreeMemory( variables );
				return 1;
			}
		}
		else if( formula[pos] != ' ' ){
			snprintf( error, 100, "Formula: unrecognized character entered on position %d", pos+1 );
			Show_error( window, error );
			FreeMemory( parenthesis );
			FreeMemory( variables );
			return 1;
		}
	}
	FreeMemory( parenthesis );
	FreeMemory( variables );
	return 0;
}

static bool Convert_num_and_check( char* text, int start, int max, GtkWidget *window )
{
	int num = 0;	
	int pos = start;
	while( text[pos] >= '0' && text[pos] <= '9' ){
		num *= 10;
		num += text[pos] - '0';
		if( num > max ){
			char error[100];
			snprintf( error, 100, "Formula: number on positon %d is bigger than set's power", start+1 );
			Show_error( window, error );
			return true;
		}
		pos++;
	}
	if( num == 0 ){
		char error[100];
		snprintf( error, 100, "Formula: number on positon %d is zero", start+1 );
		Show_error( window, error );
		return true;
	}
	return false;
}
		
//Funkcja przechodząc na odwrotną notację polską traci lokalne deklaracje np. Vx(Ey xRy) => ~(Ez zRx) jest błędnie interpretowana...
//Formulę należy przełożyć na NNF
static void Analyze( GtkWidget *window, char* formula, relations *rels )
{
	lnode *variables, *stack;
	variables = NULL;
	stack = NULL;
	int Rev_not[100];
	char result[100], name;
	int pos = 0, temp = 0;

	for( int i = 0 ; formula[i] != '\0' ; i++ )
		if( formula[i] == 'V' || formula[i] == 'E' )
			if( AddElem( &variables, formula[i+1], formula[i] == 'V' ? 1 : 0 ) ){
				FreeMemory( variables );
				Show_error( window, "Problem occured during memory reservation" );
				return;
			}

	//Rewriting formula to reverse notation
	for( int i = 0 ; formula[i] != '\0' ; i++ ){
		if( formula[i] == 'E' || formula[i] == 'V' ) i++;
		else if( IsDigitOrNum( formula[i] ) ){
			if( IsDigitOrNum( formula[i] ) == 1 )	Rev_not[pos++] = formula[i++];
			else if( IsDigitOrNum( formula[i] ) == 3 ){ 
				temp = 0;
				while( IsDigitOrNum( formula[i] ) == 3 ){
					temp *= 10;
					temp += formula[i] - '0';
					i++;
				}
				Rev_not[pos++] = temp + 1000;
			}
			name = formula[i++];
			if( IsDigitOrNum( formula[i] ) == 1 ) Rev_not[pos++] = formula[i];
			else{
				temp = 0;
				while( IsDigitOrNum( formula[i] ) == 3 ){
					temp *= 10;
					temp += formula[i] - '0';
					i++;
				}
				Rev_not[pos++] = temp + 1000;
				i--;
			}
			Rev_not[pos++] = name;
		}
		else if( formula[i] == '(' ){
			if( PushElem( &stack, '(', 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
		}
		else if( formula[i] == '<' ){
			while( stack != NULL && stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			if( PushElem( &stack, 1, 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
			i+=2;
		}
		else if( formula[i] == '=' ){
			while( stack != NULL && stack->a >= 2 && stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			if( PushElem( &stack, 2, 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
			i++;
		}
		else if( formula[i] == 'V' ){
			while( stack != NULL && stack->a >= 3 && stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			if( PushElem( &stack, 3, 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
		}
		else if( formula[i] == '&' ){
			while( stack != NULL && stack->a >= 4 && stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			if( PushElem( &stack, 4, 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
		}
		else if( formula[i] == '~' ){
			while( stack != NULL && stack->a >= 5 && stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			if( PushElem( &stack, 5, 0 ) ){
				Show_error( window, "Problem occured with memory reservation" );
				FreeMemory( variables );
				FreeMemory( stack );
				return;
			}
		}
		else if( formula[i] == ')' ){
			while( stack->a != '(' ){
				Rev_not[pos++] = stack->a;
				RemoveFirst( &stack );
			}
			RemoveFirst( &stack );
		}
		else i++;
	}
	//Finishing first notation (emptying the stack)
	while( stack != NULL ){
		Rev_not[pos++] = stack->a;
		RemoveFirst( &stack );
	}
	Rev_not[pos] = 0;
		
	//Checking if formula is true or false
	return;
}				
			
static int IsDigitOrNum( char c )
{
	if( c >= 'a' && c <= 'z' ){
		if( c == 'v' ) return 0;
		else return 1;
	}
	if( c >= 'A' && c <= 'Z' ){
		if( c == 'E' || c == 'V' ) return 0;
		else return 2;
	}
	if( c >= '0' && c <= '9' ) return 3;
	return 0;
}
