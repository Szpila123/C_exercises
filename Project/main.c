#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "gtk_window.h"
#include <string.h>

//Getting entred text
void SentenceEntered( GtkWidget *widget, void* data );


int main( int argc, char *argv[] )
{
	gtk_init( &argc, &argv );

//Creating main WINDOW	
	GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title( GTK_WINDOW( window ), "Logic analizer" );
	gtk_window_set_position( GTK_WINDOW( window ), GTK_WIN_POS_CENTER );
	g_signal_connect( G_OBJECT( window ), "destroy", G_CALLBACK( gtk_main_quit ), NULL );
	gtk_container_set_border_width( GTK_CONTAINER( window ), 10 );

//Chain functions
//Starting displaying
	Display_sentence_entry( window );
	gtk_main();
	
	return 0;
}

void SentenceEntered( GtkWidget* widget, void *data)
{
	window_and_text *wnd_and_txt = data;
	char stuff[100];
	strcpy( stuff, gtk_entry_get_text( GTK_ENTRY( wnd_and_txt->text ) ) );
	printf("%s\n", stuff );
	Display_set_entry( wnd_and_txt->window );
	return;
}
