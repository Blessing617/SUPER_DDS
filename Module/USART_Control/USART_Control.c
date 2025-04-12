#include "USART_Control.h"

double fre = 35000000;
double amp = 1023;
double phase = 0;

void USTART_Control(void)
{
	uint8_t Rx_data[100];
    if(MyUSART1_ReadLine(Rx_data))
    {  
		
      if(Rx_data[0]=='9'&& Rx_data[1]=='8'&& Rx_data[2]=='3'&& Rx_data[3]=='4')
	  {
		  sscanf((char*)Rx_data+5,"%lf",&fre);
		  AD9834_Set_Freq(FREQ_0,fre);
	  }
	  
      if(Rx_data[0]=='9'&& Rx_data[1]=='9'&& Rx_data[2]=='5'&& Rx_data[3]=='9')
	  {
		  if(Rx_data[4]=='0')
		  {
			  if(Rx_data[5]=='f')
			  {
				sscanf((char*)Rx_data+7,"%lf",&fre);
				Write_frequence(0,fre);				  
			  }
			  else if(Rx_data[5]=='a')
			  {
				sscanf((char*)Rx_data+7,"%lf",&amp);
				Write_Amplitude(0,amp);
			  }
			  else if(Rx_data[5]=='p')
			  {
				sscanf((char*)Rx_data+7,"%lf",&phase);
				AD9959_SetPhase4Channel(phase,0,0,0);
			  }			  			  
		  }
		  else if(Rx_data[4]=='1')
		  {
			  if(Rx_data[5]=='f')
			  {
				sscanf((char*)Rx_data+7,"%lf",&fre);
				Write_frequence(1,fre);	
			  }	
			  else if(Rx_data[5]=='a')
			  {
				sscanf((char*)Rx_data+7,"%lf",&amp);
				Write_Amplitude(1,amp);
			  }		
			  else if(Rx_data[5]=='p')
			  {
				sscanf((char*)Rx_data+7,"%lf",&phase);
				AD9959_SetPhase4Channel(0,phase,0,0);
			  }				  
		  }
		  else if(Rx_data[4]=='2')
		  {
			  if(Rx_data[5]=='f')
			  {
				sscanf((char*)Rx_data+7,"%lf",&fre);
				Write_frequence(2,fre);
			  }		
			  else if(Rx_data[5]=='a')
			  {
				sscanf((char*)Rx_data+7,"%lf",&amp);
				Write_Amplitude(2,amp);
			  }	
			  else if(Rx_data[5]=='p')
			  {
				sscanf((char*)Rx_data+7,"%lf",&phase);
				AD9959_SetPhase4Channel(0,0,phase,0);
			  }				  
		  }
		  else if(Rx_data[4]=='3')
		  {
			  if(Rx_data[5]=='f')
			  {
				sscanf((char*)Rx_data+7,"%lf",&fre);
				Write_frequence(3,fre);
			  }	
			  else if(Rx_data[5]=='a')
			  {
				sscanf((char*)Rx_data+7,"%lf",&amp);
				Write_Amplitude(3,amp);
			  }		
			  else if(Rx_data[5]=='p')
			  {
				sscanf((char*)Rx_data+7,"%lf",&phase);
				AD9959_SetPhase4Channel(0,0,0,phase);
			  }				  
		  }		  
	  }	 
	  
	  if(Rx_data[0]=='9'&& Rx_data[1]=='9'&& Rx_data[2]=='1'&& Rx_data[3]=='0')
	  {
		  if(Rx_data[4]=='f')
		  {
			  sscanf((char*)Rx_data+6,"%lf",&fre);
			  Freq_convert(fre);
		  }
		  else if(Rx_data[4]=='a')
		  {
			  sscanf((char*)Rx_data+6,"%lf",&amp);
			  Amp_convert(amp);
		  }
	  }
    }
}
