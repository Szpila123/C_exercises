#ifndef _gtk_window_h
#define _gtk_window_h
 
#define max_len 100

typedef struct{ 
		GtkWidget *window;	
		GtkWidget *text;
	} window_and_text; 

void Show_error( GtkWidget window, char* info );
void Display_sentence_entry( GtkWidget *window );
void Display_set_entry( GtkWidget *window );

#endif
