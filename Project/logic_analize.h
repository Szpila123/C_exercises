#ifndef _logic_analize_h
#define _logic_analize_h

#define max_len 100
	

typedef struct{
		GtkWidget *text;
		GtkWidget *set_entry;
	} text_and_set;

void SentenceEntered( GtkWidget *widget, void* data );

#endif
