// Linux x86-64
// GCC 12.2.1 20230201

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

void setup_com(int port){
    struct termios tty;

    if(tcgetattr(port, &tty)){
        perror("tcgetattr");
        exit(errno);
    }

    tty.c_cflag &= ~PARENB; 
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN]  = 0;

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    if(tcsetattr(port, TCSANOW, &tty)){
        perror("tcsetattr");
        exit(errno);
    }
}

int main(int argc, char** argv){
    //
    if(argc != 3){
        printf("missed command arguments\n"
               "usage: ./main <path> <path>\n");
        
        exit(EXIT_FAILURE);
    }

    int com1;
    int com2;

    if((com1 = open(argv[1], O_WRONLY)) < 0){
        fprintf(stderr, "error open COM1 (WR): %s", strerror(errno));
        exit(errno);
    }

    if((com2 = open(argv[2], O_RDONLY)) < 0){
        fprintf(stderr, "error open COM2 (RD): %s", strerror(errno));
        exit(errno);
    }
    
    const char* const messages[4] = {
        "Snow on the ground.",
        "Snow on the tree.",
        "Snow on the house.",
        "Snow on me!"
    };

    for(int i = 0; i < 4; i++){
        //
        char received[256] = {0};
        const char* sent = messages[i];

        int size = strlen(sent);
        int count_sent = write(com1, sent, size);
        int count_received = read(com2, received, size);

        printf("sent = %d, received = %d\n"
               "sent: %s\nreceived: %s\n\n", 
               count_sent, count_received, sent, received);
    }

    exit(EXIT_SUCCESS);
}