#include <stdlib.h>

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
		unsigned int num_of_rels;
		lhead *rel;
	} relations; 

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//Adds new element to the relations elements list
int AddElem( lnode **Rel, int a, int b );

//Frees the memory of relation list
void FreeMemory( lnode *Rel );

//Frees space of relations structure
void FreeRelationsSpace( relations *rels );

//Checks if list containes element
int CheckListElem( lnode *list, int a, int b ); 

//Removes first element and returns second
void RemoveFirst( lnode **Rel );

//Insert the element on the first place
int PushElem( lnode **Rel, int a, int b );

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int AddElem( lnode **Rel, int a, int b )
{
	while( *Rel != NULL ) Rel = &(*Rel)->next;
	*Rel = malloc( sizeof( lnode ) );
	if( *Rel == NULL ) return 1;
	(*Rel)->a = a;
	(*Rel)->b = b;
	(*Rel)->next = NULL;
	return 0;
}

void FreeMemory( lnode *Rel )
{
	lnode **temp;
	if( Rel != NULL ) temp = &(Rel->next);
	else return;

	while( Rel != NULL || *temp != NULL  ){
		free(Rel);
		Rel = *temp;
		if( Rel != NULL ) temp = &(Rel->next);
	}
	return;
}


void FreeRelationsSpace( relations *rels )
{
	for( unsigned int j = 0 ; j < rels->num_of_rels ; j++ )
		if( rels != NULL && rels->rel != NULL && rels->rel[j].elem != NULL  )	FreeMemory( rels->rel[j].elem );
	if( rels != NULL ) free(rels->rel);
		free(rels);
	return;
}

int CheckListElem( lnode *list, int a, int b )
{
	while( list != NULL ){
		if( list->a == a && list->b == b ) return 1;
		list = list->next;
	}
	return 0;
}

void RemoveFirst( lnode **Rel )
{
	if( *Rel == NULL ) return;
	lnode* temp = (*Rel)->next;
	free( Rel );
	*Rel = temp;
	return;
}

int PushElem( lnode **Rel, int a, int b )
{
	lnode *temp = malloc( sizeof( lnode ) );
	if( temp == NULL ) return 1;
	temp->a = a;
	temp->b = b;
	temp->next = *Rel;
	*Rel = temp;
	return 0;
}
