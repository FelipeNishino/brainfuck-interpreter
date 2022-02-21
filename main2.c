//
// Created by Nishi on 21/02/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEMORY_SIZE 30000
#define BUFFER_SIZE 100000

#define ANGLE_BRACKET_O 60
#define ANGLE_BRACKET_C 62
#define BRACKET_O 91
#define BRACKET_C 93
#define PLUS 43
#define MINUS 45
#define DOT 46
#define COMMA 44

typedef struct stack {
    char **list;
    int top;
    int max;
} Stack;

typedef struct tape {
    char *tape;
    char *head;
} Tape;

typedef struct subcode {
    char *start;
    char *end;
} Subcode;

typedef struct instance {
    Tape input;
    Tape code;
    int id;
} Instance;

void initStack(Stack *stack, int size);
void initTape(Tape *t, int size);
char *findTapeTail(Tape t);
void resetTapeHead(Tape *t);
bool runInstance(Instance instance);
bool validateCode(Tape *code);
void executeCode(Tape *memory, Tape *code, Tape *input, Subcode subcode);
void executeSubcode(Tape *memory, Tape *code, Tape* input, Subcode subcode);
bool isEmpty(Stack *stack);
bool isFull(Stack *stack);
char *peek(Stack *stack);
char *pop(Stack *stack);
void push(Stack *stack, char *data);

void initStack(Stack *stack, int size) {
    stack->max = size;
    stack->list = malloc(stack->max * sizeof(char *));
    for(int i = 0; i < stack->max; ++i)
        stack->list[i] = 0;
    stack->top = 0;
}

void initTape(Tape *t, int size) {
    t->tape = malloc(size * sizeof(char));
    resetTapeHead(t);
}

char *findTapeTail(Tape t) {
    int i = 0;
    while (t.tape[++i] != '\0')
        continue;
    return &t.tape[i-1];
}

void resetTapeHead(Tape *t) {
    t->head = t->tape;
}

void printTape(Tape *t) {
    int i = 0;
    while (t->tape[i] != 0)
        printf("%c", t->tape[i++]);
}

int main() {
    int instanceCount, i;
    char buffer[BUFFER_SIZE];
    Tape code, input;

    // Read instance count from first line
    gets(buffer);
    instanceCount = atoi(buffer);

    Instance instances[instanceCount];

    for (i = 0; i < instanceCount; i++) {
        initTape(&code, BUFFER_SIZE);
        initTape(&input, BUFFER_SIZE);
        gets(buffer);
        gets(input.tape);
        gets(code.tape);
        instances[i] = (Instance) {.code = code, .input = input, .id = i + 1};
    }

    for (i = 0; i < instanceCount; i++)
        runInstance(instances[i]);

    return 0;
}

bool validateCode(Tape *code) {
    Stack brackets;
    initStack(&brackets, 100);

    while (*code->head) {
        switch (*code->head) {
            case BRACKET_O:
                push(&brackets, code->head);
                break;
            case BRACKET_C:
                pop(&brackets);
                break;
            default:
                break;
        }
        ++code->head;
    }
    resetTapeHead(code);
    return isEmpty(&brackets);
}

char *findEnclosingBracket(Tape *code, char *start) {
    Stack brackets;
    initStack(&brackets, 100);
    char *tempHead = start;
//    int i=0;
    push(&brackets, tempHead++);

//    printf("Cheguei aqui, start {%c}", *start);
    while (*tempHead != '\0') {
        switch (*tempHead) {
            case BRACKET_O:
                push(&brackets, tempHead);
                break;
            case BRACKET_C:
                pop(&brackets);
                if (isEmpty(&brackets)) {
//                    printf("found enclosing {%c} on index {%d}", *tempHead, i);
                    return tempHead;
                }
                break;
            default:
                break;
        }
        ++tempHead;
//        i++;
    }
}

void executeSubcode(Tape *memory, Tape *code, Tape *input, Subcode subcode) {
    Stack brackets;
    initStack(&brackets, 100);
    char *tempSubcode;

    code->head = subcode.start;

    while (*code->head) {
        switch(*code->head){
            case ANGLE_BRACKET_O:
                --memory->head;
                break;
            case ANGLE_BRACKET_C:
                ++memory->head;
                break;
            case BRACKET_O:
                if (code->head != subcode.start)
                    push(&brackets, code->head);
                else
                    break;
            case BRACKET_C:
                if (code->head != subcode.end) {
                    tempSubcode = pop(&brackets);
                    if (isEmpty(&brackets)) {
                        executeSubcode(memory, code, input, (Subcode) {.start = tempSubcode, .end = code->head});
                    }
                }
                else {
                    if (*memory->head == 0)
                        return;
                    if (code->head == subcode.end)
                        code->head = subcode.start - 1;
                }
                break;
            case PLUS:
                ++*memory->head;
                break;
            case MINUS:
                --*memory->head;
                break;
            case DOT:
                printf("%c", *memory->head);
                break;
            case COMMA:
                if (*input->head != '\0') {
                    *memory->head = *input->head;
                    ++input->head;
                }
                else
                    *memory->head = 0;
                break;
        }
        ++code->head;
    }
}

//void executeCode(Tape *memory, Tape *code, Tape *input){
//    Stack brackets;
//    initStack(&brackets, 100);
//    bool pauseExec = false;
//    char *tempSubcode;
//
//    while (*code->head) {
//        if (pauseExec && *code->head != BRACKET_C) {
//            ++code->head;
//            continue;
//        }
//        switch(*code->head){
//            case ANGLE_BRACKET_O:
//                --memory->head;
//                break;
//            case ANGLE_BRACKET_C:
//                ++memory->head;
//                break;
//            case BRACKET_O:
//                push(&brackets, code->head);
//                pauseExec = true;
//                break;
//            case BRACKET_C:
//                tempSubcode = pop(&brackets);
//                if (isEmpty(&brackets)){
//                    executeSubcode(memory, code, input, (Subcode){.start = tempSubcode, .end = code->head});
//                    pauseExec = false;
//                }
//                break;
//            case PLUS:
//                ++*memory->head;
//                break;
//            case MINUS:
//                --*memory->head;
//                break;
//            case DOT:
//                printf("%c", *memory->head);
//                break;
//            case COMMA:
//                if (*input->head != '\0') {
//                    *memory->head = *input->head;
//                    ++input->head;
//                }
//                else
//                    *memory->head = 0;
//                break;
//        }
//        ++code->head;
//    }
//
//    printf("\n");
//}

void executeCode(Tape *memory, Tape *code, Tape *input, Subcode subcode){
    Stack brackets;
    initStack(&brackets, 100);
    bool pauseExec = false;
    Subcode tempSubcode;

    while (*code->head) {
//        if (pauseExec && *code->head != BRACKET_C) {
//            ++code->head;
//            continue;
//        }
        switch(*code->head){
            case ANGLE_BRACKET_O:
//                printf("{<}\n");
                --memory->head;
                break;
            case ANGLE_BRACKET_C:
//                printf("{>}\n");
                ++memory->head;
                break;
            case BRACKET_O:
//                printf("{[}\n");
//                push(&brackets, code->head);
                tempSubcode.end = findEnclosingBracket(code, code->head);
                code->head++;
                tempSubcode.start = code->head;
//                printf("Starting loop at {%c}\n", *tempSubcode.start);
                while(*memory->head != 0) {
                    executeCode(memory, code, input, tempSubcode);
                    code->head = tempSubcode.start;
//                    printf("loopou\n");
                }
                code->head = tempSubcode.end;
//                pauseExec = true;
                break;
            case BRACKET_C:
//                printf("{]}\n");
//                tempSubcode = pop(&brackets);
//                if (isEmpty(&brackets)){
//                    executeSubcode(memory, code, input, (Subcode){.start = tempSubcode, .end = code->head});
//                    pauseExec = false;
//                }
                break;
            case PLUS:
//                printf("{+}\n");
                ++*memory->head;
                break;
            case MINUS:
//                printf("{-}\n");
                --*memory->head;
                break;
            case DOT:
//                printf("{.}\n");
                printf("%c", *memory->head);
                break;
            case COMMA:
//                printf("{,}\n");
                if (*input->head != '\0') {
                    *memory->head = *input->head;
                    ++input->head;
                }
                else
                    *memory->head = 0;
                break;
        }
        if (code->head == subcode.end) {
//            printf("Saindo do loop\n");
            return;
        }

        ++code->head;
    }

    printf("\n");
}

bool runInstance(Instance instance) {
    Tape memory;
    initTape(&memory, MEMORY_SIZE);

    for (int i = 0; i < MEMORY_SIZE; i++)
        memory.tape[i] = 0;

    printf("Instancia %d\n", instance.id);

    if (!validateCode(&instance.code)) {
        printf("Codigo Invalido!\n\n");
        return false;
    }

//    printf("endchar:{%c}", *findTapeTail(instance.code));
    executeCode(&memory, &instance.code, &instance.input, (Subcode) {.start = instance.code.tape, .end = findTapeTail(instance.code)});

    printf("\n");

    return true;
}

bool isEmpty(Stack *stack) {
    return stack->top == 0;
}

bool isFull(Stack *stack) {
    return stack->top == stack->max;
}

char *peek(Stack *stack) {
    return stack->list[stack->top - 1];
}

char *pop(Stack *stack) {
    char *data = NULL;
    if(!isEmpty(stack))
        data = stack->list[(stack->top--) - 1];
    else
        printf("Could not retrieve data, Stack is empty.\n");
    return data;
}

void push(Stack *stack, char *data) {
    if(!isFull(stack))
        stack->list[stack->top++] = data;
    else
        printf("Could not insert data, Stack is full.\n");
}
