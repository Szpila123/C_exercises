CC= gcc 
PACKAGE= `pkg-config --cflags gtk+-3.0`
LIBS= `pkg-config --libs gtk+-3.0`


prog: main.o 
	$(CC) main.o gtk_window.o logic_analize.o lists.o  -o prog $(LIBS)
main.o: gtk_window.o
	$(CC) $(PACKAGE) -c main.c -o main.o
gtk_window.o: gtk_window.c logic_analize.o gtk_window.h
	$(CC) $(PACKAGE) -c gtk_window.c -o gtk_window.o 
logic_analize.o: logic_analize.c lists.o logic_analize.h
	$(CC) $(PACKAGE) -c logic_analize.c -o logic_analize.o 
lists.o: lists.c lists.h
	$(CC) -c  lists.c -o lists.o



