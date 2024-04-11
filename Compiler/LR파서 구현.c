#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

void LR_Parser(char* input); // LR_Parser �Լ� ����

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

int is_empty() {// ������ ����� ��
    return sp == -1;
}

int is_full() {// ������ ����á�� ��
    return (sp >= MAX - 1);
}

void push(int input) {// push(shift)�Լ� ����
    if (is_full()) {
        printf("������ ���� á���ϴ�.");
    }
    else {
        stack[++sp] = input;
    }
}

void pop() {//pop(reduce)�Լ� ����
    if (is_empty()) {
        printf("������ ������ϴ�.");
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

void LR_Parser(char* input) { //LR�ļ� ����
    sp = -1; //���� ������ �ʱ�ȭ
    int i = 0;
    int state = 0;//���� ����
    int j = 1;
    push(state); // ���ÿ� 0 ����
    printf("\n(%d) initial :", j); // �ʱ� ���� ���
    for (int k = 0; k < sp + 1; k++) {
        printf(" %d ", stack[k]);
    }
    printf("    %s", input);

    while (1) {
        char lookahead = input[i];


        printf("\n(%d)", j+1);

        // ��ū �и�, ������ ��ū�� ������ ��ū ���Խ� �����޼��� ��� �� ����
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

        // action���̺��� actionã��
        int action = action_tbl[state][action_index];

        //action�� ����� ��, shift ���� ����
        if (action > 0 && action != 999) {
            state = action; // ���� ���� ������ ����
            push(input[i]); // ��ū ����
            push(state); // ������ ���°� ����
            printf(" Shift %d:", action);
            i++;
            j++;
            printf(" ");
            for (int k = 0; k <= sp; k++) { // ���� ���
                if (stack[k] >= 32 && stack[k] <= 126) {
                    printf("%c", stack[k]);
                }
                else {
                    printf("%d", stack[k]);
                }
            }
            printf("   ");
            printf("%s", &input[i]); // input���
        }

        //action�� ������ ��, Reduce ���� ����
        else if (action < 0) {
            int rule_num = abs(action); //action�� ���밪
            int len; // ���� ����
            len = rhs_len[rule_num];
            printf(" Reduce %d:", rule_num);
            for (int j = 0; j < 2 * len; j++) {//������ 2�踦 pop��
                pop();
            }
            char spell = lhs[rule_num];//E, T, F ����

            int goto_index;//E, T, F�� �����ϰ� �ε����� �ο�
            if (spell == ' ')
                goto_index = 0;
            else if (spell == 'E')
                goto_index = 1;
            else if (spell == 'T')
                goto_index = 2;
            else if (spell == 'F')
                goto_index = 3;

            state = goto_tbl[stack[sp]][goto_index]; 
            //���� ���������Ϳ� ������ ������ �ε����� goto���̺��� ���¸� ����

            push(spell);
            push(state);
            j++;

            printf(" ");
            for (int k = 0; k <= sp; k++) {//���� ���
                if (stack[k] >= 32 && stack[k] <= 126) {
                    printf("%c", stack[k]);
                }
                else {
                    printf("%d", stack[k]);
                }
            }
            printf("   ");
            printf("%s", &input[i]);//input ���
        }
        else if (action == 999) {//999�϶� accept
            printf("accept\n");
            break;
        }
        else {
            printf("error\n");
            break;
        }
    }

}