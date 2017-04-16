#include "enhanced.h"

#define x1 P3DIR
#define y1 P3OUT
#define z1 P3SEL
#define BIT(q) (1<<(q))
#define y11 P3IN
#define trans UCB0TXBUF
#define rec UCB0RXBUF
char address;
int pktl;

//This function initializes the SPI interface between CC1101 and MSP430F5438A, does a Manual Power On Reset of the CC1101 and initializes the registers for transmission.
void init_transmit(int addr,int pktlen)
{
    pktl=pktlen;
    address=addr;
    x1|=BIT(3)|BIT(0);                                        //Set SCLK = 1 and SI = 0, to avoid potential problems with pin control mode
    y1|=BIT(3);                                                //Strobe CSn low / high.
    y1&=~BIT(0);                                               //Hold CSn high for at least 40µs relative to pulling CSn low
    __delay_cycles(50);
    y1|=BIT(0);
    __delay_cycles(50);                                        //Pull CSn low and wait for SO to go low
    y1&=~BIT(0);                                               //Issue the SRES strobe on the SI line
    while((y11&0x04)==0x04);                                   //When SO goes low again, reset is complete and the chip is in the IDLE state.
    z1|=BIT(1)|BIT(2)|BIT(3);
    UCB0CTL1=UCSWRST;
    UCB0CTL1=UCSSEL_2;
    UCB0IE=UCRXIE;   //This is extra. Delete to reset
    UCB0CTL0=UCCKPH+UCMSB+UCMST+UCMODE0+UCSYNC;   //UCSYNC is extra. delete to reset
    UCB0CTL1&=~UCSWRST;
    UCB0TXBUF=0xB0;                                            //SYSRES command strobe
    __delay_cycles(810);
    x1&=~(BIT(0));
    while((P3IN&0x04)==0x04);



    trans=0x40;                          //transmitting 01000000    (R/W bit is 0 and the burst bit is on)
    __delay_cycles(100);
    trans=0x2E;  // IOCFG2        GDO2 Output Pin Configuration //initially 2e. for receiver, it was 01
    __delay_cycles(100);
    trans=0x2E;  // IOCFG1        GDO1 Output Pin Configuration
    __delay_cycles(100);
    trans=0x06;  // IOCFG0        GDO0 Output Pin Configuration
    __delay_cycles(100);
    trans=0x4F;  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    __delay_cycles(100);
    trans=0x78;  // SYNC1         Sync Word, High Byte
    __delay_cycles(100);
    trans=0x9D;  // SYNC0         Sync Word, Low Byte
    __delay_cycles(100);
    trans=42;  // PKTLEN        Packet Length //setting it permanently to 64 bytes
    __delay_cycles(100);
    //unsigned char pktl=0x12;                                     //Writing packet length
    trans=0x01;  // PKTCTRL1      Packet Automation Control      //Initially 0x04
    __delay_cycles(100);
    trans=0x04;  // PKTCTRL0      Packet Automation Control    //setting to fixed packet packet length mode
    __delay_cycles(100);
    trans=addr;  // ADDR          Device Address                 //Setting broadcast address to 0x53
    //unsigned char address=0x53;                                  //Writing address
    __delay_cycles(100);
    trans=0x00;  // CHANNR        Channel Number
    __delay_cycles(100);
    trans=0x0C;  // FSCTRL1       Frequency Synthesizer Control
    __delay_cycles(100);
    trans=0x00;  // FSCTRL0       Frequency Synthesizer Control
    __delay_cycles(100);
    trans=0x10;  // FREQ2         Frequency Control Word; High Byte  //11      10
    __delay_cycles(100);
    trans=0xCE;  // FREQ1         Frequency Control Word; Middle Byte //4e      ce
    __delay_cycles(100);
    trans=0xC4;  // FREQ0         Frequency Control Word; Low Byte
    __delay_cycles(100);
    trans=0xF5;  // MDMCFG4       Modem Configuration
    __delay_cycles(100);
    trans=0x83;  // MDMCFG3       Modem Configuration
    __delay_cycles(100);
    trans=0x01;  // MDMCFG2       Modem Configuration        //Should be 0x00 in 2FSK and 0x30 in ASK
    __delay_cycles(100);
    trans=0x43;  // MDMCFG1       Modem Configuration   //changed from 0x23 to 0x43 for 4 additional preamble bytes (total 8)
    __delay_cycles(100);
    trans=0xF8;  // MDMCFG0       Modem Configuration
    __delay_cycles(100);
    trans=0x15;  // DEVIATN       Modem Deviation Setting
    __delay_cycles(100);
    trans=0x07;  // MCSM2         Main Radio Control State Machine Configuration
    __delay_cycles(100);
    trans=0x30;  // MCSM1         Main Radio Control State Machine Configuration
    __delay_cycles(100);
    trans=0x18;  // MCSM0         Main Radio Control State Machine Configuration
    __delay_cycles(100);
    trans=0x1D;  // FOCCFG        Frequency Offset Compensation Configuration
    __delay_cycles(100);
    trans=0x1C;  // BSCFG         Bit Synchronization Configuration
    __delay_cycles(100);
    trans=0x07;  // AGCCTRL2      AGC Control
    __delay_cycles(100);
    trans=0x00;  // AGCCTRL1      AGC Control
    __delay_cycles(100);
    trans=0xB0;  // AGCCTRL0      AGC Control
    __delay_cycles(100);
    trans=0x87;  // WOREVT1       High Byte Event0 Timeout
    __delay_cycles(100);
    trans=0x6B;  // WOREVT0       Low Byte Event0 Timeout
    __delay_cycles(100);
    trans=0x78;  // WORCTRL       Wake On Radio Control
    __delay_cycles(100);
    trans=0xB6;  // FREND1        Front End RX Configuration
    __delay_cycles(100);
    trans=0x10;  // FREND0        Front End TX Configuration         //Should be 0x10 in 2FSK and 0x11 in ASK
    __delay_cycles(100);
    trans=0xE9;  // FSCAL3        Frequency Synthesizer Calibration
    __delay_cycles(100);
    trans=0x2A;  // FSCAL2        Frequency Synthesizer Calibration
    __delay_cycles(100);
    trans=0x00;  // FSCAL1        Frequency Synthesizer Calibration
    __delay_cycles(100);
    trans=0x1F;  // FSCAL0        Frequency Synthesizer Calibration
    __delay_cycles(100);
    trans=0x41;  // RCCTRL1       RC Oscillator Configuration
    __delay_cycles(100);
    trans=0x00;  // RCCTRL0       RC Oscillator Configuration
    __delay_cycles(100);
    trans=0x59;  // FSTEST        Frequency Synthesizer Calibration Control
    __delay_cycles(100);
    trans=0x7F;  // PTEST         Production Test
    __delay_cycles(100);
    trans=0x3F;  // AGCTEST       AGC Test
    __delay_cycles(100);
    trans=0x81;  // TEST2         Various Test Settings
    __delay_cycles(100);
    trans=0x35;  // TEST1         Various Test Settings
    __delay_cycles(100);
    trans=0x09;  // TEST0         Various Test Settings


    x1|=BIT(0);            //Pulling Csn high, stopping burst access
    y1|=BIT(0);
    __delay_cycles(100);
    y1&=~BIT(0);
    while((P3IN&0x04)==0x04);
    __delay_cycles(1000);

    trans=0x3E;           //PATABLE    //SB access in 2FSK(0x3E) and burst access(0x7E) in ASK
    __delay_cycles(1000);
    trans=0xC0;           //Writing to PATABLE 00   //Should be 0xC0 if in 2FSK mode or 0x12 in ASK mode
    __delay_cycles(1000);
    /*trans=0xC0;                    //Comment out if not in OOK mode
  __delay_cycles(1000);
  trans=0x00;
  __delay_cycles(1000);
  trans=0x00;
  __delay_cycles(1000);
  trans=0x00;
  __delay_cycles(1000);
  trans=0x00;
  __delay_cycles(1000);
  trans=0x00;
  __delay_cycles(1000);
  trans=0x00;   */
}

int transmit(int data[],int z,int numdata)
{
    int i=0;
    int num=40;
    int tlen = numdata;
    int pktl=0;
    int xy;
    while(1)
    {
        xy = tlen/num;
        if(xy >= 1)
        {
            pktl = num+1;
        }
        else
        {
            pktl = tlen+1;
            trans = 0x06;
            __delay_cycles(100);
            trans = pktl+1;
            __delay_cycles(100);
        }
        /* Need to work/verify on this algorithm */

        trans = 0x7F;         //starting burst accest for TXfifo
        __delay_cycles(100);
        trans = pktl;            //Transmitting length field
        __delay_cycles(100);
        trans = z;         //Transmitting device address
        __delay_cycles(100);
        while(1)
        {
            trans = data[i];
            i++;
            if(i == numdata)
            {
                sending();
                return 0;
            }
            else if(i%num==0)
            {
                break;
            }
            __delay_cycles(2000);
        }
        sending();              //place a break point here next time you try. see if it ever reaches this point
        if(tlen > num)
            tlen = tlen-num;
    }
}


void sending()
{
    y1|=BIT(0);                //stopping burst access
    __delay_cycles(100);
    y1&=~BIT(0);
    while((P1IN&0x80)==0x80);  //port 1 pin 7
    __delay_cycles(1000);

    trans=0x35;                //CMD strobe for turn TX ON
    while(!(P1IN & 0x80));
    while(P1IN & 0x80);
    trans=0x36;                //set to sidle
    __delay_cycles(100);
    trans=0x3B;                //issuing sftx
    __delay_cycles(1000);
}
