#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define N     33554432		

int 	rand(void);
int   	rand_r();
int   	n, lst[N], buffer[N];

void  	srand();
struct 	timeval tv1, tv2;

void init_lst(int *l, int n) {
    srand(1234);

    for (int i = 0; i < n; i++) {
        l[i] = rand();
    }
}

void printLst(int *m){
	for(int i = 0; i < n; i++) {
		printf("%d\n", m[i]);
	}
}

void checkSorted(int *m){
	for(int i = 0; i < n-1; i++) {
		if (m[i] > m[i + 1]) {
			printf("Not sorted!\n");
			printf("%d is greater than %d", m[i], m[i+1]);
			exit(0);
		} 
	}
	printf("\nSorted! Woo!\n");
}

void merge(int left, int middle, int right){
	int leftSize  = middle - left + 1;
	int rightSize = right - middle;
	int Left[leftSize], Right[rightSize];

	for (int i = 0; i < leftSize; i++) {
		Left[i] = lst[left + i];
	} 

	for (int i = 0; i < rightSize; i++) {
		Right[i] = lst[middle + i + 1];
	}

	int i = 0;
	int j = 0;
	int k = left;

	while(i < leftSize && j < rightSize) {
		if (Left[i] <= Right[j]) {
			lst[k] = Left[i];
			i++;
		} else {
			lst[k] = Right[j];
			j++;
		}
		k++;
	}

	while (i < leftSize){
		lst[k] = Left[i];
		i++;
		k++;
	}

	while (j < rightSize){
		lst[k] = Right[j];
		j++;
		k++;
	}
}

void mergeSort(int left, int right) {
	if (left < right) {
		int middle = (left + right ) / 2;
		mergeSort(left, middle);
		mergeSort(middle + 1, right);
		merge(left, middle, right);
	}
}

void radix_sort(int group) {
	int mask 				= 0x0000000F;
	int arrayCopyDirection 	= 1;
	int digit;

	for (int j = 0; j < 8; j++) {
		int count[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int map[16]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		for (int i = 0; i < n; i++){
			if (arrayCopyDirection == 1){
				digit = (lst[i] & mask) >> (j * group);
			} else {
				digit = (buffer[i] & mask) >> (j * group);
			}
			count[digit]++;
		}

		for(int k = 1; k < 16; k++) {
			map[k] = count[k - 1] + map[k - 1];
		}

		for(int a = 0; a < n; a++) {
			if(arrayCopyDirection == 1) {
				digit = (lst[a] & mask) >> (j * group);
			} else {
				digit = (buffer[a] & mask) >> (j * group);
			}

			if(arrayCopyDirection == 1){
				buffer[map[digit]] = lst[a];
				map[digit] = map[digit] + 1;
			} else {
				lst[map[digit]] = buffer[a];
				map[digit] = map[digit] + 1;
			}
		}

		mask = mask << group;
		arrayCopyDirection *= -1;

		printf("Pass #%d complete.\n\n", j+1);
	}
}

void selection_sort(){
	int j = 0;

	while (j != n){
		int minNum = lst[j];
		int tmpNum;
		int tmpIdx = 0;

		for(int i = j; i < n; i++){
			if (lst[i] <= minNum){
				minNum = lst[i];
				tmpIdx = i;
			}
		}

		tmpNum = lst[j];
		lst[j] = minNum;
		lst[tmpIdx] = tmpNum;
		j++;
	}
}

int main() {
	n = 1000000;
	init_lst(lst, n);
	gettimeofday(&tv1, NULL);
	radix_sort(4);
	//selection_sort();
	//mergeSort(0, n-1);

	gettimeofday(&tv2, NULL);
	checkSorted(lst);
	//printLst(lst);

	printf ("Sorted %d elements in %f seconds\n\n", n,
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

	return 0;
}