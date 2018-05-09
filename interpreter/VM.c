#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "vm.h"

CodeBlock *codeBlock;
WORD* stack;
WORD* global;
int t;
int b;
int pc;
int ps;
int stackSize;
int codeSize;

void resetVM(void) {
  pc = 0;
  t = -1;
  b = 0;
  ps = PS_INACTIVE;
}

void initVM(void) {
  codeBlock = createCodeBlock(codeSize);
  stack = (Memory) malloc(stackSize * sizeof(WORD));
  resetVM();
}

void cleanVM(void) {
  freeCodeBlock(codeBlock);
  free(stack);
}

int loadExecutable(FILE* f) {
  loadCode(codeBlock,f);
  resetVM();
  return 1;
}


int checkStack(void) {
  return ((t >= 0) && (t <stackSize));
}

int base(int p) {
  int currentBase = b;
  while (p > 0) {
    currentBase = stack[currentBase + 3];
    p --;
  }
  return currentBase;
}

int run(void) {
  Instruction* code = codeBlock->code;
  int count = 0;
  int number;
 
  ps = PS_ACTIVE;
  while (ps == PS_ACTIVE) {

    switch (code[pc].op) {
    case OP_LA: 
      t++;
      if(checkStack())
      	stack[t] = base(code[pc].p) + code[pc].q;
      break;
    case OP_LV: 
      t++;
      if(checkStack())
      	stack[t] = stack[base(code[pc].p) + code[pc].q];
      break;
    case OP_LC: 
      t++;
      if(checkStack())
      	stack[t] = code[pc].q;
      break;
    case OP_LI: 
      stack[t] = stack[stack[t]];
      break;
    case OP_INT:
      t = t + code[pc].q;
      checkStack();
      break;
    case OP_DCT: 
      t = t - code[pc].q;
      checkStack();
      break;
    case OP_J: 
      pc = code[pc].q - 1;
      break;
    case OP_FJ: 
      if(stack[t] == FALSE)
      	pc = code[pc].q - 1;
	  t--;
	  checkStack();	
      break;
    case OP_HL: 
      ps = PS_NORMAL_EXIT;
      break;
    case OP_ST: 
      stack[stack[t-1]] = stack[t];
      t -= 2;
      checkStack();
      break;
    case OP_CALL: 
      stack[t+2] = b;
      stack[t+3] = pc;
      stack[t+4] = base(code[pc].p);
      b = t + 1;
      pc = code[pc].q - 1;
      break;
    case OP_EP: 
      t = b - 1;
      pc = stack[b+2];
      b = stack[b+1];
      break;
    case OP_EF:
      t = b;
	  pc = stack[b+2];
	  b = stack[b+1];                 
      break;
    case OP_RC: 
      t ++;
      scanf("%c",&number);
      stack[t] = number;
      checkStack();
      break;
    case OP_RI:
      t ++;
      scanf("%d",&number);
      stack[t] = number;
      checkStack();
      break;
    case OP_WRC: 
      printf("%c",stack[t]);
      t --;
      checkStack();
      break;     
    case OP_WRI: 
      printf("%d",stack[t]);
      t --;
      checkStack();
      break;
    case OP_WLN:
      printf("\n");
      break;
    case OP_AD:
      t --;
      if (checkStack()) 
	stack[t] += stack[t+1];
      break;
    case OP_SB:
      t --;
      if (checkStack()) 
	stack[t] -= stack[t+1];
      break;
    case OP_ML:
      t --;
      if (checkStack()) 
	stack[t] *= stack[t+1];
      break;

    case OP_DV: 
      t --;
      if (checkStack()) {
	if (stack[t+1] == 0)
	  ps = PS_DIVIDE_BY_ZERO;
	else stack[t] /= stack[t+1];
      }
      break;
    case OP_NEG:
      stack[t] = - stack[t];
      break;
    case OP_CV: 
      stack[t+1] = stack[t];
      t ++;
      checkStack();
      break;
    case OP_EQ:
      t --;
      if (stack[t] == stack[t+1]) 
         stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_NE:
      t --;
      if (stack[t] != stack[t+1]) 
	     stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    case OP_GT:
      t --;
      if (stack[t] > stack[t+1]) 
	     stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_LT:
      t --;
      if (stack[t] < stack[t+1]) 
	     stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_GE:
      t --;
      if (stack[t] >= stack[t+1]) 
	stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    case OP_LE:
      t --;
      if (stack[t] <= stack[t+1]) 
	stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    default: break;
    }
    
    pc ++;
  }
  printf("\nPress any key to exit...");getch();
  return ps;
}
