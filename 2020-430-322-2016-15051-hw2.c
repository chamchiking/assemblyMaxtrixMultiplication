// This code is for 430.322 Project #2
// Student ID        : 2016-15051
// Student name      : mincheol cha
// Student email     : chamj61047@snu.ac.kr

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define nMAX 64
#define iMAX 1
#define rMAX 128

#define read_csr(reg) ({uint64_t __tmp; asm volatile ("csrr %0, " #reg : "=r"(__tmp)); __tmp;})

void counting(uint32_t count[], uint32_t array[]){
  asm(" \n\
		addi t3, a1, 256  \n\
		add  t0, a1, zero \n\
        countloop: \n\
		lw   t1, 0(t0)  \n\
		slli t1, t1, 2  \n\
		add  t2, a0, t1 \n\
		lw   t1, 0(t2)  \n\
		addi t1, t1, 1  \n\
		sw   t1, 0(t2)  \n\
		addi t0, t0, 4 \n\
		bne  t0, t3, countloop \n\
		addi t3, a0, 508 \n\
		addi t0, a0, -4 \n\
		add  t1, a1, zero \n\
	cloop: \n\
		addi t0, t0, 4 \n\
		lw   t2, 0(t0)  \n\
		beq  t2, zero, cloop \n\
		slli t2, t2, 2 \n\
		add t5, t2, t1 \n\
		sub t6, t0, a0 \n\
		srli t6, t6, 2 \n\
	aloop: \n\
		sw t6, 0(t1) \n\
		addi t1, t1, 4 \n\
		bne t1, t5, aloop \n\
		bne t0, t3, cloop \n\
  ");
}
//void putback(uint32_t count[], uint32_t array[]){
// asm(" \n\
		addi t3, a0, 508  \n\
		addi t0, a0, -4 \n\
		add  t1, a1, zero \n\
	cloop: \n\
		addi t0, t0, 4 \n\
		lw   t2, 0(t0) \n\
		beq  t2, zero, cloop \n\
		slli t2, t2, 2 \n\
		add  t5, t2, t1 \n\
		sub  t6, t0, a0 \n\
		srli t6, t6, 2 \n\
	aloop: \n\
		sw   t6, 0(t1) \n\
		addi t1, t1, 4 \n\
		bne  t1, t5, aloop \n\
		bne  t0, t3, cloop \n\
// ");
//}

void your_sort(uint32_t array[])
{
  // TODO: Please implement your sort algorithm here. 
  // The less execution cycle, the better grade.
  
  uint32_t count[rMAX] = {0, };
  counting(count, array);
  //putback(count, array);
  //int k = 0;
  //for (int i=0;i<rMAX; i++){
  //	for (int j = 0; j<count[i]; j++){
  //		array[k++] = i;
  //	}
  //}
/*******************************************************/
}

// Reference code (Bubble sort)
void bubble_sort(uint32_t array[])
{
  int i, j;
  uint32_t temp;
  for (i = 0; i < (nMAX - 1); i++)
  {
    for (j = 0; j < (nMAX - i - 1); j++)
    {
      if (array[j] > array[j + 1])
      {
        temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
}

// Check the sorted results between 
// reference code and your code implemented.
void check(uint32_t array1[], uint32_t array2[])
{
  int i;
  uint32_t temp = 0;
  for (i = 0; i < nMAX; i++)
  {
    if (array1[i] != array2[i])
    {
      printf("Error : Wrong at array random_num[%d]\n",i);
      temp = 1;
    }
  }
  if (temp == 0)
  {
    printf("Success!\n");
  }
}


int main()
{
  int i, j;
  uint32_t random_nums[nMAX] = {0, };
  uint32_t your_nums[nMAX]   = {0, };

  uint64_t start_inst, end_inst;
  uint64_t start_cycle, end_cycle;

  srand(0);
  for (i = 0; i < iMAX; i++)
  {
    for (j = 0; j < nMAX; j++)
    {
      random_nums[j] = rand() % rMAX;
      your_nums[j] = random_nums[j];
    }
    bubble_sort(random_nums);


    
    start_inst = read_csr(instret); // read the current instruction count from instret csr register.
    start_cycle = read_csr(cycle);  // read the current cycle from cycle csr register.


    //** run your function **//
    your_sort(your_nums);
    //***********************//
    
    end_inst = read_csr(instret);  // read the current instruction count from instret csr register.
    end_cycle = read_csr(cycle);   // read the current cycle from cycle csr register.

    printf("Instruction counts = %ld\n", end_inst - start_inst);
    printf("Execution cycles = %ld\n", end_cycle - start_cycle);
    printf("IPC = %f\n", ((float)((double)(end_inst - start_inst))/((double)(end_cycle - start_cycle))));

    check(random_nums, your_nums);
  }

  return 0;
}
