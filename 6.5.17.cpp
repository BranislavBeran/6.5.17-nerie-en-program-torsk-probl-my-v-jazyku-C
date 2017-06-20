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

CTWL *ctwl_create_empty(void){
	CTWL *l;	
	
	l=(CTWL*)malloc(sizeof(CTWL));
	l->cur=NULL;
	
	return l;
}

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
	
	if(list->cur==NULL){
		list->cur=ins;
		list->cur->next=ins;
		list->cur->prev=ins;
	}
	
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
	
	if(list->cur->next==NULL&&list->cur->prev==NULL){
		list->cur=ins;
		list->cur->next=ins;
		list->cur->prev=ins;
	}
	
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
	char CTWL_OK=1,CTWL_FAIL=0;
	
	if(list->cur==NULL){
	return CTWL_FAIL;
	}
	
	else if(list->cur==list->cur->next){
		list->cur=NULL;
		return CTWL_OK;
	}
	else{	
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
}

CTWL *ctwl_create_random(unsigned int size){
	int i;
	CTWL *list;
	TWN *begin,*tempprev,*tempnext,*tempcur;
	
	if(size==0){
		list=ctwl_create_empty();
		return list;
	}
	
	list=ctwl_create_empty();
	list->cur=(TWN*)malloc(sizeof(TWN));
	list->cur->data=rand()/(float)(RAND_MAX)* rand_;
	begin=list->cur;


	for(i=0;i<size-1;i++){
		list->cur->next=(TWN*)malloc(sizeof(TWN));
		
		ctwl_insert_right(list,rand()/(float)(RAND_MAX)*rand_);		
	}	
	
	tempprev=list->cur;
	list->cur->next=begin;
	ctwl_cur_step_right(list);
	list->cur->prev=tempprev;
		
	return list;
}

void ctwl_destroy(CTWL* list){
	TWN *curent;
	
	if(list->cur==list->cur->next){
		free(list->cur);
	}
	else{
		curent = list->cur;
		
		while(curent!=list->cur){
			free(list->cur->prev);
			ctwl_cur_step_right(list);
		}
	
		free(list->cur->prev);
		free(list->cur);
		free(list);
	}
}

CTWL *ctwl_create_random_unimodal(unsigned int size){
	int i,rand_min,rand_max,j=0;
	float rand_range_min,rand_range_max;
	CTWL *list;
	TWN *begin,*tempprev,*tempnext,*tempcur;
	
	if(size==0){
		list=ctwl_create_empty();
		return list;
	}
	else if(size==1){
		list=ctwl_create_empty();
		list->cur=(TWN*)malloc(sizeof(TWN));	
		list->cur->data=rand()/(float)(RAND_MAX)*10;
		list->cur->next=list->cur;
		list->cur->prev=list->cur;
	}
	else if(size==2){
		list=ctwl_create_empty();
		list->cur=(TWN*)malloc(sizeof(TWN));
		list->cur->data=rand()/(float)(RAND_MAX)*10;
		tempcur=list->cur;
		list->cur->next=(TWN*)malloc(sizeof(TWN));
		
		ctwl_cur_step_right(list);
		list->cur->prev=tempcur;
		list->cur->next=tempcur;
		list->cur->data=rand()/(float)(RAND_MAX)*10;
	}
	else{
		rand_min=rand()%(size-2)+2;
	
		
		if(rand_min!=2){
			rand_range_min=50/(float)(rand_min-2);
		}
	
		rand_range_max=50/(float)(size-rand_min);
	
		list=ctwl_create_empty();
		list->cur=(TWN*)malloc(sizeof(TWN));
		list->cur->data=rand()/(float)(RAND_MAX)*10+50;
		begin=list->cur;

		for(i=0;i<size-1;i++){
			list->cur->next=(TWN*)malloc(sizeof(TWN));
		
			if(i+2<rand_min){
				ctwl_insert_right(list,rand()/(float)(RAND_MAX)*rand_range_min+(50-(i+1)*rand_range_min));
			}
		
			else if(i+2==rand_min){
				ctwl_insert_right(list,rand()/(float)(RAND_MAX)*10-10);
			}
			
			else if(i+2>rand_min){
				ctwl_insert_right(list,rand()/(float)(RAND_MAX)*rand_range_max+j*rand_range_max);
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

main(){
	unsigned int x;
	CTWL l;
	CTWL *zoznam;
	char ctwl;
	
	srand(time(NULL));
	printf("zadaj dlzku zoznamu: ");
	scanf("%d",&x);

	zoznam=ctwl_create_random_unimodal(x);
	ctwl_print(zoznam);
}
