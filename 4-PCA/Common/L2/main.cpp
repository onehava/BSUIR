// Individual task 10: base vector A8h / 08h

#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Print video mode

struct word {
    unsigned char symbol;
    unsigned char attribute;
};

void print();

void interrupt(*oldint08)(...);	// IRQ0
void interrupt(*oldint09)(...); // IRQ1
void interrupt(*oldint10)(...); // IRQ2
void interrupt(*oldint11)(...); // IRQ3
void interrupt(*oldint12)(...); // IRQ4
void interrupt(*oldint13)(...); // IRQ5
void interrupt(*oldint14)(...); // IRQ6
void interrupt(*oldint15)(...); // IRQ7

void interrupt(*oldint70)(...); // IRQ8
void interrupt(*oldint71)(...); // IRQ9
void interrupt(*oldint72)(...); // IRQ10
void interrupt(*oldint73)(...); // IRQ11
void interrupt(*oldint74)(...); // IRQ12
void interrupt(*oldint75)(...); // IRQ13
void interrupt(*oldint76)(...); // IRQ14
void interrupt(*oldint77)(...); // IRQ15

void interrupt newint08(...) { print(); oldint08(); }
void interrupt newint09(...) { print(); oldint09(); }
void interrupt newint10(...) { print(); oldint10(); }
void interrupt newint11(...) { print(); oldint11(); }
void interrupt newint12(...) { print(); oldint12(); }
void interrupt newint13(...) { print(); oldint13(); }
void interrupt newint14(...) { print(); oldint14(); }
void interrupt newint15(...) { print(); oldint15(); }

void interrupt newint70(...) { print(); oldint70(); }
void interrupt newint71(...) { print(); oldint71(); }
void interrupt newint72(...) { print(); oldint72(); }
void interrupt newint73(...) { print(); oldint73(); }
void interrupt newint74(...) { print(); oldint74(); }
void interrupt newint75(...) { print(); oldint75(); }
void interrupt newint76(...) { print(); oldint76(); }
void interrupt newint77(...) { print(); oldint77(); }


void print_register(word far* position, int regv)
{
    char temp;
    int i;
    
    for(i = 0; i < 8; i++)
    {
        temp = regv % 2;
        regv >>= 1;
        position->symbol = temp + '0';
        position->attribute = 0x89;
        position++;
    }
}

void print()
{
    int regv;
    word far* screen = (word far*) MK_FP(0xB800, 0);
    
    // ...
    
    regv = inp(0x21);
    print_register(screen, regv);
    screen += 9;
    
    regv = inp(0xA1);
    print_register(screen, regv);
    screen += 71;
    
    // ...
    
    outp(0x20, 0x0A); // 0b000'01'0'10
    regv = inp(0x20);
    print_register(screen, regv);
    screen += 9;
    
    outp(0xA0, 0x0A); // 0b000'01'0'10
    regv = inp(0xA0);
    print_register(screen, regv);
    screen += 71;
    
    // ...
    
    outp(0x20, 0x0B); // 0b000'01'0'11
    regv = inp(0x20);
    print_register(screen, regv);
    screen += 9;
    
    outp(0xA0, 0x0B); // 0b000'01'0'11
    regv = inp(0xA0);
    print_register(screen, regv);
}

void init()
{
    oldint08 = getvect(0x08);
    oldint09 = getvect(0x09);
    oldint10 = getvect(0x0A);
    oldint11 = getvect(0x0B);
    oldint12 = getvect(0x0C);
    oldint13 = getvect(0x0D);
    oldint14 = getvect(0x0E);
    oldint15 = getvect(0x0F);

    oldint70 = getvect(0x70);
    oldint71 = getvect(0x71);
    oldint72 = getvect(0x72);
    oldint73 = getvect(0x73);
    oldint74 = getvect(0x74);
    oldint75 = getvect(0x75);
    oldint76 = getvect(0x76);
    oldint77 = getvect(0x77);

    setvect(0xA8, newint08);
    setvect(0xA9, newint09);
    setvect(0xAA, newint10);
    setvect(0xAB, newint11);
    setvect(0xAC, newint12);
    setvect(0xAD, newint13);
    setvect(0xAE, newint14);
    setvect(0xAF, newint15);

    setvect(0x08, newint70);
    setvect(0x09, newint71);
    setvect(0x0A, newint72);
    setvect(0x0B, newint73);
    setvect(0x0C, newint74);
    setvect(0x0D, newint75);
    setvect(0x0E, newint76);
    setvect(0x0F, newint77);

    _disable();         // CLI

    outp(0x20, 0x11);   // OCW1
    outp(0x21, 0xA8);   // OCW2
    outp(0x21, 0x04);   // OCW3
    outp(0x21, 0x01);   // OCW4

    outp(0xA0, 0x11);   // OWC1
    outp(0xA1, 0x08);   // OWC2
    outp(0xA1, 0x02);   // OWC3
    outp(0xA1, 0x01);   // OWC4

    _enable();          // STI
}

int main()
{
    unsigned far* fp;

    init();

    system("cls");
    printf("                   - IMR\n");
    printf("                   - IRR\n");
    printf("                   - ISR\n");
    printf("   MC        SC         \n");

    FP_SEG(fp) = _psp;
    FP_OFF(fp) = 0x2c;
    _dos_freemem(*fp);
    _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

    return 0;
}

