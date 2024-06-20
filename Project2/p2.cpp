#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

int Partition(string words[], int i, int k) {
   int l;
   int h;
   int midpoint;
   string pivot;
   string temp;
   bool done;
   
   /* Pick middle element as pivot */
   midpoint = i + (k - i) / 2;
   pivot = words[midpoint];
   
   done = false;
   l = i;
   h = k;
   
   while (!done) {
      
      /* Increment l while numbers[l] < pivot */
      while (words[l] < pivot) {
         ++l;
      }
      
      /* Decrement h while pivot < numbers[h] */
      while (pivot < words[h]) {
         --h;
      }
      
      /* If there are zero or one elements remaining,
       all numbers are partitioned. Return h */
      if (l >= h) {
         done = true;
      }
      else {
         /* Swap numbers[l] and numbers[h],
          update l and h */
         temp = words[l];
         words[l] = words[h];
         words[h] = temp;
         
         ++l;
         --h;
      }
   }
   
   return h;
}
void Quicksort(string words[], int i, int k) {
   int j;
   
   /* Base case: If there are 1 or zero elements to sort,
    partition is already sorted */
   if (i >= k) {
      return;
   }
   
   /* Partition the data within the array. Value j returned
    from partitioning is location of last element in low partition. */
   j = Partition(words, i, k);
   
   /* Recursively sort low partition (i to j) and
    high partition (j + 1 to k) */
   Quicksort(words, i, j);
   Quicksort(words, j + 1, k);
}

void Merge(int numbers[], int i, int j, int k) {
   int mergedSize;                            // Size of merged partition
   int mergePos;                              // Position to insert merged number
   int leftPos;                               // Position of elements in left partition
   int rightPos;                              // Position of elements in right partition
   int* mergedNumbers = nullptr;

   mergePos = 0;
   mergedSize = k - i + 1;
   leftPos = i;                               // Initialize left partition position
   rightPos = j + 1;                          // Initialize right partition position
   mergedNumbers = new int[mergedSize];    // Dynamically allocates temporary array
                                              //for merged numbers

   // Add smallest element from left or right partition to merged numbers
   


   while (leftPos <= j && rightPos <= k) {
      if (numbers[leftPos] < numbers[rightPos]) {
         mergedNumbers[mergePos] = numbers[leftPos];
         ++leftPos;
      }
      else {
         mergedNumbers[mergePos] = numbers[rightPos];
         ++rightPos;
      }
      ++mergePos;
   }
   
   // If left partition is not empty, add remaining elements to merged numbers
   while (leftPos <= j) {
      mergedNumbers[mergePos] = numbers[leftPos];
      ++leftPos;
      ++mergePos;
   }
   
   // If right partition is not empty, add remaining elements to merged numbers
   while (rightPos <= k) {
      mergedNumbers[mergePos] = numbers[rightPos];
      ++rightPos;
      ++mergePos;
   }
   
   // Copy merge number back to numbers
   for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
      numbers[i + mergePos] = mergedNumbers[mergePos];
   }
   delete[] mergedNumbers;
}
void MergeSort(int numbers[], int i, int k) {
   int j;
   
   if (i < k) {
      j = (i + k) / 2;  // Find the midpoint in the partition
      
      // Recursively sort left and right partitions
      MergeSort(numbers, i, j);
      MergeSort(numbers, j + 1, k);
      // Merge left and right partition in sorted order
      Merge(numbers, i, j, k);
   }
}

int main(int argc, char *argv[]) {
   ifstream inFS1, inFS2;
   string key = argv[1];

   string value;
   int num;

   int arrSize1;
   int arrSize2;

   int i = 0;
   int j = 0;
   bool inArr;

   if(argc != 4)
   {
      cout << "Usage: ./a.out i|s input1.txt input2.txt";
      return 0;
   }
   if(key != "s" && key != "i")
   {
      cout << "Invalid datatype \"" << key << "\" (must be \"i\" or \"s\")";
      return 0;
   }

   inFS1.open(argv[2]);
   inFS2.open(argv[3]);
   if(!inFS1 || !inFS2)
   {
      cout << "Unable to open one of the input files";
      return 0;
   }
   
   //case for strings
   if(key == "s")
   {
      //build array1 from input
      string arr1[20000];
      while(!inFS1.fail())
      {
         inFS1 >> value;
         inArr = false;
         for(i = 0; i < j; i++)
         {
            //if the word doubles in input 1, dont add
            if(arr1[i] == value)
            {
               inArr = true;
               break;
            }
         }
         if(!inArr)
         {
            //add word if it is the first instance in input 1
            arr1[j] = value;
            j++;
         }
      }
      //save array1 size, reset j
      arrSize1 = j;
      j = 0;

      //build array2 from input
      string arr2[20000];
      while(!inFS2.fail())
      {
         inFS2 >> value;
         inArr = false;
         for(i = 0; i < j; i++)
         {
            //if the word doubles in input 2, dont add
            if(arr2[i] == value)
            {
               inArr = true;
               break;
            }
         }
         if(!inArr)
         {
            //add word if it is the first instance in input 2
            arr2[j] = value;
            j++;
         }
      }
      //save array2 size, reset j
      arrSize2 = j;
      j = 0;

      //build array3 from combination of input arrays and sizes
      string arr3[arrSize1+arrSize2];
      for(i = 0; i < arrSize1; i++)
      {
         arr3[i] = arr1[i];
      }
      for(i = 0; i < arrSize2; i++)
      {
         arr3[arrSize1+i] = arr2[i];
      }

      //sort array3
      Quicksort(arr3, 0, arrSize1+arrSize2-1);

      //loop through sorted array3, printing duplicates
      for(i = 0; i < arrSize1+arrSize2-1; i++)
      {
         if(arr3[i] == arr3[i+1])
         {
            cout << arr3[i] << endl;
            while(arr3[i] == arr3[i+1])
            {
               i++;
            }
         }
      }
   }

   //case for integers
   if(key == "i")
   {
      //build array1 from input
      int arr1[20000];
      while(!inFS1.fail())
      {
         inFS1 >> num;
         inArr = false;
         for(i = 0; i < j; i++)
         {
            //if number doubles in input 1, dont add
            if(arr1[i] == num)
            {
               inArr = true;
               break;
            }
         }
         //add number if it is the first instance in input 1
         if(!inArr)
         {
            arr1[j] = num;
            j++;
         }
      }
      //save array1 size, reset j
      arrSize1 = j;
      j = 0;

      //build array2 from input
      int arr2[20000];
      while(!inFS2.fail())
      {
         inFS2 >> num;
         inArr = false;
         for(i = 0; i < j; i++)
         {
            //if the number doubles in input 2, dont add
            if(arr2[i] == num)
            {
               inArr = true;
               break;
            }
         }
         if(!inArr)
         {
            //add number if it is the first instance in input 2
            arr2[j] = num;
            j++;
         }
      }
      //save array2 size, reset j
      arrSize2 = j;
      j = 0;

      //build array3 from combination of input arrays and sizes
      int arr3[arrSize1+arrSize2];
      for(i = 0; i < arrSize1; i++)
      {
         arr3[i] = arr1[i];
      }
      for(i = 0; i < arrSize2; i++)
      {
         arr3[arrSize1+i] = arr2[i];
      }

      //sort array3
      MergeSort(arr3, 0, arrSize1+arrSize2-1);

      //loop through sorted array3, printing duplicates
      for(i = 0; i < arrSize1+arrSize2-1; i++)
      {
         if(arr3[i] == arr3[i+1])
         {
            cout << arr3[i] << endl;
            while(arr3[i] == arr3[i+1])
            {
               i++;
            }
         }
      }
   }

   //close the two input filestreams
   inFS1.close();
   inFS2.close();



   return 0;
}