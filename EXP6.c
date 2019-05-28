#include<reg52.h>
#include<intrins.h>
#define LcdData P2

//AD
sfr PLASF = 		0X9D;   
sfr ADC_CONTR = 0XBC;      
sfr ADC_RES = 	0XBD;           
sfr ADC_RESL = 	0XBE;       
sfr AUXR1 = 		0XA2;
sfr IPH = 			0XB7;              
bit EADC = 			0XA8^5;           
#define ADC_POWER		0X80    
#define ADC_FLAG    0X10   
#define ADC_START  	0X08
#define ADC_SPEEDLL	0X00
#define ADC_SPEEDL 	0X20    
#define ADC_SPEEDH 	0X40
#define ADC_SPEEDHH	0X60
void InitAD(unsigned char n);
unsigned int ADC_GET(unsigned char n);
void DelayUs(unsigned int n);  

//LCD
sbit RST = 	P1^5;
sbit CS1 = 	P1^7;
sbit CS2 = 	P1^6;
sbit E = 		P3^3;
sbit RW = 	P3^4;
sbit RS = 	P3^5;
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
void Display(unsigned int weight);        
void Delay(unsigned int cnt);               
unsigned char code zhong[]={0x10,0x10,0x14,0xD4,0x54,0x54,0x54,0xFC,0x52,0x52,0x52,0xD3,0x12,0x10,0x10,0x00,0x40,0x40,0x50,0x57,0x55,0x55,0x55,0x7F,0x55,0x55,0x55,0x57,0x50,0x40,0x40,0x00};
unsigned char code liang[]={0x20,0x20,0x20,0xBE,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xBE,0x20,0x20,0x20,0x00,0x00,0x80,0x80,0xAF,0xAA,0xAA,0xAA,0xFF,0xAA,0xAA,0xAA,0xAF,0x80,0x80,0x00,0x00};
unsigned char code wei[]={0x00,0x20,0x22,0x2C,0x20,0x20,0xE0,0x3F,0x20,0x20,0x20,0x20,0xE0,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x06,0x01,0x00,0x01,0x46,0x80,0x40,0x3F,0x00,0x00,0x00};
unsigned char code maohao[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
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
unsigned char code ke[]={0x04,0x04,0xE4,0x24,0x24,0x24,0x24,0x3F,0x24,0x24,0x24,0x24,0xE4,0x04,0x04,0x00,0x80,0x80,0x43,0x22,0x12,0x0E,0x02,0x02,0x02,0x7E,0x82,0x82,0x83,0x80,0xE0,0x00};



//AD
void InitAD(unsigned char n)
{ 
 
    PLASF = 0xff;  
    ADC_RES = 0;
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(10000);    

    n &= 0x07;        
    AUXR1 |= 0x04;       
    PLASF = 1<<n;        

    PLASF = 0X01;   
    ADC_CONTR = 0X10;
    EA = 1;
    ADC_RES = 0X00;
    ADC_RESL = 0X00;
    EADC = 1;
}
unsigned int ADC_GET(unsigned char n)
{
    
    unsigned int ADCData;
    n &= 0x07;            
    ADC_RES = 0;        
    ADC_RESL = 0;        
    ADC_CONTR = 0;                           
    ADC_CONTR |= (ADC_POWER|ADC_SPEEDLL|n|ADC_START);
    DelayUs(6);                                  
    while(!((ADC_CONTR & ADC_FLAG) == 0x10)) ADCData = (ADC_RES&0x03)*256 + ADC_RESL;
    return (ADCData-1545)/2;          
}
void DelayUs(unsigned int n)
{
    unsigned int temp=0;
    for(temp=0;temp<n;temp++) _nop_();
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
void Display(unsigned int weight){

    unsigned char a,b,c;
    DisplayWord(1,1,0*16,zhong);
    DisplayWord(1,1,1*16,liang);
    DisplayWord(1,1,2*16,wei);
    DisplayWord(1,1,3*16,maohao);
    a = weight/100%10;
    b = weight%100/10;
    c = weight%10;
    switch(a){
        case 0: DisplayWord(2,4,0*16,ling);break;
        case 1: DisplayWord(2,4,0*16,yi);break;
        case 2: DisplayWord(2,4,0*16,er);break;
        case 3: DisplayWord(2,4,0*16,san);break;
        case 4: DisplayWord(2,4,0*16,si);break;
        case 5: DisplayWord(2,4,0*16,wu);break;
        case 6: DisplayWord(2,4,0*16,liu);break;
        case 7: DisplayWord(2,4,0*16,qi);break;
        case 8: DisplayWord(2,4,0*16,ba);break;
        case 9: DisplayWord(2,4,0*16,jiu);break;
    }
    switch(b){
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
    switch(c){
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
    DisplayWord(2,4,3*16,ke);
    Delay(5000);
}
void Delay(unsigned int cnt)
{
    while(--cnt);
}



void main()
{
    InitAD(0);
    InitLCD();
    while(1) Display(ADC_GET(0));
}