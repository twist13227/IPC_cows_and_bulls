#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
struct Message {
    long msgtype;
    int data[4];
} Message;

int main()
{
    srand(time(NULL));
    int number[4], cows, bulls, i, j;
    int f = 0;
    i = 0;
    while (i < 4) {
        number[i] = (rand() % 6) + 1;
        for (j = 0; j < i; j++) {
            if (number[i] == number[j]) {
                f = 1;
                break;
            }
        }
        if (f == 0) {
            i++;
        }
        f = 0;
    }
    key_t key;
    int msgid;
    key = ftok("/home/twist/CLionProjects/untitled/cows_and_bulls_server.c",'s');
    msgid = msgget(key, 0666 | IPC_CREAT);
    struct Message msg;
    struct Message msg1;
    msg1.msgtype = 2;
    for (int k = 0; k < 10; k++) {
        bulls = 0;
        cows = 0;
        msgrcv(msgid, (struct msgbuf*) (&msg), 4*sizeof(int), 1, 0);
        for (i = 0; i < 4; i++) {
            if (msg.data[i] == number[i]) {
                bulls++;
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (msg.data[i] == number[j]) {
                    cows++;
                }
            }
        }
        cows = cows - bulls;
        msg1.data[0] = bulls;
        msg1.data[1] = cows;
        msg1.data[2] = 0;
        msg1.data[3] = 0;
        msgsnd(msgid, (struct msgbuf*) (&msg1), 4*sizeof(int), 0);
        if (bulls == 4) {
            break;
        }
        for (int o = 0; o < 4; o++) {
            msg1.data[o] = 0;
            msg.data[o] = 0;
        }
    }
    sleep(10);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}