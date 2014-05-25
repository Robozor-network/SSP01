#include "main.h"

#define LED1 PIN_A1  
#define LED2 PIN_A2  

int8 lsb, msb;
signed int16 pwm;

#int_SSP
void  SSP_isr(void) 
{
   int8 state;
   
   state = i2c_isr_state();
   if (state == 0) 
   {
      i2c_read();
   }
   
   if (state == 1) 
   {
      lsb = i2c_read();
   }
   
   if (state == 2) 
   {
      output_low(LED2);
      msb = i2c_read();
      pwm = MAKE16(msb,lsb);
      if (pwm==0)
      {
         set_pwm1_duty(0);
         set_pwm2_duty(0);      
      }
      if (pwm>0)
      {
         set_pwm1_duty(pwm);
         set_pwm2_duty(0);      
      }
      if (pwm<0)
      {
         set_pwm1_duty(0);
         set_pwm2_duty(abs(pwm));      
      }
   }
}


void main()
{
   setup_oscillator(OSC_8MHZ|OSC_INTRC);
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
//!!!   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_16,255,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
//   setup_ccp1(CCP_PWM_FULL_BRIDGE|CCP_SHUTDOWN_AC_L|CCP_SHUTDOWN_BD_L);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
//   set_pwm1_duty(512);
//   set_pwm2_duty(512);
   set_pwm1_duty(0);
   set_pwm2_duty(0);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);

   printf("Cvak.\r\n");
   pwm=0;
    //Example blinking LED program
    while(true){
      printf("Hmm...%Ld\r\n",pwm);
      output_low(LED1);
      delay_ms(500);
      output_high(LED1);
      output_high(LED2);
      delay_ms(500);
    }

}
