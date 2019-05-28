#include<reg52.h>
#include<intrins.h>
#define LcdData P2

#define TIMER 10
#define MAXSPEED 180
#define MIDSPEED 90
#define MINSPEED 50

//COM
sbit KEY1		=	P3^6;
sbit KEY2		=	P3^7;
sbit OUT		=	P1^1;
sfr P4			=	0xc0;
sfr P4SW		=	0xbb;
sbit DCLK		=	P4^4;
sbit LED		=	P4^5;
char code ledCode[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

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
int timer=TIMER,count=0,countS=0,currentSpeed=0,objSpeed=0;
int SUM=0,NextS=30,MaxS=256;
int GetStepLen(int num)
{
	int temp;
	if(num < 6){
		return 0;
	}
	if(num < 10){
		return 1;
	}
	temp = num/100;
	temp = (num-temp*100)/10;
	temp = temp/3*2+1;
	if((NextS-temp)<1){
		return 1;
	}
	return temp;
}
void ex_int0() interrupt 0
{
	count=count+1;
}
void t0_int0() interrupt 1
{
	TR0=0;
	
	timer=timer-1;
	if(timer==0){
		if(countS==0){
			countS=count;
			currentSpeed=count*2;
		}else{
			currentSpeed=count+countS;
			countS=0;
		}
		timer=TIMER;
		count=0;
		if(currentSpeed<objSpeed){
			NextS=NextS+GetStepLen(objSpeed-currentSpeed);
		}
		if(currentSpeed>objSpeed){
			NextS=NextS-GetStepLen(currentSpeed-objSpeed);
		}
	}
	
	TH0=0x3c;
	TL0=0xb0;
	TR0=1;
}
void t1_int0() interrupt 3
{
	TR1=0;
	
	SUM=SUM+NextS;
	if((!KEY1)&&(!KEY2)){
		OUT=0;
	}else{
		if(!KEY1){
			objSpeed=MAXSPEED;
		}
		if(!KEY2){
			objSpeed=MINSPEED;
		}
		if(KEY1 && KEY2){
			objSpeed=MIDSPEED;
		}
		if(SUM>MaxS){
			OUT=0;
			SUM=SUM-MaxS;
		}else{
			OUT=1;
		}
	}
	
	TH1=0xff;
	TL1=0x9c;
	TR1=1;
}	
void InitTimer(){
	P4SW=0x30;
	
	TMOD=0x11;
	TH0=0x3c;
	TL0=0xb0;
	TH1=0xff;
	TL1=0x9c;
	
	IT0=1;
	EA=1;
	ET0=1;
	ET1=1;
	EX0=1;
	
	TR0=1;
	TR1=1;
}	
void send2LED(char temp){
	int i;
	for(i=0;i<8;i++){
		DCLK=0;
		LED=temp & 0x80;
		DCLK=1;
		temp<<=1;
	}
}
	
void outLed(){
	int a,b,c;
	a=currentSpeed/100;
	b=(currentSpeed-a*100)/10;
	c=currentSpeed-a*100-b*10;
	send2LED(ledCode[c]);
	send2LED(ledCode[b]);
	send2LED(ledCode[a]);
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
    a = objSpeed/100%10;
    b = objSpeed%100/10;
    c = objSpeed%10;
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
    d = currentSpeed/100%10;
    e = currentSpeed%100/10;
    f = currentSpeed%10;
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
		outLed();
    Delay(5000);
}
void Delay(unsigned int cnt)
{
    while(--cnt);
}

void main()
{
    InitLCD();
		InitTimer();
    while(1) {
			Display();
    }
}