#include <stdio.h>
#include "stack.h"
#include "expression_evaluator.h"
#include <locale.h>

int main() {
	
	struct Stack* begin;
	begin = NULL;
	
	puts("1. Input Stack");
	puts("2. View Stack");
	puts("3. Find Most Frequent Element In Stack");
	puts("4. Input Ascending Stacks");
	puts("0. Exit");
	while(1){
		char ch = _getch();
		if (ch == '1') {
			fillStackFromKeyboard(&begin);
		}
		else if (ch == '2') {
			printStack(begin);
		}
		else if (ch == '3') {
			int n = findMostFrequentElement(begin);
			printf("Most Frequent element: %d", n);
		}
		else if (ch == '4') {
			struct Stack* begin1;
			begin1 = NULL;
			struct Stack* begin2;
			begin2 = NULL;
			fillAscendingStack(&begin1);
			fillAscendingStack(&begin2);
			printf("Stack 1: ");
			printStack(begin1);
			puts(" ");
			printf("Stack 2: ");
			printStack(begin2);

			struct Stack* result = mergeAscendingStacks(begin1, begin2);

			printf("Merged Stack: ");
			printStack(result);
		}
		else if (ch == '0') break;
	}
	




	setlocale(LC_ALL, "russian");
	writeExpressionsToFile();

	FILE* inputFile = fopen("input.txt", "r");
	FILE* outputFile = fopen("output.txt", "w");

	if (!inputFile || !outputFile) {
		printf("Error.\n");
		return 1;
	}

	char line[256];
	while (fgets(line, sizeof(line), inputFile)) {
		int errorPosition = -1;
		char* postfix = infixToPostfix(line, &errorPosition);
		if (errorPosition == -1) {
			EvaluationResult evalResult = evaluatePostfix(postfix);
			if (evalResult.errorPosition == -1) {
				fprintf(outputFile, "%lf\n", evalResult.result);
			}
			else {
				fprintf(outputFile, "Error in position %d\n", evalResult.errorPosition);
			}
		}
		else {
			fprintf(outputFile, "Error in position  %d\n", errorPosition);
		}
		free(postfix);
	}

	fclose(outputFile);
	fclose(inputFile);

	printFile("input.txt");

	outputFile = fopen("output.txt", "r");
	if (outputFile != NULL) {
		fclose(outputFile);
		printFile("output.txt");
	}

	return 0;
}