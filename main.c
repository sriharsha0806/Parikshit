#include "enhanced.h"
//maximum frequency is 6.5 MHz for SPI burst access. Set to 6 MHz for precaution

//works correctly with packet RX QM rec

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                   // Stop watchdog timer to prevent time out reset
  extern char address;
  int data1[193]={0x00};
  int i=0;
  for(i=0;i<sizeof(data1);i++)
  {
      data1[i]=i;
  }
  extern int pktl;
  while(1)
  {
  init_transmit(0x53,150);                    //Arguements are (address,packetlength)
  transmit(data1,address, 150);               //Data is still an issue
  }
  return 0;
}
