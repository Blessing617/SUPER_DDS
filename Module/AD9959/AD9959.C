#include "AD9959.H"

u8 CSR_DATA0[1] = {0x10};      // 开 CH0
u8 CSR_DATA1[1] = {0x20};      // 开 CH1
u8 CSR_DATA2[1] = {0x40};      // 开 CH2
u8 CSR_DATA3[1] = {0x80};      // 开 CH3		
																	
u8 FR1_DATA[3]	 = 	{0xD0,0x00,0x00};		//20倍频 
u8 FR2_DATA[2] 	 = 	{0x00,0x01};			//default Value = 0x0000
u8 CFR_DATA[3] 	 = 	{0x00,0x03,0x02};		//default Value = 0x000302	   														
u8 CPOW0_DATA[2] = 	{0x00,0x00};			//default Value = 0x0000   @ = POW/2^14*360													
u8 LSRR_DATA[2]  = 	{0x00,0x00};	
u8 RDW_DATA[4] 	 = 	{0x00,0x00,0x00,0x00};																
u8 FDW_DATA[4]   = 	{0x00,0x00,0x00,0x00};
u8 ACRdata[3]    = 	{0x00,0x10,0x00};		//default Value = 0x--0000 Rest = 18.91/Iout 

//AD9959初始化
void Init_AD9959(void)  
{ 
	Intserve();  //IO口初始化
	IntReset();  //AD9959复位  
	WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//写功能寄存器1
} 
//延时
void delay1 (u32 length)
{
	length = length*12;
	while(length--);
}

//IO口初始化
void Intserve(void)		   
{   	
    CS_SET;
    SCLK_RESET;
    UPDATE_RESET;
	SDIO0_RESET;
}

//AD9959复位
void IntReset(void)	  
{
	Reset_RESET;
	delay1(1);
	Reset_SET;
	delay1(30);
	Reset_RESET;
}

 //AD9959更新数据
void IO_Update(void)  
{
	UPDATE_RESET;
	delay1(2);
	UPDATE_SET;
	delay1(4);
	UPDATE_RESET;
}

//SPI数据写入
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData,u8 temp)
{
	u8	ControlValue = 0;
	u8	ValueToWrite = 0;
	u8	RegisterIndex = 0;
	u8	i = 0;

	ControlValue = RegisterAddress;
//写入地址
	SCLK_RESET;
	CS_RESET;	 
	for(i=0; i<8; i++)
	{
		SCLK_RESET;
		if(0x80 == (ControlValue & 0x80))
		SDIO0_SET;	  
		else
		SDIO0_RESET;	  
		SCLK_SET;
		ControlValue <<= 1;
	}
	SCLK_RESET;
//写入数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_RESET;
			if(0x80 == (ValueToWrite & 0x80))
			SDIO0_SET;	  
			else
			SDIO0_RESET;	  
			SCLK_SET;
			ValueToWrite <<= 1;
		}
		SCLK_RESET;		
	}	
	if(temp==1)
	IO_Update();	
  CS_SET;
} 

//设置单个通道频率
void Write_frequence(u8 Channel,u32 Freq)
{	 
	  u8 CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  u32 Temp;            
	  Temp=(u32)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
		
	  CFTW0_DATA[3]=(u8)Temp;
	  CFTW0_DATA[2]=(u8)(Temp>>8);
	  CFTW0_DATA[1]=(u8)(Temp>>16);
	  CFTW0_DATA[0]=(u8)(Temp>>24);
	  if(Channel==0)	  
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写入CH0通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
		}
	  else if(Channel==1)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写入CH1通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率	
	  }
	  else if(Channel==2)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写入CH2通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率	
	  }
	  else if(Channel==3)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写入CH3通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);//CTW0 address 0x04.输出CH3设定频率	
	  }																																																																										 
} 

//设置单个通道幅度<1023
void Write_Amplitude(u8 Channel, u16 Ampli)
{ 
	u16 A_temp;  
	u8 ACR_DATA[3] = {0x00,0x00,0x00};
	
	A_temp=Ampli|0x1000;
	ACR_DATA[2] = (u8)A_temp;  //低位数据
	ACR_DATA[1] = (u8)(A_temp>>8); //高位数据
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
}

//设置单个通道相位
void Write_Phase(u8 Channel,u16 Phase)
{
	u16 P_temp=0;
	P_temp=(u16)Phase*45.511111;  //将输入相位差写入，进度1度，45.511111=2^14）/360
	CPOW0_DATA[1]=(u8)P_temp;
	CPOW0_DATA[0]=(u8)(P_temp>>8);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==2)
	{	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
}

//设置4个通道的频率
void AD9959_SetFrequency4Channel(u32 Freq0,u32 Freq1,u32 Freq2,u32 Freq3)
{
	
	Write_frequence(0,Freq0);
	Write_frequence(1,Freq1);
	Write_frequence(2,Freq2);
	Write_frequence(3,Freq3);	
	WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);
}

//设置4个通道的幅度
void AD9959_SetAmp4Channel(u32 Amp0,u32 Amp1,u32 Amp2,u32 Amp3)
{
	Write_Amplitude(0,Amp0);
	Write_Amplitude(1,Amp1);
	Write_Amplitude(2,Amp2);
	Write_Amplitude(3,Amp3);	
	WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);
}

//设置4个通道的相位
void AD9959_SetPhase4Channel(u32 Phase0,u32 Phase1,u32 Phase2,u32 Phase3)
{
	Write_Phase(0,Phase0);
	Write_Phase(1,Phase1);
	Write_Phase(2,Phase2);
	Write_Phase(3,Phase3);	
	WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);
}

//四通道同频
void AD9959_Set_Freq(u32 Freq)
{
	  u8 CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  u32 Temp;            
	  Temp=(u32)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
	  CFTW0_DATA[3]=(u8)Temp;
	  CFTW0_DATA[2]=(u8)(Temp>>8);
	  CFTW0_DATA[1]=(u8)(Temp>>16);
	  CFTW0_DATA[0]=(u8)(Temp>>24);
		
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);	//控制寄存器写入CH0通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);	//CTW0 address 0x04.输出CH0设定频率
	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);	//控制寄存器写入CH1通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);	//CTW0 address 0x04.输出CH1设定频率	

	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);	//控制寄存器写入CH2通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);	//CTW0 address 0x04.输出CH2设定频率	
	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);	//控制寄存器写入CH3通道
		WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);	//CTW0 address 0x04.输出CH3设定频率
}

//简单扫频控制
void mode1(uint32_t starthz, uint32_t stophz, uint32_t stephz)
{
	uint32_t i = 0;
	if (starthz < stophz) //从低到高
	{
		for (i = 0; i < ((stophz - starthz) / stephz); i++)
		{
			AD9959_Set_Freq(starthz + i * stephz);
		}
	}
	else 
	{
		for (i = ((starthz - stophz) / stephz); i > 0; i--)
		{
			AD9959_Set_Freq( starthz + i * stephz);
		}
	}
}
