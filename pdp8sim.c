
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char data[4096*2*2];

#define DATAMASK 4095
#define MEMMASK 4095
unsigned short mem[MEMMASK+1];
unsigned char memvalid[MEMMASK+1];

unsigned int ac;
unsigned int pc;

unsigned long read_ac_count;
unsigned long write_ac_count;
unsigned long read_memory_count;
unsigned long write_memory_count;
unsigned long fetch_count;

//-------------------------------------------------------------------
unsigned int read_ac ( void )
{
    read_ac_count++;
    return(ac);
}
//-------------------------------------------------------------------
void write_ac ( unsigned int  data, unsigned int nol )
{
    write_ac_count++;
    if(nol)
    {
        ac=(data&0xFFF)|(ac&0x1000);
    }
    else
    {
        ac=data&0x1FFF;
    }
}
//-------------------------------------------------------------------
void write_memory ( unsigned int addr, unsigned int data )
{
    write_memory_count++;
    addr&=MEMMASK;
printf("writemem(0%04o,0%04o) (0x%03X)(%u)\n",addr,data,data,data);
    mem[addr]=data&DATAMASK;
    return(data);
}
//-------------------------------------------------------------------
unsigned int fetch ( unsigned int addr )
{
    unsigned int data;

    fetch_count++;
    addr&=MEMMASK;
    if(memvalid[addr]==0)
    {
        printf("fetching a non-instruction 0%04o\n",addr);
    }
    data=mem[addr]&DATAMASK;
printf("fetch(0%04o)==0%04o (0x%03X)(%u)\n",addr,data,data,data);
    if((addr&(~7))==8)
    {
        write_memory(addr,data+1);
    }
    return(data);
}
//-------------------------------------------------------------------
unsigned int read_memory ( unsigned int addr )
{
    unsigned int data;

    read_memory_count++;
    addr&=MEMMASK;
    data=mem[addr]&DATAMASK;
printf("readmem(0%04o)==0%04o (0x%03X)(%u)\n",addr,data,data,data);
    if((addr&(~7))==8)
    {
        write_memory(addr,data+1);
    }
    return(data);
}
//-------------------------------------------------------------------
int reset_pdp8 ( void )
{
    ac = 0123;
    pc = 0200;
    read_ac_count = 0;
    write_ac_count = 0;
    read_memory_count = 0;
    write_memory_count = 0;
    fetch_count = 0;

    return(0);
}
//-------------------------------------------------------------------
int execute_one ( void )
{
    unsigned int inst;

    inst=fetch(pc); pc=(pc+1)&MEMMASK;
    switch((inst>>9)&7)
    {
        case 0:
        {
printf("[0%04o] 0%04o : TAD\n",addr,inst);



            break;
        }
        default:
        {
printf("[0%04o] 0%04o : UNKNOWN\n",addr,inst);
            return(1);
            //break;
        }
    }

            //x=mem[addr];
            //printf("[0%04o] 0%04o : ",addr,x);
            ////0123456789-1
            ////1-9876543210
            //ir=(x>>9)&7;
            //switch(ir)
            {
                //case 0:
                //{
                    //printf("AND "); show_mri(addr,x);
                    //break;
                //}
                //case 1:
                //{
                    //printf("TAD "); show_mri(addr,x);
                    //break;
                //}
                //case 2:
                //{
                    //printf("ISZ "); show_mri(addr,x);
                    //break;
                //}
                //case 3:
                //{
                    //printf("DCA "); show_mri(addr,x);
                    //break;
                //}
                //case 4:
                //{
                    //printf("JMS "); show_mri(addr,x);
                    //break;
                //}
                //case 5:
                //{
                    //printf("JMP "); show_mri(addr,x);
                    //break;
                //}
                //case 6:
                //{
                    //printf("UNKNOWN ");
                    //break;
                //}
                //case 7:
                //{
                    //if(x&(1<<8))
                    //{
                        //if((x&9)==0)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("SMA ");
                            //if(x&(1<<5)) printf("SZA ");
                            //if(x&(1<<4)) printf("SNL ");
                            //if(x&(1<<2)) printf("OSR ");
                            //if(x&(1<<1)) printf("HLT ");
                        //}
                        //else
                        //if((x&9)==8)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("SPA ");
                            //if(x&(1<<5)) printf("SNA ");
                            //if(x&(1<<4)) printf("SZL ");
                            //if(x&(1<<2)) printf("OSR ");
                            //if(x&(1<<1)) printf("HLT ");
                        //}
                        //else
                        //if(x&1)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("MQA ");
                            //if(x&(1<<5)) printf("SCA ");
                            //if(x&(1<<4)) printf("MQL ");
                            //if(((x>>1)&7)==0x00) printf("no operation ");
                            //if(((x>>1)&7)==0x01) printf("SCL ");
                            //if(((x>>1)&7)==0x02) printf("MUY ");
                            //if(((x>>1)&7)==0x03) printf("DVI ");
                            //if(((x>>1)&7)==0x04) printf("NMI ");
                            //if(((x>>1)&7)==0x05) printf("SHL ");
                            //if(((x>>1)&7)==0x06) printf("ASR ");
                            //if(((x>>1)&7)==0x07) printf("LSR ");
                        //}
                        //else
                        //{
                            //printf("UNKNOWN ");
                        //}
                    //}
                    //else
                    //{
                        //if(x&(1<<7)) printf("CLA ");
                        //if(x&(1<<6)) printf("CLL ");
                        //if(x&(1<<5)) printf("CMA ");
                        //if(x&(1<<4)) printf("CML ");
                        //if(x&(1<<3)) printf("RAR ");
                        //if(x&(1<<2)) printf("RAL ");
                        //if(x&(1<<1)) printf("BSW ");
                        //if(x&(1<<0)) printf("IAC ");
                    //}
                    //break;
                //}
            //}


            //printf("\n");

        }

}
//-------------------------------------------------------------------
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






    reset_pdp8();
    ra=0;
    while(1)
    {
        if(++ra>20) break;
        if(execute_one()) break;
    }




    //for(addr=0;addr<=MEMMASK;addr++)
    //{
        //if(memvalid[addr])
        //{
            //x=mem[addr];
            //printf("[0%04o] 0%04o : ",addr,x);
            ////0123456789-1
            ////1-9876543210
            //ir=(x>>9)&7;
            //switch(ir)
            //{
                //case 0:
                //{
                    //printf("AND "); show_mri(addr,x);
                    //break;
                //}
                //case 1:
                //{
                    //printf("TAD "); show_mri(addr,x);
                    //break;
                //}
                //case 2:
                //{
                    //printf("ISZ "); show_mri(addr,x);
                    //break;
                //}
                //case 3:
                //{
                    //printf("DCA "); show_mri(addr,x);
                    //break;
                //}
                //case 4:
                //{
                    //printf("JMS "); show_mri(addr,x);
                    //break;
                //}
                //case 5:
                //{
                    //printf("JMP "); show_mri(addr,x);
                    //break;
                //}
                //case 6:
                //{
                    //printf("UNKNOWN ");
                    //break;
                //}
                //case 7:
                //{
                    //if(x&(1<<8))
                    //{
                        //if((x&9)==0)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("SMA ");
                            //if(x&(1<<5)) printf("SZA ");
                            //if(x&(1<<4)) printf("SNL ");
                            //if(x&(1<<2)) printf("OSR ");
                            //if(x&(1<<1)) printf("HLT ");
                        //}
                        //else
                        //if((x&9)==8)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("SPA ");
                            //if(x&(1<<5)) printf("SNA ");
                            //if(x&(1<<4)) printf("SZL ");
                            //if(x&(1<<2)) printf("OSR ");
                            //if(x&(1<<1)) printf("HLT ");
                        //}
                        //else
                        //if(x&1)
                        //{
                            //if(x&(1<<7)) printf("CLA ");
                            //if(x&(1<<6)) printf("MQA ");
                            //if(x&(1<<5)) printf("SCA ");
                            //if(x&(1<<4)) printf("MQL ");
                            //if(((x>>1)&7)==0x00) printf("no operation ");
                            //if(((x>>1)&7)==0x01) printf("SCL ");
                            //if(((x>>1)&7)==0x02) printf("MUY ");
                            //if(((x>>1)&7)==0x03) printf("DVI ");
                            //if(((x>>1)&7)==0x04) printf("NMI ");
                            //if(((x>>1)&7)==0x05) printf("SHL ");
                            //if(((x>>1)&7)==0x06) printf("ASR ");
                            //if(((x>>1)&7)==0x07) printf("LSR ");
                        //}
                        //else
                        //{
                            //printf("UNKNOWN ");
                        //}
                    //}
                    //else
                    //{
                        //if(x&(1<<7)) printf("CLA ");
                        //if(x&(1<<6)) printf("CLL ");
                        //if(x&(1<<5)) printf("CMA ");
                        //if(x&(1<<4)) printf("CML ");
                        //if(x&(1<<3)) printf("RAR ");
                        //if(x&(1<<2)) printf("RAL ");
                        //if(x&(1<<1)) printf("BSW ");
                        //if(x&(1<<0)) printf("IAC ");
                    //}
                    //break;
                //}
            //}


            //printf("\n");

        //}
    //}

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
