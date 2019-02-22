#ifndef _lists_h
#define _lists_h


typedef struct list_node{
		int a,b;
		struct list_node *next;
	} lnode;

typedef struct list_head{
		char name;
		lnode* elem;
	} lhead;

typedef struct{
		unsigned int power;
		unsigned int num_of_rels;
		lhead *rel;
	} relations; 

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

int AddElem( lnode **Rel, int a, int b );

void FreeMemory( lnode *Rel );

void FreeRelationsSpace( relations *rels );

int CheckListElem( lnode *list, int a, int b );

void RemoveFirst( lnode **Rel );

int PushElem( lnode **Rel, int a, int b );
#endif
