#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "gtk_window.h"

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
