#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
struct Message {
    long msgtype;
    int data[4];
} Message;

int main()
{
    key_t key;
    int msgid;
    key = ftok("/home/twist/CLionProjects/untitled/cows_and_bulls_server.c",'s');
    msgid = msgget(key, 0666 | IPC_CREAT);
    int  i, a, b;
    struct Message msg;
    struct Message msg1;
    int attempt = 0;
    while (attempt < 10) {
        ++attempt;
        printf("Try to guess a number!\n");
        scanf("%d", &a);
        for (i = 3; i >= 0; i--) {
            b = a % 10;
            msg.data[i] = b;
            a = (a - b) / 10;
        }
        msg.msgtype = 1;
        msgsnd(msgid, (struct msgbuf*) (&msg), 4*sizeof(int), 0);
        msgrcv(msgid, (struct msgbuf*) (&msg1), 4*sizeof(int), 2, 0);
        if (msg1.data[0] == 4) {
            printf("You are fantastic!!!\n");
            return 0;
        } else {
            printf("Try one more time. Your result is %d bulls and %d cows\n", msg1.data[0], msg1.data[1]);
        }
        for (int o = 0; o < 4; o++) {
            msg1.data[o] = 0;
        }
    }
    printf("Sorry, you lost in this game:(\n");
    return 0;
}