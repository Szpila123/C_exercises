//Kurs WdpC Lista_4 Zadanie2
#include <stdio.h>
#include <limits.h>

#define NUM_OF_ELE 10

int print_combinations( int n, int k, int A[] );

int main()
{
	int A[NUM_OF_ELE];
	int n = sizeof( A ) / sizeof( A[0] );
	int k;

	for( int i = 1 ; i <= n ; i++)	A[i-1] = i;
	scanf( "%d", &k );
	print_combinations( n, k, A );
	return 0;
}

int print_combinations( int n, int k, int A[] )
{
	unsigned int Mask = 0;
	for( int i = 0; i < k ; i++ )
		Mask |= 1 << i;

	int bit = 1;
	int sum = 0;
	while( (Mask  & 1 << n) == 0 )
	{
		for( int i = 0 ; i < n ; i++ )
			if( ( Mask & 1 << i ) != 0 ) printf( "%d ", A[i]);
		printf( "\n" );

		while( ( Mask & bit ) == 0 )
		        bit <<= 1;

		Mask += bit;	

		for( int i = 0 ; i < n ; i++ )	
			if( Mask & 1 << i ) sum ++;
		
		for( int i = 0 ; i < k - sum ; i++ )	
			Mask |= 1 << i;

		sum = 0;
		bit = 1;
	}
	return 0;
}
