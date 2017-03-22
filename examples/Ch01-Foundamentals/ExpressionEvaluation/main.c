/* Copyright (C) 2017, kylinsage <kylinsage@gmail.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Vector.h"
#include <stdio.h>

/**
 * Example of simple binary arithmetic operation
 * Operators shouble be +, -, *, and / (not including unary operation +/-).
 * Operands should be positive integral numbers from 0 to 9.
 */

VECTOR_DECL(operator_stack, char, OperatorStack)
VECTOR_IMPL(operator_stack, char, OperatorStack)

VECTOR_DECL(operand_stack, int, OperandStack)
VECTOR_IMPL(operand_stack, int, OperandStack)

void show_operator_stack(OperatorStack * stack)
{
	printf("Operator Stack:\n");
	for (int i = 0; i < stack->size; i++) {
		char t;
		if (operator_stack_get(stack, i, &t))
			printf("[%2d]\t%c\n", i, t);
	}
}

void show_operand_stack(OperandStack * stack)
{
	printf("Operand Stack:\n");
	for (int i = 0; i < stack->size; i++) {
		int t;
		if (operand_stack_get(stack, i, &t))
			printf("[%2d]\t%d\n", i, t);
	}
}

int operator_rank(char operator)
{
	if (operator == '+' || operator == '-')
		return 1;
	else if (operator == '*' || operator == '/')
		return 2;
	else
		return 0;
}

void calculate(char operator, OperandStack * operandStack)
{
	int a, b;
	operand_stack_get_back(operandStack, &b);
	operand_stack_pop_back(operandStack);
	operand_stack_get_back(operandStack, &a);
	operand_stack_pop_back(operandStack);
	printf(">> Calculating %d %c %d ...\n", a, operator, b);
	switch (operator) {
	case '+':
		operand_stack_push_back(operandStack, a + b);
		break;
	case '-':
		operand_stack_push_back(operandStack, a - b);
		break;
	case '*':
		operand_stack_push_back(operandStack, a * b);
		break;
	case '/':
		operand_stack_push_back(operandStack, a / b);
		break;
	}
}

int main(int argc, char *argv[])
{
	OperatorStack operatorStack;
	OperandStack operandStack;

	operator_stack_init(&operatorStack);
	operand_stack_init(&operandStack);

	char expression[] = "8+3-4*5+9/4*2";
	char *p = expression;
	while (*p) {
		printf(">> Scanning %c ...\n", *p);
		if (*p >= '0' && *p <= '9') {
			operand_stack_push_back(&operandStack, *p - '0');
		} else {
			char operator;
			while (operator_stack_get_back
			       (&operatorStack, &operator)
			       && operator_rank(operator) >= operator_rank(*p)) {
				calculate(operator, &operandStack);
				operator_stack_pop_back(&operatorStack);
			}
			operator_stack_push_back(&operatorStack, *p);
		}
		p++;
	}
	printf("   [ok] Scanning finished!\n");

	char operator;
	while (operatorStack.size > 0) {
		operator_stack_get_back(&operatorStack, &operator);
		calculate(operator, &operandStack);
		operator_stack_pop_back(&operatorStack);
	}
	printf("   [ok] Calculating finished!\n");

	show_operand_stack(&operandStack);

	operator_stack_destroy(&operatorStack);
	operand_stack_destroy(&operandStack);

	return 0;
}
