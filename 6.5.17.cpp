#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#define rand_ 100


typedef struct TWN{
	float data;
	struct TWN *prev;
	struct TWN *next;
}TWN;

typedef struct{
	TWN *cur;
}CTWL;

void ctwl_cur_step_right(CTWL *list){
	list->cur=list->cur->next;
}

void ctwl_cur_step_left(CTWL *list){
	list->cur=list->cur->prev;
}

TWN *ctwl_insert_right(CTWL* list, float val){
	TWN *ins,*curent,*next;
	
	ins=(TWN*)malloc(sizeof(TWN));
	ins->data=val;
	curent=list->cur;
	next=list->cur->next;
	
	ins->prev=curent;
	ins->next=next;
	list->cur->next=ins;
	next->prev=ins;
	ctwl_cur_step_right(list);
	
	return ins;	
}

TWN *ctwl_insert_left(CTWL* list, float val){	
	TWN *ins,*curent,*prev;
	
	ins=(TWN*)malloc(sizeof(TWN));
	ins->data=val;
	curent=list->cur;
	prev=list->cur->prev;
	
	ins->next=curent;
	ins->prev=prev;
	prev->next=ins;
	curent->prev=ins;
	ctwl_cur_step_left(list);
	
	return ins;
}

char ctwl_delete(CTWL* list){
	TWN *curent,*next,*prev;
	char CTWL_OK,CTWL_FAIL;
	
	if(list->cur->next==NULL)	return CTWL_FAIL;
	
	curent=list->cur;
	
	next=list->cur->next;
	prev=list->cur->prev;
	
	ctwl_cur_step_left(list);
	list->cur->next=next;
	ctwl_cur_step_right(list);
	list->cur->prev=prev;
	
	free(curent);
	return CTWL_OK;

	
}

CTWL *ctwl_create_empty(void){
	CTWL *l;
	
	
	l=(CTWL*)malloc(sizeof(CTWL));
	
	l->cur=(TWN*)malloc(sizeof(TWN));
	l->cur->next=NULL;
	l->cur->prev=NULL;
	
	return l;
}

CTWL *ctwl_create_random(unsigned int size){
	int i;
	CTWL *list;
	TWN *begin,*tempprev,*tempnext,*tempcur;
	
	list=ctwl_create_empty();
	list->cur->data=rand()/(float)(RAND_MAX)* rand_;
	begin=list->cur;


	for(i=0;i<size-1;i++){
		list->cur->next=(TWN*)malloc(sizeof(TWN));
		if(list->cur->next==NULL) return NULL;
		

		tempprev=list->cur;
		ctwl_cur_step_right(list);
		list->cur->prev=tempprev;
		
		list->cur->data=rand()/(float)(RAND_MAX)*rand_;		
	}	
	
	tempprev=list->cur;
	list->cur->next=begin;
	ctwl_cur_step_right(list);
	list->cur->prev=tempprev;
		
	return list;
}

void ctwl_destroy(CTWL* list){
	TWN *curent,*prev;
	int i;
	
	curent=list->cur;
	ctwl_cur_step_right(list);
		
	while(curent!=list->cur){
		prev=list->cur;
		ctwl_cur_step_right(list);
		free(prev);
	}
	
	free(list->cur);
}

void ctwl_print(CTWL *list){
	TWN *cur;
	int i=2;
	
	if(list->cur==NULL){
		printf("empty");
	}
	else{
	
	cur=list->cur;
	printf("cur :%f \n",list->cur->data);

	ctwl_cur_step_right(list);
	
	while(list->cur!=cur){
		
		printf("%d %f\n",i,list->cur->data);
		ctwl_cur_step_right(list);
		i++;
	}
	}
}

CTWL *ctwl_create_random_unimodal(unsigned int size){
	int i,rand_min,rand_max,j=0;
	float rand_range_min,rand_range_max;
	CTWL *list;
	TWN *begin,*tempprev,*tempnext,*tempcur;
	
	if(size==2){
		list=ctwl_create_empty();
		list->cur->data=rand()/(float)(RAND_MAX)*10;
		tempcur=list->cur;
		list->cur->next=(TWN*)malloc(sizeof(TWN));
		
		ctwl_cur_step_right(list);
		list->cur->prev=tempcur;
		list->cur->next=tempcur;
		list->cur->data=rand()/(float)(RAND_MAX)*10;
	}
	else{
		rand_min=rand()%(size-1)+1;
		while(rand_min==1){
			rand_min=rand()%(size-1)+1;
		}
		
		if(rand_min!=2){
			rand_range_min=50/(float)(rand_min-2);
		}
	
		rand_range_max=50/(float)(size-rand_min);
	
		list=ctwl_create_empty();
		list->cur->data=rand()/(float)(RAND_MAX)*10+50;
		begin=list->cur;

		for(i=0;i<size-1;i++){
			list->cur->next=(TWN*)malloc(sizeof(TWN));
			if(list->cur->next==NULL) return NULL;	

			tempprev=list->cur;
			ctwl_cur_step_right(list);
			list->cur->prev=tempprev;	
		
			if(i+2<rand_min){
				list->cur->data=rand()/(float)(RAND_MAX)*rand_range_min+(50-(i+1)*rand_range_min);
			}
		
			else if(i+2==rand_min){
				list->cur->data=rand()/(float)(RAND_MAX)*10-10;
			}
			
			else if(i+2>rand_min){
				list->cur->data=rand()/(float)(RAND_MAX)*rand_range_max+j*rand_range_max;
				j++;
			}		
		}
		
		tempprev=list->cur;
		list->cur->next=begin;
		ctwl_cur_step_right(list);
		list->cur->prev=tempprev;
	}
	return list;
	
}

main(){
	unsigned int x;
	CTWL l;
	CTWL *zoznam;
	
	srand(time(NULL));
	printf("zadaj dlzku zoznamu: ");
	scanf("%d",&x);

	zoznam=ctwl_create_random_unimodal(x);
//	zoznam=ctwl_create_random(x);
	ctwl_print(zoznam);

	ctwl_insert_left(zoznam,500);
	ctwl_print(zoznam);


}
