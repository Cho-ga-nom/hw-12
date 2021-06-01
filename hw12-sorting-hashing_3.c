/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);
int freeArray(int *a);
void printArray(int *a);

int selectionSort(int *a);
int insertionSort(int *a);
int bubbleSort(int *a);
int shellSort(int *a);
/* recursive function으로 구현 */
int quickSort(int *a, int n);


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL)
		free(a);
	return 0;
}

void printArray(int *a)                         // 배열 출력함수
{
	if (a == NULL) {                               // 배열이 NULL이면 출력하지 않고 돌아감
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a)                     // 선택정렬
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (i = 0; i < MAX_ARRAY_SIZE; i++)               // 배열의 첫번째 요소부터 끝까지 반복
	{
		minindex = i;                            // 최소값의 인덱스 번호를 현재 인덱스로 초기화
		min = a[i];                              // 다음 인덱스의 값과 비교를 위해 현재 값으로 변수 초기화
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)       // 중첩 반복문을 통해, i의 다음 인덱스부터 반복
		{
			if (min > a[j])                      // 위에서 초기화한 min이 자신의 다음 값보다 크다면 
			{
				min = a[j];                    // 최소값과 최소 인덱스를 바꿈
				minindex = j;
			}
		}
		a[minindex] = a[i];              // 찾은 최소값을 해당 반복에서 배열의 첫번째 위치로 옮김.
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}

int insertionSort(int *a)         // 삽입 정렬
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)              // 배열의 두번째 요소부터 반복
	{
		t = a[i];                              // t에는 배열의 값 저장
		j = i;                                 // j에는 인덱스 번호 저장
		while (a[j-1] > t && j > 0)           // 현재 인덱스-1의 값이 현재 값보다 크고, j가 0이 아니라면
		{
			a[j] = a[j-1];                  // 앞에 있던 큰 값을 교환하여 뒤로 보낸다.
			j--;                           // j를 1씩 내리면서 배열의 첫번째 요소까지 접근하여 정렬을 수행한다.
		}
		a[j] = t;                // i의 값은 1씩 올라가므로 한칸씩 뒤로가며 그 앞에 있는 요소들을 순차적으로 정렬해나간다.
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int bubbleSort(int *a)                // 버블정렬
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++)               // 이중반복
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)           // 최대값을 배열의 맨 뒤로 보내는 작업을 반복한다.
		{
			if (a[j-1] > a[j])                     // 앞에 큰 값이 있다면
			{
				t = a[j-1];                          // 두 값을 교환
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int shellSort(int *a)                // 셸 정렬
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)               // 배열의 부분집합의 기준이 되는 h를 배열 길이의 절반으로 설정.
	{                                                       // h가 1이 될때까지 h의 값을 반씩 감소시키며 반복
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)       // i에서 h만큼 떨어진 곳에서 시작. 간격 h만큼 j가 증가
			{
				v = a[j];                                 
				k = j;
				while (k > h-1 && a[k-h] > v)            // a[j]와 a[j]에서 h만큼 떨어져 있는 값을 비교하여 h만큼 앞에 있는 값이 크다면
				{
					a[k] = a[k-h];                       // 큰 값을 뒤로 가져옴
					k -= h;                             // 인덱스 k를 h만큼 앞으로 이동하여
				}
				a[k] = v;                              // 원래 a[j]에 있던 값을 앞으로 가져옴. 
			}                                         // 결과적으로 h만큼 떨어져 있던 두 요소가 교환됨.
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int quickSort(int *a, int n)          // 퀵 정렬
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];             // 피벗 설정
		i = -1;                 // left
		j = n - 1;              // right

		while(1)
		{
			while(a[++i] < v);    // left에서부터 피벗보다 큰 값을 만날때까지 증가
			while(a[--j] > v);    // right에서부터 피벗보다 작은 값을 만날때까지 감소

			if (i >= j) break;     // left와 right가 교차하면 루프 탈출
			t = a[i];              
			a[i] = a[j];          // left와 right의 값을 교환
			a[j] = t;
		}
		t = a[i];              
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);            // 피벗의 왼쪽에서 반복하여 정렬
		quickSort(a+i+1, n-i-1);    // 피벗의 오른쪽에서 반복하여 정렬
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;               // k를 해시 테이블의 최대 크기로 나눈 나머지를 k의 홈 버킷으로 사용
}                                                   // 

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];
		hashcode = hashCode(key);
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		} else 	{

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);

	if(ht[index] == key)
		return index;

	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}