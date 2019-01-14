//Kurs WdpC Lista_2 Zadanie3
#include <stdio.h>
#include <stdlib.h>

void Code( int x, int y );
void Decode( int x, int y );


int main()
{
    char Character,C;
    int x,y;

    scanf( "%c %d %d", &Character, &x, &y );

    while( ( C = getchar() )  != '\n' && C != EOF );
    if( Character == 'K' )  Code( x, y );
    else Decode( x, y );

    return 0;
}


void Code( int x, int y )
{
    char Text[x][y];
    int End = 0;
    do
    {
        for( int i = 0 ; i < y ; i ++ )
            for( int j = 0 ; j < x ; j++ )
		if( !End )
		{
			Text[j][i] = getchar();
			if( Text[j][i] == EOF ) End = 1;
		}
		else	Text[j][i] = EOF;

        for( int j = 0 ; j < x ; j++ )
            for( int i = 0 ; i < y ; i ++ )
                if( Text[j][i] != EOF )
		{
			putchar( Text[j][i] );
			Text[j][i] = EOF;
		}
    }
    while( !End );
    return;
}

void Decode( int x, int y)
{
    char Text[x][y];
    int End = 0;

    while(!End)
    {
        for( int j = 0 ; j < x ; j ++ )
            for( int i = 0 ; i < y ; i++ )
		if( !End )
		{
	    		Text[j][i] = getchar();
			if( Text[j][i] == EOF ) End = 1;
		}
		else	Text[j][i] = EOF;
	//This if statment is loading the end of the text in right order
	//Letters are shifted because the last "frame" isnt fully filled
	if( End )	
	{
		char GoodElements[ x * y ];
		int ok = 0;
		for( int j = 0 ; j < x ; j++ )
        		for( int i = 0 ; i < y ; i ++ )
				if( Text[j][i] != EOF )
				{
					GoodElements[ok] = Text[j][i];
					ok++;
				}
		int Element = 0;
		for( int i = 0 ; i < y ; i++ )
			for( int j = 0 ; j < x ; j++ )	
				if( Element < ok )
				{
					Text[j][i] = 1; 
					Element++;
				}
				else 
					Text[j][i] = EOF;
		Element = 0;
		for( int j = 0 ; j < x ; j ++ )
			for( int i = 0 ; i < y ; i ++ )
				if( Text[j][i] == 1 )
				{
					Text[j][i] = GoodElements[ Element ];
					Element ++;
				}


    	}

        for( int i = 0 ; i < y ; i++ )
       		for( int j = 0 ; j < x ; j ++ )
              		if( Text[j][i] != EOF )
			{
				putchar( Text[j][i] );
				Text[j][i] = EOF;
    			}	
    }

    return;
}

