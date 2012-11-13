

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char data[4096*2*2];

#define MEMMASK 4095
unsigned short mem[MEMMASK+1];
unsigned char memvalid[MEMMASK+1];


            //0123456789-1
            //1-9876543210
void show_mri ( unsigned int addr, unsigned int x )
{
    unsigned int ma;

    ma=x&0x7F;
    if(x&(1<<7))
    {
        //current page
        ma|=addr&(~0x7F);
    }

    if(x&(1<<8))
    {
        //indirect
        printf("I ");
    }
    printf("0%04o",ma);
}

int main ( int argc, char *argv[] )
{
    FILE *fp;
    unsigned int ra,rb,rc;
    unsigned int addr;
    unsigned int ir;
    unsigned int x;


    if(argc<2)
    {
        printf(".bin file not specified\n");
        return(1);
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    printf("read %u bytes\n",rb);
    if(rb&1)
    {
        //printf("Should be an even number of bytes!\n");
        //return(1);
        rb++;
    }
    memset(memvalid,0,sizeof(memvalid));
    addr=0;
    for(ra=0;ra<rb;ra+=2)
    {
        rc=(((unsigned int)data[ra+0])<<6)|(data[ra+1]&0x3F);
        if(rc&0x1000)
        {
           addr=rc&MEMMASK;
           continue;
        }
        mem[addr]=rc&0xFFF;
//printf("[0%04o] = 0%04o\n",addr,mem[addr]);
        if((data[ra+0]&0x80)==0)
        if((data[ra+1]&0x80)==0)
            memvalid[addr]=1;
        addr=(addr+1)&MEMMASK;
    }

    for(addr=0;addr<=MEMMASK;addr++)
    {

        if(memvalid[addr])
        {
            x=mem[addr];
            printf("[0%04o] 0%04o : ",addr,x);
            //0123456789-1
            //1-9876543210
            ir=(x>>9)&7;
            switch(ir)
            {
                case 0:
                {
                    printf("AND "); show_mri(addr,x);
                    break;
                }
                case 1:
                {
                    printf("TAD "); show_mri(addr,x);
                    break;
                }
                case 2:
                {
                    printf("ISZ "); show_mri(addr,x);
                    break;
                }
                case 3:
                {
                    printf("DCA "); show_mri(addr,x);
                    break;
                }
                case 4:
                {
                    printf("JMS "); show_mri(addr,x);
                    break;
                }
                case 5:
                {
                    printf("JMP "); show_mri(addr,x);
                    break;
                }
                case 6:
                {
                    printf("UNKNOWN ");
                    break;
                }
                case 7:
                {
                    if(x&(1<<8))
                    {
                        if((x&9)==0)
                        {
                            if(x&(1<<7)) printf("CLA ");
                            if(x&(1<<6)) printf("SMA ");
                            if(x&(1<<5)) printf("SZA ");
                            if(x&(1<<4)) printf("SNL ");
                            if(x&(1<<2)) printf("OSR ");
                            if(x&(1<<1)) printf("HLT ");
                        }
                        else
                        if((x&9)==8)
                        {
                            if(x&(1<<7)) printf("CLA ");
                            if(x&(1<<6)) printf("SPA ");
                            if(x&(1<<5)) printf("SNA ");
                            if(x&(1<<4)) printf("SZL ");
                            if(x&(1<<2)) printf("OSR ");
                            if(x&(1<<1)) printf("HLT ");
                        }
                        else
                        if(x&1)
                        {
                            if(x&(1<<7)) printf("CLA ");
                            if(x&(1<<6)) printf("MQA ");
                            if(x&(1<<5)) printf("SCA ");
                            if(x&(1<<4)) printf("MQL ");
                            if(((x>>1)&7)==0x00) printf("no operation ");
                            if(((x>>1)&7)==0x01) printf("SCL ");
                            if(((x>>1)&7)==0x02) printf("MUY ");
                            if(((x>>1)&7)==0x03) printf("DVI ");
                            if(((x>>1)&7)==0x04) printf("NMI ");
                            if(((x>>1)&7)==0x05) printf("SHL ");
                            if(((x>>1)&7)==0x06) printf("ASR ");
                            if(((x>>1)&7)==0x07) printf("LSR ");
                        }
                        else
                        {
                            printf("UNKNOWN ");
                        }
                    }
                    else
                    {
                        if(x&(1<<7)) printf("CLA ");
                        if(x&(1<<6)) printf("CLL ");
                        if(x&(1<<5)) printf("CMA ");
                        if(x&(1<<4)) printf("CML ");
                        if(x&(1<<3)) printf("RAR ");
                        if(x&(1<<2)) printf("RAL ");
                        if(x&(1<<1)) printf("BSW ");
                        if(x&(1<<0)) printf("IAC ");
                    }
                    break;
                }
            }


            printf("\n");

        }
    }

    return(0);
}


/*
The .bin file appears to be each 8 bit byte in the file is a padded
6 bit byte(?).  Everything is two byte pairs, big endian style.  The
first byte in file contains the most significant 6 bits and second byte
is the least significant 6 bits.  These first two bytes are the first
12 bit item in the file.  If the first bit in the pair has the 0x40
bit set then the 12 bit item is an address, the non address values after
that word start at that address and continue from there.

00000000  80 80 80 80 80 80 80 80  80 80 80 80 80 80 80 80  |................|
*
000000f0  42 00 3b 00 0a 08 38 21  1a 0a 0a 09 12 0a 2a 04  |B.;...8!......*.|
00000100  3c 02 00 12 00 24 00 00  07 1d 80                 |<....$.....|
0000010b

offset 0xF0 in the file contains 0x42 then 0x00 then 0x3B and 0x00

combine bytes by shifting the first, even numbered, byte left 6 bits and
orring with odd byte

42 00 => 100 0010 00 0000 = 1000010000000 = 1 000 010 000 000 = 10200 octal
  that 1xxxx bit in octal or 0x1000 bit in hex means this is an address,
  strip off the lower 12 bits 0200 in octal or 0x080 in hex
3b 00 => 11 1011 00 0000 = 7300 octal this goes in memory at address 200 octal
increment your address pointer to 201 octal
0a 08 => 001010 001000 = 1210 octal goes in memory at 201 octal.
increment address pointer to 202
etc.
The rim file format is pretty much the same and your code to read the
.bin file should work for the .rim file as is.  The .rim file once it
gets into the real data will have an address word before every data word
address, data, address, data.  The bin file only has the address
when the next data word is not the prior data words address+1

It looks like the 0x80 bit is for fill bytes or something, not sure.  there is
an odd byte at the end of the file.


*/

