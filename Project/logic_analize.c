#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define max_len 100

//Structre for passing args to SentenceEntered
typedef struct{
		GtkWidget *text;
		GtkWidget *set_entry;
	} text_and_set;


//Structure containing relations

	//List containing relation elements
typedef struct list_node{
		int a,b;
		struct list_node *next;
	} lnode;

	//Head of list containing name of relation and pointer to elements
typedef struct list_head{
		char name;
		lnode* elem;
	} lhead;

	//Structure containing set power and all relations
typedef struct{
		unsigned int power;
		unsigned int num_of_rels;
		lhead *rel;
	} relations; 


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

//Adds new element to the relations elements list
static int AddRelationElem( lnode *Rel, int a, int b );

//Frees the memory of relation list
static void FreeRelationMemory( lnode *Rel );
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

	printf( "%d\n", rels->power );


	for( int i = 0 ; i < rels->num_of_rels ; i ++ ){
		printf( "%c\n", rels->rel[i].name );
		FreeRelationMemory( rels->rel[i].elem );
	}
	free( rels->rel );
	free( rels );
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
					for( int j = 0 ; j < num_of_add_rels ; j++ )
						FreeRelationMemory( rels->rel[j].elem );	
					free(set);
					free(rels->rel);
					free(rels);
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
					}
					pos++;
					while( set[pos] >= '0' && set[pos] <= '9' ){
						b *= 10;
						b += set[pos] - '0';
						pos++;
					}
					//Checking if numbers arent bigger than set power
					if( a > rels->power || b > rels->power || a == 0 || b == 0 ){
						if( a == 0 || b == 0 )
							snprintf( error, 100, "Line %d: Element cant be a 0", CountNewlines( set, pos ) );	
						else
							snprintf( error, 100, "Line %d: Element is bigger than a set power"
								, CountNewlines( set, pos ) );
						Show_error( gtk_widget_get_toplevel( set_entry ), 
							error);
						for( int j = 0 ; j < num_of_add_rels ; j++ )
							FreeRelationMemory( rels->rel[j].elem );
						free(set);
						free(rels->rel);
						free(rels);
						return NULL;	
					}	
					if( AddRelationElem( rels->rel[num_of_add_rels].elem, a, b )){
						Show_error( gtk_widget_get_toplevel( set_entry ), 
							"Error occured during memory reservation" );
						for( int j = 0 ; j < num_of_add_rels ; j++ )
							FreeRelationMemory( rels->rel[j].elem );
						free(set);
						free(rels->rel);
						free(rels);
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

static int AddRelationElem( lnode *Rel, int a, int b )
{
	while( Rel != NULL )	Rel = Rel->next;
	Rel = malloc( sizeof( lnode ) );
	if( Rel == NULL ) return 1;
	Rel->a = a;
	Rel->b = b;
	return 0;
}

static void FreeRelationMemory( lnode *Rel )
{
	lnode *temp = Rel;
	while( Rel != NULL ){
		while( temp->next != NULL ) temp = temp->next;
		free(temp);
		temp = Rel;
	}
	return;
}
