#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

void LR_Parser(char* input); // LR_Parser 함수 선언

int action_tbl[12][6] = {
    { 5, 0, 0, 4, 0, 0 },
    { 0, 6, 0, 0, 0, 999 },
    { 0, -2, 7, 0, -2, -2 },
    { 0, -4, -4, 0, -4, -4 },
    { 5, 0, 0, 4, 0, 0 },
    { 0, -6, -6, 0, -6, -6},
    { 5, 0, 0, 4, 0, 0 },
    { 5, 0, 0, 4, 0, 0 },
    { 0, 6, 0, 0, 11, 0 },
    { 0, -1, 7, 0, -1, -1 },
    { 0, -3, -3, 0, -3, -3 },
    { 0, -5, -5, 0, -5, -5 }
};
int goto_tbl[12][4] = {
    { 0, 1, 2, 3 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 8, 2, 3 },
    { 0, 0, 0, 0 },
    { 0, 0, 9, 3 },
    { 0, 0, 0, 10 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

char lhs[] = { ' ', 'E', 'E', 'T', 'T', 'F', 'F' }; // dummy in 0 index
int rhs_len[] = { 0, 3, 1, 3, 1, 3, 1 }; // rhs length: 0 for dummy rule
char token[] = { 'd', '+', '*', '(', ')', '$' };
char NT[] = { ' ', 'E', 'T', 'F' }; // non-terminals: dummy in 0 index
int stack[MAX], sp;

int is_empty() {// 스택이 비었을 때
    return sp == -1;
}

int is_full() {// 스택이 가득찼을 때
    return (sp >= MAX - 1);
}

void push(int input) {// push(shift)함수 구현
    if (is_full()) {
        printf("스택이 가득 찼습니다.");
    }
    else {
        stack[++sp] = input;
    }
}

void pop() {//pop(reduce)함수 구현
    if (is_empty()) {
        printf("스택이 비었습니다.");
    }
    else {
        return stack[sp--];
    }
}

int main() {
    char input[MAX];
    while (1) {
        printf("\nInput: ");
        scanf("%s", input);
        if (input[0] == '$')
            break;
        LR_Parser(input);
    }
    return 0; 
}

void LR_Parser(char* input) { //LR파서 구현
    sp = -1; //스택 포인터 초기화
    int i = 0;
    int state = 0;//상태 변수
    int j = 1;
    push(state); // 스택에 0 삽입
    printf("\n(%d) initial :", j); // 초기 상태 출력
    for (int k = 0; k < sp + 1; k++) {
        printf(" %d ", stack[k]);
    }
    printf("    %s", input);

    while (1) {
        char lookahead = input[i];


        printf("\n(%d)", j+1);

        // 토큰 분리, 정해진 토큰을 제외한 토큰 삽입시 에러메세지 출력 후 종료
        int action_index = 0;
        if (lookahead == 'd')
            action_index = 0;
        else if (lookahead == '+')
            action_index = 1;
        else if (lookahead == '*')
            action_index = 2;
        else if (lookahead == '(')
            action_index = 3;
        else if (lookahead == ')')
            action_index = 4;
        else if (lookahead == '$')
            action_index = 5;
        else {
            printf("invlid token (-) error\n");
            break;
        }

        // action테이블에서 action찾기
        int action = action_tbl[state][action_index];

        //action이 양수일 때, shift 동작 수행
        if (action > 0 && action != 999) {
            state = action; // 현재 상태 변수로 설정
            push(input[i]); // 토큰 삽입
            push(state); // 현재의 상태값 삽입
            printf(" Shift %d:", action);
            i++;
            j++;
            printf(" ");
            for (int k = 0; k <= sp; k++) { // 스택 출력
                if (stack[k] >= 32 && stack[k] <= 126) {
                    printf("%c", stack[k]);
                }
                else {
                    printf("%d", stack[k]);
                }
            }
            printf("   ");
            printf("%s", &input[i]); // input출력
        }

        //action이 음수일 때, Reduce 동작 수행
        else if (action < 0) {
            int rule_num = abs(action); //action의 절대값
            int len; // 길이 변수
            len = rhs_len[rule_num];
            printf(" Reduce %d:", rule_num);
            for (int j = 0; j < 2 * len; j++) {//길이의 2배를 pop함
                pop();
            }
            char spell = lhs[rule_num];//E, T, F 구별

            int goto_index;//E, T, F를 구별하고 인덱스를 부여
            if (spell == ' ')
                goto_index = 0;
            else if (spell == 'E')
                goto_index = 1;
            else if (spell == 'T')
                goto_index = 2;
            else if (spell == 'F')
                goto_index = 3;

            state = goto_tbl[stack[sp]][goto_index]; 
            //현재 스택포인터와 위에서 구별한 인덱스로 goto테이블의 상태를 구함

            push(spell);
            push(state);
            j++;

            printf(" ");
            for (int k = 0; k <= sp; k++) {//스택 출력
                if (stack[k] >= 32 && stack[k] <= 126) {
                    printf("%c", stack[k]);
                }
                else {
                    printf("%d", stack[k]);
                }
            }
            printf("   ");
            printf("%s", &input[i]);//input 출력
        }
        else if (action == 999) {//999일때 accept
            printf("accept\n");
            break;
        }
        else {
            printf("error\n");
            break;
        }
    }

}