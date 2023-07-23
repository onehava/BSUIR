#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

struct sound
{
    unsigned int frequency;
    unsigned int delay;
};

struct sound sounds[9];

void set_sounds()
{
    sounds[0].frequency = 349; sounds[0].delay = 600;
    sounds[1].frequency = 392; sounds[1].delay = 300;
    sounds[2].frequency = 440; sounds[0].delay = 600;
    sounds[3].frequency = 349; sounds[3].delay = 300;
    sounds[4].frequency = 440; sounds[4].delay = 300;
    sounds[5].frequency = 440; sounds[5].delay = 300;
    sounds[6].frequency = 392; sounds[6].delay = 300;
    sounds[7].frequency = 349; sounds[7].delay = 300;
    sounds[8].frequency = 392; sounds[8].delay = 300;
}

void to_bin(unsigned char state, char* bin)
{
    int i;
    char tmp;

    for(i = 7; i >= 0; i--) 
    {
        tmp = state % 2;
        state /= 2;
        bin[i] = tmp + '0';
    }
    bin[8] = '\0';
}

void print_state()
{
    unsigned char state;
    char bin[9];
    
    outp(0x43, 0xE2); // 11 1 0 001 0
                      //    | |   | channel 0 
                      //    | | fix counter state
                      //    | do not fix the counter value 
    state = inp(0x40);
    to_bin(state, bin);
    printf("channel 0 state: %s\n", bin);
    
    outp(0x43, 0xE4); // 11 1 0 010 0
                      //    | |  | channel 1
                      //    | | fix counter state
                      //    | do not fix the counter value 
    state = inp(0x41);
    to_bin(state, bin);
    printf("channel 1 state: %s\n", bin);
    
    outp(0x43, 0xE8); // 11 1 0 100 0
                      //    | | | channel 2
                      //    | | fix counter state
                      //    | do not fix the counter value 
    state = inp(0x42);
    to_bin(state, bin);
    printf("channel 2 state: %s\n", bin);
}

void set_frequency(int input)
{
    long base = 1193180;
    long freq;
    
    outp(0x43, 0xB6); // 10 11 011 0
                      // |  |   |  | binary count
                      // |  |   | 3rd operating mode
                      // |  | recording the counter value (from lowest)
                      // | selecting the 2nd channel
    freq = base / input;
    outp(0x42, freq % 256);
    
    freq /= 256;
    outp(0x42, freq);
}

void play_sound()
{
    for(int i = 0; i < 9; i++)
    {
        outp(0x61, inp(0x61) | 0x03); //   turn on speaker
		
        set_frequency(sounds[i].frequency); // set setup for 2nd channel of timer
        delay(sounds[i].delay);
		
        outp(0x61, inp(0x61) & 0xFC); // turn off speaker
    }
}

int main()
{
    set_sounds();
    int op;
    
    while(1)
    {
        printf("\nChoose operation:\n"
               "1 - play sound\n"
               "2 - print state\n"
               "3 - exit\n\n> ");
		
        while(!scanf("%d", &op)){}
        
        switch(op)
        {
            case 1:
                play_sound();
                break;
                
            case 2:
                print_state();
                break;

            case 3:
                return 0;
            
            default:
                printf("Error: unknown operation\n");
                break;
        }
    }
}
