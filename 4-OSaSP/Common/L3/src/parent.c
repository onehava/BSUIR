#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#include <stdlib.h>
#include <unistd.h>

#include "parent.h"

#define log(...) fprintf(stderr, __VA_ARGS__)

static process_t processes[PROCESS_MAX];
static num_t count;

void setup_state(){
    // установить обработчик atexit
    if(atexit(atexit_hd)){
        log("error: failed to set atexit handler\n");
        exit(EXIT_FAILURE);
    }

    // Init alarm handler
    if(signal(SIGALRM, ptimer_hd) == SIG_ERR){
        perror("signal");
        exit(errno);
    }

    // установить обработчик сигналов дочернего процесса
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = extern_hd;

    if(sigaction(SIGUSR2, &action, NULL)){
        perror("sigaction");
        exit(errno);
    }
}

void print_state(){
    // if(count == 0){
    //     log("warning: there are no process to print\n");
    //     return;
    // }
    if(count == 0){
        printf("current parent state: empty\n");
        return;
    }

    printf("current parent state:\n");

    for(num_t i = 0; i < count; i++){
        process_t process = processes[i];

        printf("\tnumber %d: pid %d, is_mute %u\n",
                i, process.pid, process.is_mute);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

void create_process(const char* path){
    pid_t pid = fork();

    switch(pid){
        case 0:{
            char name[16];
            sprintf(name, "CHILD_%u", count);

            if(execl(path, name, NULL) == -1){
                perror("execl");
                exit(errno);
            }
        }

        case -1:
            perror("fork");
            exit(errno);

        default:
            log("info: new process (num = %d, pid = %d) has been created\n",
                count, pid);

            processes[count].pid = pid;
            processes[count].is_mute = 0;
            count++;

    }
}

#pragma GCC diagnostic pop

void pop_process(){
    if(count){
        send_message(count - 1, MSG_KILL);
        count--;
    }
}

void remove_processes(){
    while(count){
        pop_process();
    }
}

void mute_process(num_t num){
    if(num >= count){
        return;
    }

    processes[num].is_mute = 1;
}

void mute_processes(){
    for(num_t i = 0; i < count; i++){
        mute_process(i);
    }
}

void unmute_process(num_t num){
    if(num >= count){
        return;
    }

    processes[num].is_mute = 0;
}

void unmute_processes(){
    for(num_t i = 0; i < count; i++){
        unmute_process(i);
    }
}

num_t search_process(pid_t pid){
    for(num_t i = 0; i < count; i++){
        //
        if(processes[i].pid == pid){
            return i;
        }
    }

    return -1;
}

void send_message(num_t num, enum parent message){
    if(num >= count){
        return;
    }

    process_t process = processes[num];

    union sigval msg = {message};

    if(sigqueue(process.pid, SIGUSR1, msg)){
        perror("sigqueue");
        exit(errno);
    }

    static const char* const names[] = {
        "MSG_KILL",
        "MSG_STAT"
    };

    log("info: SIGUSR1 [%s] has been sent to %d\n",
        *(names + message), num);
}

void broadcast_send_message(pid_t exclude, enum parent message){
    //
    for(process_t* process = processes; process < processes + count; process++){
        if(process->pid != exclude){
            send_message(process->pid, message);
        }
    }
}

void extern_hd(__attribute__((unused)) int sig, siginfo_t* info, __attribute__((unused)) void* context){
    // SIGUSR2
    pid_t pid = info->si_pid;
    num_t num = search_process(pid);

    if(num == -1){
        log("warning: SIGUSR2 received from unregistered process with pid %d\n", pid);
        return;
    }

    process_t process = processes[num];

    switch(info->si_int){
        case MSG_ASK:
            log("info: SIGUSR2 [MSG_ASK] received from %d (pid = %d, is_mute = %u)\n",
                num, pid, process.is_mute);

            if(!process.is_mute){
                send_message(num, MSG_STAT);
            }

            break;

        case MSG_END:
            log("info: SIGUSR2 [MSG_END] received from %d (pid = %d, is_mute = %u)\n",
                num, pid, process.is_mute);

            break;

        default:
            log("warning: unknown SIGUSR2 message %d received from %d (pid = %d, is_mute = %u)\n",
                info->si_int, num, pid, process.is_mute);
    }
}

void atexit_hd(){
    remove_processes();
    log("info: parent process has been stopped...\n");
}

void ptimer_hd(__attribute__((unused)) int sig){
    log("info: unmute processes after 'p' command...\n");
    unmute_processes();
}

int is_num_in_stdin(){
    int current = getc(stdin);
    int isdigit = isdigit(current);

    ungetc(current, stdin);
    return isdigit;
}

num_t read_num_t(){
    num_t num;
    return scanf("%d", &num) ? num : -1;
}

int main(int argc, char** argv){
    if(argc != 2){
        log("error: one argument expected but %d provided", argc - 1);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];

    if(access(path, X_OK)){
        log("error: executable file %s doesn't exists\n", path);
        exit(EXIT_FAILURE);
    }

    setup_state();

    while(1){
        int option = getchar();

        switch(option){
            case '+':
                create_process(path);
                break;

            case '-':
                pop_process();
                print_state();
                break;

            case 'l':
                print_state();
                break;

            case 'k':
                remove_processes();
                break;

            case 's':
                if(is_num_in_stdin()){
                    num_t n = read_num_t();
                    mute_process(n);
                }
                else {
                    mute_processes();
                }

                break;

            case 'g':
                if(is_num_in_stdin()){
                    num_t n = read_num_t();
                    unmute_process(n);
                }
                else {
                    unmute_processes();
                }

                break;

            case 'p':
                if(is_num_in_stdin()){
                    num_t n = read_num_t();

                    mute_processes();
                    send_message(n, MSG_STAT);
                    alarm(TIMER_WAIT_SEC);
                }
                else {
                    printf("missing <num> after 'p' command\n");
                }

                break;

            case 'q':
                exit(EXIT_SUCCESS);

            default:
                if(!(option == 10 || option == -1)){
                    printf("unknown command %c (%d)\n", option, option);
                    break;
                }
        }
    }
}