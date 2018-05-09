#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"

#define MAX_BLOCK 50

CodeBlock* createCodeBlock(int maxSize) {
  CodeBlock* codeBlock = (CodeBlock*) malloc(sizeof(CodeBlock));

  codeBlock->code = (Instruction*) malloc(maxSize * sizeof(Instruction));
  codeBlock->codeSize = 0;
  codeBlock->maxSize = maxSize;
  return codeBlock;
}

void freeCodeBlock(CodeBlock* codeBlock) {
  free(codeBlock->code);
  free(codeBlock);
}


void loadCode(CodeBlock* codeBlock, FILE* f) {
  Instruction* code = codeBlock->code;
  int n;
  char c;

  codeBlock->codeSize = 0;
  while (fscanf(f, "%d", &(code->op))>0) {
	c = fgetc(f);
	if(c == ' '){
			fscanf(f, "%d", &(code->p));
			c = fgetc(f);
			if(c == ' '){
				fscanf(f,"%d", &(code->q));
				c = fgetc(f);
			}
		}
    code += 1;
    codeBlock->codeSize += 1;
  }
}

