#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#define NR_TESTS 5
#define MAX_SIZE 5000
#define STEP_SIZE 100

Profiler p("medie");
void printFunction(int v[], int n)
{
    for(int i=0;i<n;i++)
        printf("%d ",v[i]);
    printf("\n");
}
void swap(int *a, int *b)
{
    int aux=*a;
    *a=*b;
    *b=aux;
}
void heapify(int v[], int n, int i, int *contorCompHeap, int *contorAtrHeap, int size)
{
    int largest=i;
    int left=2*i+1, right=2*i+2;

    if(left<n && v[left] > v[largest])
        largest=left;

    if(right<n && v[right] > v[largest])
        largest=right;
    (*contorCompHeap)+=2;
    if(largest!=i) {
        swap(&v[i], &v[largest]);
        (*contorAtrHeap)+=3;
        heapify(v,n,largest,contorCompHeap,contorAtrHeap,size);
    }
}
void buildHeap_bottomUp(int v[], int n, int *contorCompHeap, int *contorAtrHeap, int size)
{
    int index=(n/2)-1;

    for(int i=index;i>=0;i--)
        heapify(v,n,i,contorCompHeap,contorAtrHeap,size);
}
void heapSort(int v[], int n, int size)
{
    Operation opCompHeapSort=p.createOperation("HeapSort comp",size);
    Operation opAtrHeapSort=p.createOperation("HeapSort atr",size);

    int contorCompHeap=0, contorAtrHeap=0;

    buildHeap_bottomUp(v,n,&contorCompHeap, &contorAtrHeap, size);
    int aux=n-1;
    for(int i=aux;i>=1;i--)
    {
        swap(&v[i],&v[0]);
        contorAtrHeap+=3;
        n--;
        heapify(v,n,0,&contorCompHeap, &contorAtrHeap,size);
    }

    opCompHeapSort.count(contorCompHeap);
    opAtrHeapSort.count(contorAtrHeap);
}
int partition(int v[], int left, int right, int *contorCompQuick, int *contorAtrQuick)
{
    int pivot=v[right];
    int i=(left-1);

    for(int j=left;j<right;j++)
    {
        (*contorCompQuick)++;
        if(v[j]<pivot)
        {
            i++;
            swap(&v[i],&v[j]);
            (*contorAtrQuick)+=3;
        }

    }
    swap(&v[i+1],&v[right]);
    (*contorAtrQuick)+=3;
    return (i+1);
}
void quickSort(int v[], int left, int right, int size)
{
    Operation opCompQuick=p.createOperation("QuickSort comp",size);
    Operation opAtrQuick=p.createOperation("QuickSort atr",size);

    int contorAtrQuick=0, contorCompQuick=0;
    if(left<right)
    {
        int pivot=partition(v,left,right,&contorCompQuick, &contorAtrQuick);

        quickSort(v,pivot+1,right,size);
        quickSort(v,left,pivot-1,size);
    }
    opCompQuick.count(contorCompQuick);
    opAtrQuick.count(contorAtrQuick);
}
int randomizedPartition(int v[], int left, int right)
{
    int a=0;
    int i= rand()%(right-left+1)+left;
    swap(&v[i],&v[right]);

    return partition(v,left,right,&a,&a);

}
int randomizedSelect(int v[], int left, int right, int i)
{
    if(left==right)
        return v[left];
    int q=randomizedPartition(v,left,right);
    int pivot=q-left+1;
    if(i==pivot)
        return v[q];
    if(i<pivot)
        return randomizedSelect(v,left,q-1,i);
    return randomizedSelect(v,q+1,right,i-pivot);


}
void demo()
{
    int v[] = {5, 9, 2, 7, 8, 4, 6, 8, 15, 27, 58, 62, 11};
    int n = sizeof(v) / sizeof(v[0]);
    int k=4;


    heapSort(v,n,n);
    printFunction(v, n);

    int q[] = {5, 9, 2, 7, 8, 4, 6, 8, 15, 27, 58, 62, 11};
    quickSort(q,0,n-1,n);
    printFunction(q,n);



    int z[] = {5, 9, 2, 7, 8, 4, 6, 8, 15, 27, 58, 62, 11};
    srand(time(0));
    printf("The %d-th smallest number is %d",k,randomizedSelect(z,0,n-1,k));


}
void perf(int order)
{
    int v[MAX_SIZE],z[MAX_SIZE];
    int n;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE)
    {
        for(int i=0;i<NR_TESTS;i++)
        {
            FillRandomArray(v,n,10,50000,false,order);
            if(order==0)
            {
                for(int j=0;j<n;j++)
                    z[j]=v[j];
                heapSort(v,n,n);
                quickSort(z,0,n-1,n);
            }
            else
            {
                quickSort(v,0,n-1,n);
            }

        }
    }
    p.divideValues("HeapSort atr", NR_TESTS);
    p.divideValues("HeapSort comp", NR_TESTS);
    p.divideValues("QuickSort atr", NR_TESTS);
    p.divideValues("QuickSort comp", NR_TESTS);

    p.addSeries("HeapSort Total", "HeapSort comp", "HeapSort atr");
    p.addSeries("QuickSort Total", "QuickSort comp", "QuickSort atr");

    p.createGroup("Comp", "HeapSort comp", "QuickSort comp");
    p.createGroup("Atr", "HeapSort atr", "QuickSort atr");
    p.createGroup("Total", "HeapSort Total", "QuickSort Total");
}
int main() {

    demo();
    ///perf(0);

   /// p.reset("worst");
    ///perf(1);

    ///p.showReport();
    return 0;
}
