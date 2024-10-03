#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

struct pair{
	int *arr;
	int size;
};

//searches the element in the bst arrays
bool esearch(struct pair *a,int n,int e){
	int j;
	for(int i = 0; i < n; i++){
		j = 0;
		while (j < a[i].size){
			if ((a[i].arr)[j] == e)
				return true;
			else if((a[i].arr)[j] > e)
				j = (2*j)+1;
			else
				j = (2*j)+2;
		}
	}
	return false;
}

//sorting							 
int compare(const void *a,const void *b){
	return (*(int*)a - *(int*)b);
}

//prints the bst arrays
void pa(int* a,int n){
	for(int i=0;i<n;i++){
		printf("%d\n",a[i]);
	}
	printf("\n");
}

//counts the size the bstarray will take with null values
int bstcount(int size){
	int newsize = 1;
	int i = 0;
	while(size > newsize){
                i++;
                newsize = newsize + pow(2,i);
        }
	return newsize;
}

//initiating array to zeroes
void initiate(int *array,int size){
	for (int i = 0; i < size; i++)
		array[i] = 0;
}

//converts the list to bst	
void listtobst(int *list, int start, int end, int *array, int i){
	if(start > end)
		return;
	int mid = (start + end) / 2;
	array[i] = list[mid];
	listtobst(list, start, mid - 1, array, (2*i+1));
	listtobst(list, mid + 1, end, array, (2*i+2));
}

//remove the null(0) values from last level 
int* balance(int *array,int size,int bstsize){
	int *balancedbst = (int*)malloc(bstsize*sizeof(int));
	int j = 0;
	for (int i = 0; i <  size; i++){
		if (array[i] == 0)
			continue;
		balancedbst[j] = array[i];
		j++;
	}
	return balancedbst;
}

//creates array of bstarray pointers and size
struct pair* exception(int *bst, int bstsize, struct pair *bstarrays, int *arrpoint){
	int i = 0;
	int currsize = 1;
	int maxsize;
	while(bstsize >= currsize){//getting the size of bst with last completely filled level
		i++;
		maxsize = currsize;
		currsize = currsize + pow(2,i);
	} 
	if ( bstsize - maxsize != 0){
		int *array = (int*)malloc((bstsize - maxsize)*sizeof(int));//array of values in last incompletely filled level
		int j = 0;
		for (i = maxsize; i < bstsize; i++){
			array[j] = bst[i];
			j++;
		}
		int n_size = bstcount(bstsize-maxsize);
		int n_array[n_size];
		initiate(n_array,n_size);
		listtobst(array,0,bstsize-maxsize-1,n_array,0);
		int *newbst = balance(n_array,n_size,bstsize-maxsize);
		int *compbst = (int*)malloc(maxsize*sizeof(int));//the bstarray with complete last level
		for(i = 0; i < maxsize; i++)
			compbst[i] = bst[i];		
		bstarrays[*arrpoint-1]. arr =  compbst;
		bstarrays[*arrpoint-1]. size = maxsize;
		//free(bst);
		*arrpoint = *arrpoint + 1; 
		struct pair *newarray = (struct pair*) malloc(sizeof(struct pair)*(*arrpoint));
		for(int i=0; i<*arrpoint-1;i++){
			newarray[i].arr = bstarrays[i].arr;
			newarray[i].size = bstarrays[i].size;
			
		}
		free(bstarrays);
		bstarrays = exception(newbst , bstsize - maxsize, newarray, arrpoint);
		
	}
	else{
		bstarrays[*arrpoint - 1]. arr = bst;
		bstarrays[*arrpoint - 1]. size = bstsize; 
	}
	return bstarrays;
}
	
int main(){
	int *list = (int*)malloc(1048576*sizeof(int));
        FILE *fptr = fopen("data.txt","r");
	int i = 0;
        while(fread(&list[i],sizeof(int),1,fptr)){
                i++;
        }
        fclose(fptr);
	int listsize = 1048576;
	qsort(list, listsize, sizeof(int), compare);
	int size = bstcount(listsize);
	printf("maxsize = %d\n",size);
	int *array =(int*)malloc(sizeof(int)*size);
	initiate(array,size);
	listtobst(list, 0, listsize - 1, array, 0);
	int *bst = balance(array,size,listsize);
	struct pair *bstarrays= (struct pair*)malloc(sizeof(struct pair));
	int n = 1;
	bstarrays = exception(bst, listsize, bstarrays, &n);
//	for(int i=0;i<n;i++)
//		pa(bstarrays[i].arr,bstarrays[i].size);
	bool found = esearch(bstarrays,n,2147378065);
	printf("%d\n",found);
}
