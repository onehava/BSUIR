#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ATTEMPT_MAX 3

static int is_run;

void interrupt(*oldint09)(...);

void interrupt newint09(...)
{
    unsigned char scancode = inp(0x60);
    
    if(scancode != 0xFA){
        printf("0x%x\n", scancode);
    }

    if(scancode == 0x01){
        is_run = 0;
    }

    (*oldint09)();
}

int write(unsigned char data)
{
    int attempt = 0;
    
    while((inp(0x64) & 2)){}
    
    do {
        attempt++;
        outp(0x60, data);
    }
    while((inp(0x60) == 0xFE) && (attempt < 3));
    
    if(attempt == ATTEMPT_MAX){
        return 1;
    }
    
    return 0;
}

int main()
{
    unsigned char state;
    
    oldint09 = getvect(0x09);
    setvect(0x09, newint09);

    is_run = 1;

    while(is_run)
    {
        if(write(0xED) || write(random(5))){
            break;
        }
        
        delay(1000);
    }
    
    setvect(0x09, oldint09);
    
    return 0;
}
