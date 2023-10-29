
#define ext_int_pin 34

float frequency8=10; float duration8=0.05; float delay8=0*1000;
float interval7=0; float duration7=0;  float nPulses7=0;// train freq
float pause7=0;

float frequency0=0.5; float duration0=1.5; float delay0=0*1000;
float interval6=0; float duration6=0; float nPulses6=0; // train freq
float pause6=0;

float ra0=1; float rc0=1; float ra6=1; float rc6=1;
float ra7=1; float rc7=1; float ra8=1; float rc8=1;
int ext_trig=0; 

int serialData=0;

void setup() 
  {
    pinMode(30,INPUT);
    pinMode(22,INPUT);
    pinMode(ext_int_pin,INPUT);
    pmc_set_writeprotect(false);
    
    pmc_enable_periph_clk(27);  //pin 2, XC0: 22
    pmc_enable_periph_clk(TC0_IRQn);
    REG_PMC_PCER0 |= PMC_PCER0_PID27;                 // Enable peripheral TC0 (TC0 Channel 0)
    REG_PIOB_ABSR |= PIO_ABSR_P25;     // Switch the multiplexer to peripheral B for TIOA6 and TIOB6
    REG_PIOB_PDR |= PIO_PDR_P25;        // Disable the GPIO on the corresponding pins
    TC_Configure(TC0,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC0 | TC_CMR_EEVT_XC0 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1);   

    pmc_enable_periph_clk(33); //pin 5
    pmc_enable_periph_clk(TC6_IRQn);
    REG_PMC_PCER1 |= PMC_PCER1_PID33;                 // Enable peripheral TC2 (TC2 Channel 0)
    REG_PIOC_ABSR |= PIO_ABSR_P25;     // Switch the multiplexer to peripheral B for TIOA6 and TIOB6
    REG_PIOC_PDR |= PIO_PDR_P25;        // Disable the GPIO on the corresponding pins
    TC_Configure(TC2,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        
    pmc_enable_periph_clk(34); //pin 3
    pmc_enable_periph_clk(TC7_IRQn);
    REG_PMC_PCER1 |= PMC_PCER1_PID34;                 // Enable peripheral TC2 (TC2 Channel 0)
    REG_PIOC_ABSR |= PIO_ABSR_P28;     // Switch the multiplexer to peripheral B for TIOA6 and TIOB6
    REG_PIOC_PDR |= PIO_PDR_P28;        // Disable the GPIO on the corresponding pins
    TC_Configure(TC2,1,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 

    pmc_enable_periph_clk(35); //pin 11, XC2:30
    pmc_enable_periph_clk(TC8_IRQn);
    REG_PMC_PCER1 |= PMC_PCER1_PID35;                
    REG_PIOD_ABSR |= PIO_ABSR_P7;     
    REG_PIOD_PDR |= PIO_PDR_P7;          
    TC_Configure(TC2,2,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC2 | TC_CMR_EEVT_XC2 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
    
    rc0=VARIANT_MCK/2/10;
    ra0=VARIANT_MCK/2*0.05;
    rc6=VARIANT_MCK/2*0;
    ra6=VARIANT_MCK/2*0;
    rc7=VARIANT_MCK/2*0;
    ra7=VARIANT_MCK/2*0;
    rc8=VARIANT_MCK/2/10;
    ra8=VARIANT_MCK/2*0.05;
        
    TC_SetRC(TC0,0, rc0);
    TC_SetRA(TC0,0, ra0);
    TC_SetRC(TC2,0, rc6);
    TC_SetRA(TC2,0, ra6);
    TC_SetRC(TC2,1, rc7);
    TC_SetRA(TC2,1, ra7);
    TC_SetRC(TC2,2, rc8);
    TC_SetRA(TC2,2, ra8);
    Serial.begin(115200);
  }

void loop() 
{
 if (Serial.available())
 { 
    serialData=Serial.read();

    if (serialData==112)
    {
      frequency8=Serial.parseFloat(); duration8=Serial.parseFloat()/1000; delay8=Serial.parseFloat()*1000; 
      interval7=Serial.parseFloat(); duration7=Serial.parseFloat(); nPulses7=Serial.parseFloat(); 
      pause7=0;
      
      frequency0=Serial.parseFloat(); duration0=Serial.parseFloat()/1000; delay0=Serial.parseFloat()*1000; 
      interval6=Serial.parseFloat(); duration6=Serial.parseFloat(); nPulses6=Serial.parseFloat(); 
      pause6=0;
      
      ext_trig=Serial.parseInt();
      
      while (Serial.available()> 0){serialData=Serial.read();} 
      if (duration7!=0 && duration7>=interval7 && nPulses7==0) {interval7=2*duration7;}
      if (duration7!=0 && duration6>=interval6 && nPulses6==0) {interval6=2*duration6;}
      if (nPulses7>0) {duration7=(nPulses7-1)/frequency8+0.5*(duration8+1/frequency8); interval7=3*duration7; pause7=2*1000000*duration7;}
      if (nPulses6>0) {duration6=(nPulses6-1)/frequency0+0.5*(duration0+1/frequency0); interval6=3*duration6; pause6=2*1000000*duration6;}

      configure_channels(frequency8, duration8, delay8, interval7, duration7, nPulses7, pause7, frequency0, duration0, delay0, interval6,duration6, nPulses6,pause6);

      serialData=0;
    }
            
    if (serialData==49)
    {
      //      TC_Start(TC2,0); 
      //      TC_Start(TC2,1);
      //      TC2->TC_BCR = TC_BCR_SYNC;
      //      TC_Start(TC2,2);
      //      TC_Start(TC0,0);

      if (ext_trig==0)
      {
   
        TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
        TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
        TC2->TC_BCR = TC_BCR_SYNC;
        
        if (delay0>delay8)
        {
          if (delay8>0) {delayMicroseconds(delay8);}
          TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
          delayMicroseconds(delay0-delay8);
          TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
        }

        if (delay8>delay0)
        {
          if (delay0>0) {delayMicroseconds(delay0);}
          TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
          delayMicroseconds(delay8-delay0);
          TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
        }

        if (delay8==delay0)
        {
          if (delay0>0) {delayMicroseconds(delay0);}
          TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
          TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
        }

        
        if (nPulses6>0 && nPulses7>0) 
        {
          if (pause6>pause7)
          {
            delayMicroseconds(pause7);
            TC_Stop(TC2,2);
            TC_Stop(TC2,1);
            delayMicroseconds(pause6-pause7);
            TC_Stop(TC0,0);
            TC_Stop(TC2,0);}
          else 
          {
            delayMicroseconds(pause6);
            TC_Stop(TC0,0);
            TC_Stop(TC2,0);
            delayMicroseconds(pause7-pause6);
            TC_Stop(TC2,2);
            TC_Stop(TC2,1);
          }
        }
        
        if (nPulses6>0 && nPulses7==0) 
        {
          delayMicroseconds(pause6);
          TC_Stop(TC0,0);
          TC_Stop(TC2,0);
        }
        if (nPulses6==0 && nPulses7>0) 
        {
          delayMicroseconds(pause7);
          TC_Stop(TC2,2);
          TC_Stop(TC2,1);
        }
   
      }

      else 
      {
        attachInterrupt(digitalPinToInterrupt(ext_int_pin),exttrig_func,RISING);
      }
    }
    
    if (serialData==48)
    { 
      if (ext_trig==0)
      {
//        if (TC0->TC_CHANNEL[0].TC_CV<ra0){TC0->TC_CHANNEL[0].TC_RA=TC0->TC_CHANNEL[0].TC_CV;}
//        if (TC2->TC_CHANNEL[0].TC_CV<ra6){TC2->TC_CHANNEL[0].TC_RA=TC2->TC_CHANNEL[0].TC_CV;}
//        if (TC2->TC_CHANNEL[1].TC_CV<ra7){TC2->TC_CHANNEL[1].TC_RA=TC2->TC_CHANNEL[1].TC_CV;}
//        if (TC2->TC_CHANNEL[2].TC_CV<ra8){TC2->TC_CHANNEL[2].TC_RA=TC2->TC_CHANNEL[2].TC_CV;}
        
        TC_Stop(TC2,0);
        TC_Stop(TC0,0);
        TC_Stop(TC2,1); 
        TC_Stop(TC2,2);

        TC0->TC_CHANNEL[0].TC_RA=1;
        TC2->TC_CHANNEL[0].TC_RA=1;
        TC2->TC_CHANNEL[1].TC_RA=1;
        TC2->TC_CHANNEL[2].TC_RA=1;

        TC_Start(TC2,0);
        TC_Start(TC0,0);
        TC_Start(TC2,1); 
        TC_Start(TC2,2);

        TC_Stop(TC2,0);
        TC_Stop(TC0,0);
        TC_Stop(TC2,1); 
        TC_Stop(TC2,2);


        TC0->TC_CHANNEL[0].TC_RA=ra0;
        TC2->TC_CHANNEL[0].TC_RA=ra6;
        TC2->TC_CHANNEL[1].TC_RA=ra7;
        TC2->TC_CHANNEL[2].TC_RA=ra8;
      }
      else
      {
        if (TC0->TC_CHANNEL[0].TC_CV<ra0){TC0->TC_CHANNEL[0].TC_RA=TC0->TC_CHANNEL[0].TC_CV+10;}
        if (TC2->TC_CHANNEL[0].TC_CV<ra6){TC2->TC_CHANNEL[0].TC_RA=TC2->TC_CHANNEL[0].TC_CV+10;}
        if (TC2->TC_CHANNEL[1].TC_CV<ra7){TC2->TC_CHANNEL[1].TC_RA=TC2->TC_CHANNEL[1].TC_CV+10;}
        if (TC2->TC_CHANNEL[2].TC_CV<ra8){TC2->TC_CHANNEL[2].TC_RA=TC2->TC_CHANNEL[2].TC_CV+10;}
        
        TC_Stop(TC2,0);
        TC_Stop(TC0,0);
        TC_Stop(TC2,1); 
        TC_Stop(TC2,2);
  
        TC0->TC_CHANNEL[0].TC_RA=ra0;
        TC2->TC_CHANNEL[0].TC_RA=ra6;
        TC2->TC_CHANNEL[1].TC_RA=ra7;
        TC2->TC_CHANNEL[2].TC_RA=ra8;
        
        detachInterrupt(digitalPinToInterrupt(ext_int_pin));
      }
    }
  }   

}

void TC0_Handler() {TC_GetStatus(TC0, 0);}
void TC6_Handler() {TC_GetStatus(TC2, 0);}
void TC7_Handler() {TC_GetStatus(TC2, 1);}
void TC8_Handler() {TC_GetStatus(TC2, 2);}

void exttrig_func()
{
  TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  TC2->TC_BCR = TC_BCR_SYNC;
  TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  
  if (nPulses6>0 && nPulses7>0) 
  {
    if (pause6>pause7)
    {
      delayMicroseconds(pause7);
      TC_Stop(TC2,2);
      TC_Stop(TC2,1);
      delayMicroseconds(pause6-pause7);
      TC_Stop(TC0,0);
      TC_Stop(TC2,0);}
    else 
    {
      delayMicroseconds(pause6);
      TC_Stop(TC0,0);
      TC_Stop(TC2,0);
      delayMicroseconds(pause7-pause6);
      TC_Stop(TC2,2);
      TC_Stop(TC2,1);
    }
  }
  
  if (nPulses6>0 && nPulses7==0) 
  {
    delayMicroseconds(pause6);
    TC_Stop(TC0,0);
    TC_Stop(TC2,0);
  }
  if (nPulses6==0 && nPulses7>0) 
  {
    delayMicroseconds(pause7);
    TC_Stop(TC2,2);
    TC_Stop(TC2,1);
  }
}

void configure_channels(float frequency8, float duration8, float delay8, float interval7, float duration7, float nPulses7, float pause7, float frequency0, float duration0, float delay0, float interval6, float duration6, float nPulses6, float pause6)
{
      if (duration6==0 && nPulses6==0) 
      {
        duration6=1500; interval6=1600; 
        TC_Configure(TC0,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_NONE| TC_CMR_TCCLKS_TIMER_CLOCK1); 
      }
      if (duration7==0 && nPulses7==0) 
      {
        duration7=1500; interval7=1600; 
        TC_Configure(TC2,2,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_NONE| TC_CMR_TCCLKS_TIMER_CLOCK1); 
      }

      rc0=VARIANT_MCK/2/frequency0;
      ra0=VARIANT_MCK/2*duration0;
      rc8=VARIANT_MCK/2/frequency8;
      ra8=VARIANT_MCK/2*duration8;
          
      TC_SetRC(TC0,0, rc0);
      TC_SetRA(TC0,0, ra0);
      TC_SetRC(TC2,2, rc8);
      TC_SetRA(TC2,2, ra8);


      if (interval6>0 && interval6<=100) 
      {
        TC_Configure(TC2,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        TC_Configure(TC0,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC0 | TC_CMR_EEVT_XC0 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1);         

        rc6=VARIANT_MCK/2*interval6;
        ra6=VARIANT_MCK/2*duration6;
        TC_SetRC(TC2,0, rc6);
        TC_SetRA(TC2,0, ra6);
      }
      
      if (interval6>100 && interval6<=400) 
      {
        TC_Configure(TC2,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK2); 
        TC_Configure(TC0,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC0 | TC_CMR_EEVT_XC0 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1);         
        rc6=VARIANT_MCK/8*interval6;
        ra6=VARIANT_MCK/8*duration6;
        TC_SetRC(TC2,0, rc6);
        TC_SetRA(TC2,0, ra6);
      }

      if (interval6>400 && interval6<=1600) 
      {
        TC_Configure(TC2,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK3);
        TC_Configure(TC0,0,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC0 | TC_CMR_EEVT_XC0 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1);         
        rc6=VARIANT_MCK/32*interval6;
        ra6=VARIANT_MCK/32*duration6;
        TC_SetRC(TC2,0, rc6);
        TC_SetRA(TC2,0, ra6);
      }

      if (interval7>0 && interval7<=100) 
      {
        TC_Configure(TC2,1,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        TC_Configure(TC2,2,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC2 | TC_CMR_EEVT_XC2 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        rc7=VARIANT_MCK/2*interval7;
        ra7=VARIANT_MCK/2*duration7;
        TC_SetRC(TC2,1, rc7);
        TC_SetRA(TC2,1, ra7);
      }

      if (interval7>100 && interval7<=400) 
      {
        TC_Configure(TC2,1,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK2); 
        TC_Configure(TC2,2,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC2 | TC_CMR_EEVT_XC2 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        rc7=VARIANT_MCK/8*interval7;
        ra7=VARIANT_MCK/8*duration7;
        TC_SetRC(TC2,1, rc7);
        TC_SetRA(TC2,1, ra7);
      }

      if (interval7>400 && interval7<=1600) 
      {
        TC_Configure(TC2,1,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_TCCLKS_TIMER_CLOCK3); 
        TC_Configure(TC2,2,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_ASWTRG_SET | TC_CMR_BURST_XC2 | TC_CMR_EEVT_XC2 | TC_CMR_ENETRG | TC_CMR_EEVTEDG_RISING | TC_CMR_AEEVT_SET | TC_CMR_TCCLKS_TIMER_CLOCK1); 
        rc7=VARIANT_MCK/32*interval7;
        ra7=VARIANT_MCK/32*duration7;
        TC_SetRC(TC2,1, rc7);
        TC_SetRA(TC2,1, ra7);
      }      
        
}
