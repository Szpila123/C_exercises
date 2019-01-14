//Kurs WdpC Lista_5 Zadanie3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

//Trojkat (3 punkty) przesuwajacy sie o wektor shift
typedef struct STriangle
{
    struct Point { int x, y; } point[3];

    struct Vec { int x, y; } shift;

    struct Segment { struct Point *A, *B; } seg[3];

} STriangle;

//Wprowadzenie trojkata z stdin - 3 punkty, wektor przesuniecia
//Odcinką przypisywane sa punkty poczatku i konca
void EnterTriangle( STriangle* PTri );

//Sprawdza czy w aktualnym ustaweieniu trojkaty sie przecinaja
int TriangleTouch( STriangle Tri1, STriangle Tri2 );

//Sprawdza czy dwa odcinki sie przecinaja
int SegmentsTouch( struct Segment S1, struct Segment S2);

//Sprawdza czy trojkaty przecinaja sie podczas ruchu
int ShiftTouch( STriangle Tri1, STriangle Tri2);

//Sprwadza czy wierzcholek pierwszego trojkata lezy na boku drugiego
int TouchPoint( struct Segment S2, struct Point P1 );

//Sprawdza orientacje punktow
int CheckOrientation( struct Point P1, struct Point P2, struct Point P3);

//Sprawdza czy jeden punkt jest po tej samej stronie odcinka co inny
int TriangleInside( STriangle Tri1, STriangle Tri2);


int main()
{
    int n;
    scanf( "%d", &n );

    STriangle Tri[n];

    int Status[n];

    for( int i = 0 ; i < n ; i ++ )
            Status[i] = 1;

    for( int i = 0 ; i < n ; i++ ) EnterTriangle(  &Tri[i] );

    for( int i = 0 ; i < n ; i++ )
        for( int j = i + 1 ; j < n; j ++ )
        {
            if( ( Status[i] != 0 || Status[j] != 0 ) && TriangleTouch( Tri[i], Tri[j] ) )
            {
                Status[i] = 0;
                Status[j] = 0;
            }
            else if( ( Status[i] == 1 || Status[j] == 1 ) && ShiftTouch( Tri[i], Tri[j] ) )
            {
                if( Status[i] == 1 ) Status[i] = 3;
                if( Status[j] == 1 ) Status[j] = 3;
            }
        }

    for( int i = 0 ; i < n ; i ++ )
        printf( "%d ", Status[i] );

    return 0;
}


void EnterTriangle( STriangle *PTri )
{
    for( int p = 0 ; p < 3 ; p ++ )
        scanf( "%d%d", & PTri -> point[p].x, & PTri -> point[p].y  );

    scanf( "%d%d", & PTri -> shift.x, & PTri -> shift.y );

    //Kopiowanie do tablicy odcinkow trojkata punktow poczatku i konca kazdego odcinka
    memcpy( PTri -> seg,(struct Segment[])
    {
        { &PTri -> point[0], &PTri -> point[1]},
        { &PTri -> point[1], &PTri -> point[2]},
        { &PTri -> point[2], &PTri -> point[0]}
    }, 3 * sizeof( struct Segment ) );
}

int TriangleTouch( STriangle Tri1, STriangle Tri2 )
{
    for( int i = 0 ; i < 3 ; i ++ )
        for( int j = 0 ; j < 3 ; j ++ )
            if( SegmentsTouch( Tri1.seg[i], Tri2.seg[j] ) ) return 1;

    if( TriangleInside( Tri1, Tri2 ) ||  TriangleInside( Tri2, Tri1 )) return 1;

    return 0;
}

int ShiftTouch( STriangle Tri1, STriangle Tri2 )
{
    struct Vec relative1, relative2;
    struct Point EndOfMov1, EndOfMov2;

    relative1.x = Tri1.shift.x - Tri2.shift.x;
    relative1.y = Tri1.shift.y - Tri2.shift.y;
    relative2.x = Tri2.shift.x - Tri1.shift.x;
    relative2.y = Tri2.shift.y - Tri1.shift.y;


        for( int i = 0 ; i < 3 ; i ++ )
        {
            EndOfMov1 = (struct Point ) { Tri1.point[i].x + relative1.x, Tri1.point[i].y + relative1.y };
            EndOfMov2 = (struct Point ) { Tri2.point[i].x + relative2.x, Tri2.point[i].y + relative2.y };
            for( int j = 0 ; j < 3 ; j ++ )
            {
                if( SegmentsTouch( (struct Segment) { &Tri1.point[i], & EndOfMov1 }, Tri2.seg[j] )) return 1;
                if( SegmentsTouch( (struct Segment) { &Tri2.point[i], & EndOfMov2 }, Tri1.seg[j] )) return 1;
            }
        }
        return 0;
}

int SegmentsTouch( struct Segment S1, struct Segment S2 )
{
    int o1 = CheckOrientation( *S1.A, *S1.B, *S2.A );
    int o2 = CheckOrientation( *S1.A, *S1.B, *S2.B );
    int o3 = CheckOrientation( *S2.A, *S2.B, *S1.A );
    int o4 = CheckOrientation( *S2.A, *S2.B, *S1.B );

    if( o1 != o2 && o3 != o4 ) return 1;

    if( o1 == 0 && TouchPoint( S1, *S2.A ) ) return 1;
    if( o2 == 0 && TouchPoint( S1, *S2.B ) ) return 1;
    if( o3 == 0 && TouchPoint( S2, *S1.A ) ) return 1;
    if( o4 == 0 && TouchPoint( S2, *S1.B ) ) return 1;
    return 0;
}

int TouchPoint( struct Segment S2, struct Point P1 )
{
    if( P1.x >= min(S2.A->x, S2.B->x) && P1.x <= max(S2.A->x, S2.B->x) )
        if( P1.y >= min(S2.A->y, S2.B->y) && P1.y <= max(S2.A->y, S2.B->y) ) return 1;
    return 0;
}

int CheckOrientation( struct Point P1, struct Point P2, struct Point P3 )
{
    int orientation = ( P2.y - P1.y) * ( P3.x - P2.x ) - ( P2.x - P1.x ) * (P3.y - P2.y);
    if( orientation == 0 ) return 0;
    return orientation > 0 ? 1 : 2;
}

int TriangleInside( STriangle Tri1, STriangle Tri2)
{
    int o1,o2,o3,o4;
    struct Segment TempSeg;

    for( int i = 0 ; i < 3 ; i ++ )
    {
        TempSeg = Tri1.seg[i];
        o1 = CheckOrientation( *TempSeg.A, *TempSeg.B, Tri1.point[(i + 2) % 3] );
        o2 = CheckOrientation( *TempSeg.A, *TempSeg.B, Tri2.point[0] );
        o3 = CheckOrientation( *TempSeg.A, *TempSeg.B, Tri2.point[1] );
        o4 = CheckOrientation( *TempSeg.A, *TempSeg.B, Tri2.point[2] );

        if( o2 != o1 || o3 != o1 || o4 != o1) return 0;
    }
    return 1;
}
