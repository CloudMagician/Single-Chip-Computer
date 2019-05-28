#include<reg52.h>
#include <stdio.h>
#include<intrins.h>
#define LcdData P2


//COM
sbit KEY1	=	P3^6;
sbit DQ		=	P1^4;
sbit RQ		=	P1^1;

bit bt;

int objTemp=0,currentTemp=0;
int FD=0,SD=0,Next=0,Max=256,SUM=0;
unsigned char status=0x01,tempNum=0x00;//?

//LCD
sbit RST 	=	P1^5;
sbit CS1 	=	P1^7;
sbit CS2 	=	P1^6;
sbit E		=	P3^3;
sbit RW 	=	P3^4;
sbit RS 	=	P3^5;
sbit BUSY = P2^7;
void InitLCD(void);
void IsBusy(void);
void LcdCommandWrite(unsigned char value);
void LcdDataWrite(unsigned char value);
void ShowOn(void);
void ShowOff(void);
void SetRow(unsigned char row);
void SetCol(unsigned char col);
void SetPage(unsigned char page);
void ScreenChoose(unsigned int screen);
void Clear(unsigned int screen);
void DisplayWord(unsigned int screen,unsigned char page,unsigned char col,unsigned char *word);
void DisplayChar(unsigned int screen,unsigned char row,unsigned char col);      
void Display();        
void Delay(unsigned int cnt);               
unsigned char code mu[]={0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,0x00,0x00,0x00,0xFF,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0xFF,0x00,0x00,0x00};
unsigned char code biao[]={0x10,0x10,0xD0,0xFF,0x90,0x10,0x20,0x22,0x22,0x22,0xE2,0x22,0x22,0x22,0x20,0x00,0x04,0x03,0x00,0xFF,0x00,0x13,0x0C,0x03,0x40,0x80,0x7F,0x00,0x01,0x06,0x18,0x00};
unsigned char code dang[]={0x00,0x40,0x42,0x44,0x58,0x40,0x40,0x7F,0x40,0x40,0x50,0x48,0xC6,0x00,0x00,0x00,0x00,0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00};
unsigned char code qian[]={0x08,0x08,0xE8,0x29,0x2E,0x28,0xE8,0x08,0x08,0xC8,0x0C,0x0B,0xE8,0x08,0x08,0x00,0x00,0x00,0xFF,0x09,0x49,0x89,0x7F,0x00,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,0x00};
unsigned char code ling[]={0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code yi[]={0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code er[]={0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code san[]={0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code si[]={0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code wu[]={0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code liu[]={0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code qi[]={0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code ba[]={0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char code jiu[]={0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//COM
void wait(unsigned int count){
	while(count--){
		_nop_();
		_nop_();
	}
}
bit initDS(void)
{
  DQ = 0;
  wait(480);
  DQ = 1;
  wait(60);
	bt=DQ;
	wait(420);
  return bt;
}
unsigned char readByte(void)
{
  int i;
	int temp=0;
  for (i=8;i>0;i--)
  {
		temp>>=1;
    DQ = 0;
		wait(1);
    DQ = 1;
		wait(1);
    if(DQ){
    	temp|=0x80;
		}
		wait(60);
  }
  return temp;
}
void writeByte(unsigned char dat)
{
  int i;
  for (i=8; i>0; i--)
  {
    DQ = 0;
		wait(8);
		dat>>=1;
		DQ=CY;
		wait(60);
    DQ = 1;
    wait(10);
  }
}
int readTemperature(void)
{
  int x,a,b;
  initDS();
	while(!DQ);
  writeByte(0xcc);
  writeByte(0x44);
	while(!DQ);
  initDS();
	while(!DQ);
  writeByte(0xcc);
  writeByte(0xbe);
  a=readByte();
  b=readByte();
	P0=a;
	wait(255);
	x=b;
	x<<=8;
	x|=a;
	return x*0.0625;
}
void waitL(int count){
	int k=0xffff;
	while(count--){
		while(k--);
	}
}
int getStep(int diff1,int diff2){
	return (diff2+(diff1+diff2)/2+(diff2-diff1))/2;
}
void Change() {
    SUM+=Next;
    if(SUM>Max){
      RQ=1;
      SUM-=Max;
    }else{
      RQ=0;
    }
    if(!KEY1){
      objTemp=70;
    }else{
      objTemp=00;
    }
    currentTemp=readTemperature();
    if(currentTemp!=objTemp){
      if(FD==0){
        FD=objTemp-currentTemp;
      }else{
        SD=objTemp-currentTemp;
        Next += (SD+(FD+SD)/2+(SD-FD))/2;
        if(Next<0){
          Next=0;
        }
        if(Next>Max+1){
          Next=Max;
        }
        FD=0;
      }
    }
}

//LCD
void InitLCD(void)
{ 
    IsBusy();
    RST=1;
    RST=0;                       
    Delay(100);
    RST=1;
    ShowOff();
    Delay(100);
    ShowOn();
    Clear(0);
    SetPage(0);
    SetRow(0);                    
    SetCol(0);    
}
void IsBusy(void)                     
{
    RS=0;
    RW=1;
    E=1;
    while(BUSY);
    E=0;                                   
}
void LcdCommandWrite(unsigned char value)
{    
    IsBusy();
    RS=0;
    RW=0;
    LcdData=value;
    E=1;
    Delay(200);
    E=0;
}
void LcdDataWrite(unsigned char value)       
{
    IsBusy();
    RS=1;
    RW=0;
    LcdData=value;
    E=1;
    Delay(200);
    E=0;
}
void ShowOn(void)                     
{
    LcdCommandWrite(0x3f);    
}
void ShowOff(void)                       
{
    LcdCommandWrite(0x3e);    
}
void SetRow(unsigned char row)                   
{
    row=row&0x3f;		//0-63
    LcdCommandWrite(0xc0+row);
}
void SetCol(unsigned char col) 
{
    col=col&0x3f;		//0-63
    LcdCommandWrite(0x40+col);
}
void SetPage(unsigned char page)
{
    page=page&0x07;	//0-7
    LcdCommandWrite(0xb8+page);
}
void Clear(unsigned int screen){
    unsigned int i,j;
    ScreenChoose(screen);
    for(i=0;i<8;i++){
        SetPage(i);
        SetCol(0x00);
        for(j=0;j<64;j++){
            LcdDataWrite(0x00);
        }
    }
}
void ScreenChoose(unsigned int screen){
    switch(screen)
    {
        case 0: CS1=1;CS2=1;break;  
        case 1: CS1=1;CS2=0;break;  
        case 2: CS1=0;CS2=1;break;   
        default: CS1=1;CS2=1;break;  
    }
}
void DisplayWord(unsigned int screen,unsigned char page,unsigned char col,unsigned char *word)
{
    unsigned int i=0;
    ScreenChoose(screen);
    SetPage(page);                         
    SetCol(col);
    for(i=0;i<16;i++){                     
        LcdDataWrite(word[i]);
    }
    SetPage(page+1);
    SetCol(col);
    for(i=0;i<16;i++){
        LcdDataWrite(word[i+16]);
    }
}
void Display(){

    unsigned char a,b,c,d,e,f;
    DisplayWord(1,1,1*16,mu);
    DisplayWord(1,1,2*16,biao);
    a = objTemp/100%10;
    b = objTemp%100/10;
    c = objTemp%10;
    switch(a){
        case 0: DisplayWord(1,4,1*16,ling);break;
        case 1: DisplayWord(1,4,1*16,yi);break;
        case 2: DisplayWord(1,4,1*16,er);break;
        case 3: DisplayWord(1,4,1*16,san);break;
        case 4: DisplayWord(1,4,1*16,si);break;
        case 5: DisplayWord(1,4,1*16,wu);break;
        case 6: DisplayWord(1,4,1*16,liu);break;
        case 7: DisplayWord(1,4,1*16,qi);break;
        case 8: DisplayWord(1,4,1*16,ba);break;
        case 9: DisplayWord(1,4,1*16,jiu);break;
    }
    switch(b){
        case 0: DisplayWord(1,4,2*16,ling);break;
        case 1: DisplayWord(1,4,2*16,yi);break;
        case 2: DisplayWord(1,4,2*16,er);break;
        case 3: DisplayWord(1,4,2*16,san);break;
        case 4: DisplayWord(1,4,2*16,si);break;
        case 5: DisplayWord(1,4,2*16,wu);break;
        case 6: DisplayWord(1,4,2*16,liu);break;
        case 7: DisplayWord(1,4,2*16,qi);break;
        case 8: DisplayWord(1,4,2*16,ba);break;
        case 9: DisplayWord(1,4,2*16,jiu);break;
    }
    switch(c){
        case 0: DisplayWord(1,4,3*16,ling);break;
        case 1: DisplayWord(1,4,3*16,yi);break;
        case 2: DisplayWord(1,4,3*16,er);break;
        case 3: DisplayWord(1,4,3*16,san);break;
        case 4: DisplayWord(1,4,3*16,si);break;
        case 5: DisplayWord(1,4,3*16,wu);break;
        case 6: DisplayWord(1,4,3*16,liu);break;
        case 7: DisplayWord(1,4,3*16,qi);break;
        case 8: DisplayWord(1,4,3*16,ba);break;
        case 9: DisplayWord(1,4,3*16,jiu);break;
    }
    DisplayWord(2,1,1*16,dang);
    DisplayWord(2,1,2*16,qian);
    d = currentTemp/100%10;
    e = currentTemp%100/10;
    f = currentTemp%10;
    switch(d){
        case 0: DisplayWord(2,4,1*16,ling);break;
        case 1: DisplayWord(2,4,1*16,yi);break;
        case 2: DisplayWord(2,4,1*16,er);break;
        case 3: DisplayWord(2,4,1*16,san);break;
        case 4: DisplayWord(2,4,1*16,si);break;
        case 5: DisplayWord(2,4,1*16,wu);break;
        case 6: DisplayWord(2,4,1*16,liu);break;
        case 7: DisplayWord(2,4,1*16,qi);break;
        case 8: DisplayWord(2,4,1*16,ba);break;
        case 9: DisplayWord(2,4,1*16,jiu);break;
    }
    switch(e){
        case 0: DisplayWord(2,4,2*16,ling);break;
        case 1: DisplayWord(2,4,2*16,yi);break;
        case 2: DisplayWord(2,4,2*16,er);break;
        case 3: DisplayWord(2,4,2*16,san);break;
        case 4: DisplayWord(2,4,2*16,si);break;
        case 5: DisplayWord(2,4,2*16,wu);break;
        case 6: DisplayWord(2,4,2*16,liu);break;
        case 7: DisplayWord(2,4,2*16,qi);break;
        case 8: DisplayWord(2,4,2*16,ba);break;
        case 9: DisplayWord(2,4,2*16,jiu);break;
    }
    switch(f){
        case 0: DisplayWord(2,4,3*16,ling);break;
        case 1: DisplayWord(2,4,3*16,yi);break;
        case 2: DisplayWord(2,4,3*16,er);break;
        case 3: DisplayWord(2,4,3*16,san);break;
        case 4: DisplayWord(2,4,3*16,si);break;
        case 5: DisplayWord(2,4,3*16,wu);break;
        case 6: DisplayWord(2,4,3*16,liu);break;
        case 7: DisplayWord(2,4,3*16,qi);break;
        case 8: DisplayWord(2,4,3*16,ba);break;
        case 9: DisplayWord(2,4,3*16,jiu);break;
    }
    Delay(5000);
}
void Delay(unsigned int cnt)
{
    while(--cnt);
}

void main()
{
    InitLCD();
		while(1){
      Change();
			Display();
			waitL(0x03);
		}
}