#include <gtk/gtk.h>
#include <string.h>
#include "logic_analize.h"

#define max_len 100

//Function from module logic_analize
extern void SentenceEntered( GtkWidget *widget, GtkWidget *text );

//Shows errors in text enteries or input/output streams
void Show_error( GtkWidget *window, char *info );

//Displays sentance entry grid in window
void Display_sentence_entry( GtkWidget *window );

//Displays set entry grid in window
void Display_set_entry( GtkWidget *window );

//Adding text to input area
static void Add_to_text( GtkWidget *widget, void *txt_and_in ); 

/////Structures for passing arguments to functions:
//Pointer to entry window and string 
typedef struct{
		GtkWidget *text;
		char *input;
	} text_and_input;

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void Show_error(GtkWidget *window, char *info )
{
	GtkWidget *dialog = gtk_message_dialog_new( GTK_WINDOW(window), 
		GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", info );	
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}	



void Display_sentence_entry( GtkWidget *window )
{
	GtkWidget *grid=gtk_grid_new();

//Creating ROWS in grid
	gtk_grid_set_row_spacing( GTK_GRID( grid ), 10 );
	gtk_grid_set_row_homogeneous( GTK_GRID( grid ), TRUE );	

//Creating COLUMNS in grid
	gtk_grid_set_column_spacing( GTK_GRID( grid ), 7 );
	gtk_grid_set_column_homogeneous( GTK_GRID( grid ), TRUE );

	gtk_container_add(GTK_CONTAINER( window ), grid);

//Creating TEXT ENTRY pole in grid
	GtkWidget *text = gtk_entry_new();
	gtk_entry_set_max_length( GTK_ENTRY( text ), max_len );
	gtk_entry_set_alignment( GTK_ENTRY( text ), 0 );
	gtk_entry_set_placeholder_text( GTK_ENTRY( text ), "Enter formula here" );
	gtk_entry_set_text( GTK_ENTRY( text ), "" );
	gtk_grid_attach( GTK_GRID( grid ), text, 0, 4, 7, 1 );

//Creating SET ENTRY pole in grid
	GtkWidget *set_entry = gtk_text_view_new();
	gtk_text_view_set_input_purpose( GTK_TEXT_VIEW( set_entry ), GTK_INPUT_PURPOSE_ALPHA );
	gtk_grid_attach( GTK_GRID( grid ), set_entry, 0, 0, 7, 3 );

//Creating BUTTONS for special signs
	struct spec_bttns{
		char *sign, *exit;
		int row, column, width, height;
	} bttns[] = {{ "~","~", 3, 0, 1, 1 }, { "v","v", 3, 1, 1, 1 }, { "AND", "AND", 3, 2, 1, 1 },
		 { "=>", "=>", 3, 3, 1, 1 }, { "<=>", "<=>", 3, 4, 1, 1 },
		 { "E", "E", 3, 5, 1, 1 }, { "V", "V", 3, 6, 1, 1 }, { "DONE", "", 5, 2, 3, 1 }
		 };

//The array of structures to pass argumnets to g_signal_connect for buttons
	text_and_input *txt_and_in = calloc(sizeof( bttns )/sizeof( bttns[0] ),
		sizeof(text_and_input)) ;

//Adding each button to window
	for( int i = 0 ; i < sizeof( bttns )/ sizeof( bttns[0] ) - 1 ; i++ )
	{
		txt_and_in[i].text = text;
		txt_and_in[i].input = bttns[i].exit;
		GtkWidget *button = gtk_button_new_with_label( bttns[i].sign );
		g_signal_connect( G_OBJECT( button ), "clicked",
			G_CALLBACK( Add_to_text ), &txt_and_in[i] );
		gtk_grid_attach( GTK_GRID( grid ), button,
			bttns[i].column, bttns[i].row, bttns[i].width, bttns[i].height );
	}

	int lst_bttn  = sizeof( bttns) / sizeof( bttns[0] ) - 1;
	GtkWidget *button_done = gtk_button_new_with_label( bttns[lst_bttn].sign );
	g_signal_connect(  button_done, "clicked", G_CALLBACK( SentenceEntered ), text );
	gtk_grid_attach( GTK_GRID( grid ), button_done, bttns[lst_bttn].column, bttns[lst_bttn].row, 
		bttns[lst_bttn].width, bttns[lst_bttn].height );
	
	gtk_widget_show_all( window );
	return ;
}



static void Add_to_text( GtkWidget *widget, void *txt_and_in )
{
	text_and_input *data = txt_and_in;

	//Getting positions of text in entry and adding button input
	int pos = gtk_entry_get_text_length( GTK_ENTRY( data->text ) );
	gtk_editable_insert_text(GTK_EDITABLE( data->text ), data->input, -1, &pos );
	return;
}
