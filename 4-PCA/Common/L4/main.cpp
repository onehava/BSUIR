#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Representation marco

#define to_decimal(bcd) ((bcd / 16 * 10) + (bcd % 16))
#define to_bcd(decimal) ((decimal / 10 * 16) + (decimal % 10))

// Data structures

typedef unsigned char byte;

struct datetime {
    byte wday, mday, month, year;
    byte hours, mins, secs;
};

// Constants 

const unsigned int datetime_registers[] = {
    0x06, 0x07, 0x08, 0x09, // wday, mday, month, year
    0x04, 0x02, 0x00,       // secs, mins, hours
};

const char* const month[] = {
    "January" , "February" , "March"     ,     
    "April"   , "May"      , "June"      ,      
    "July"    , "August"   , "September" , 
    "October" , "November" , "December"  ,
};

const char* const week[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
};

// Global var state

unsigned int time_running;
unsigned int time_repeats;
unsigned int exec_repeats;

// Interrupt handlers

void interrupt(*old_time_int)(...);
void interrupt new_time_int(...);   // implemented under set_delay()

// Datetime interaction functions

void set_datetime(struct datetime* dt)
{
    byte* output = (byte*) dt;
    int i;
    byte status;
    
    _disable();
    
    do {
        outp(0x70, 0xA);
        status = inp(0x71) & 0x80;
    }
    while(status); // wait "updating" status disabled
    
    outp(0x70, 0xB);
    outp(0x71, inp(0x71) | 0x80); // disable update
    
    for(i = 0; i < 7; i++){
        outp(0x70, datetime_registers[i]);
        outp(0x71, to_bcd(output[i]));
    }
    
    outp(0x70, 0xB);
    outp(0x71, inp(0x71) & 0x7F); // enable update
    
    _enable();
}

void get_datetime(struct datetime* dt)
{
    int i;
    byte* input = (byte*) dt;
    
    for(i = 0; i < 7; i++){
        outp(0x70, datetime_registers[i]);
        input[i] = inp(0x71);
        input[i] = to_decimal(input[i]);
    }
}

// I/O datetime functions

void print_datetime(struct datetime* dt)
{
    printf("%s, %s %02u, 20%02u %02u:%02u:%02u\n", 
           week[dt->wday],
           month[dt->month],
           dt->mday,
           dt->year,
           dt->hours, dt->mins, dt->secs);
}

// todo: improve it (and test it)
void input_datetime(struct datetime* dt)
{
    printf("Input date <wday, month, mday, year>: ");
    scanf("%u%u%u%u", &dt->wday, &dt->month, &dt->mday, &dt->year);

    printf("Input time <hours, mins, secs>: ");
    scanf("%u%u%u", &dt->hours, &dt->mins, &dt->secs);
}

// Clock control functions

void set_delay(unsigned int ms)
{
    _disable();
    
    time_running = 1;
    time_repeats = 0;
    exec_repeats = ms;

    old_time_int = getvect(0x70);
    setvect(0x70, new_time_int);

    outp(0xA1, inp(0xA0) & 0xFE); // unmask request signal line
    outp(0x70, 0xB);
    outp(0x71, inp(0x71) | 0x40); // period interrupt

    _enable();
}

void reset_delay()
{
    _disable();

    time_running = 0;
    
    setvect(0x70, old_time_int);
    
    outp(0x70, 0xB);
    outp(0x71, inp(0x71) & 0xBF); // period interrupt disabe

    _enable();
}

void interrupt new_time_int(...)
{
    time_repeats++;
    
    outp(0x70, 0x0C); // select memory in CMOS
    inp(0x71);
    
    if(time_repeats == exec_repeats)
    {
        reset_delay();
        puts("Delay has been expired\n");
    }

    outp(0x20, 0x20); // send End Of Interrupt
    outp(0xA0, 0x20);
}

// Entry point function

int main()
{
    unsigned int ms;
    struct datetime dt;

    printf("Menu:\n\t1 - print datetime\n\t2 - input datetime\n\t3 - start delay\n\t4 - exit program\n\n");

    for(;;)
    {
        switch(getch())
        {
            case '1':
                get_datetime(&dt);
                print_datetime(&dt);
                break;

            case '2':
                input_datetime(&dt);
                set_datetime(&dt);
                break;

            case '3':
                if(time_running){
                    printf("Error: timer in use\n");
                }
                else {
                    printf("Enter ms: ");
                    fflush(stdin);
                    scanf("%d", &ms);
                    set_delay(ms);
                }

                break;
                
            case '4':
                if(time_running){
                    printf("Exit: reset running delay\n");
                    reset_delay();
                }
                return 0;
        }
    }
}