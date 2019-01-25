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

	free( rels );
	return;
}
	

			
static relations* RelationsInfo( GtkWidget *set_entry )
{
	
	GtkTextIter start, end;
	relations *rels = malloc( sizeof( relations ) );
	*rels = (relations) { 0, NULL };
	int pos = 0;
	char c;
	
//Getting string from text_view frame 
	GtkTextBuffer *buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW( set_entry ) );
	gtk_text_buffer_get_bounds( buffer, &start, &end );
	char *set = gtk_text_buffer_get_text( buffer, &start, &end, FALSE );

//Getting set power
	for( c = set[0] ;  c != '\0' && c != '\n' ; pos ++, c = set[pos] )
		if( c > '9' || c < '0' )
		{
			free(set);
			free(rels);
			Show_error( gtk_widget_get_toplevel( set_entry ),
				"Set power should be a number" );
			return NULL;
		}
		else{
			rels->power *= 10; 
			rels->power += c - '0';
		}
	
	if( rels->power == 0 )
	{
		free(set);
		free(rels);
		Show_error( gtk_widget_get_toplevel( set_entry ),
			"Set power should be bigger than 0" );
		return NULL;
	}


	
	free(set);
	return rels;
}
