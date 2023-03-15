/*NOT TESTED
 * ONLY GENERATES THE 19KHZ PILOT AND 38KHZ DSB
 * NEED A RESISTOR NETWORK TO GENERATE THE R+L
 * NEED A FILTER SQUARE TO SINE FOR 19K & 38K
 * 
 * BEFORE UPLOADING CODE, IN THE TOOLS MENU
 * SELECT "INTERNAL 8 MHZ" AND ATINY85, THEN
 * CLICK BURN BOOTLOADER
 * 
 * THEN UPLOAD
 * 
 * stereo encoder Pin defenitions:
 *  PB1(pin 6) - 38Khz out
 *  PB4(pin 3) - 19Khz pilot out
 *  PB3(pin 2) - left audio input
 *  PB2(pin 7) - right audio input
 *  1    *    8
 *  2         7
 *  3         6
 *  4         5
 */

void set_adc_left(){
 ADMUX=67;
}
void set_adc_right(){
 ADMUX=65;
}
void start_conv(){
 ADCSRA|=(1<<6);  
}
void setup_adc(){
  //left: ADC3
  //right: ADC1
  set_adc_right();
  ADCSRA=(1<<7);
}

void setup() {
  // put your setup code here, to run once:
  DDRB=(1<<1)|(1<<4);//OC1B&OC0B disable OC1A
  TCCR1=2;//prescale by 2
  GTCCR=(1<<6)|(1<<4);
  OCR1C=211;
  OCR1B=211/2;//211 18.957 khz
  //19khz pilot tone done
  //begin 38khz mod
  TCCR0A=(1<<5)|3;
  TCCR0B=(1<<3)|1;//no pre scaling
  OCR0A=211;//frequency 211=37.914691 khz
  OCR0B=255;//modulation 0-211
  setup_adc();
}

void loop() {
  short right=0;
  short left=0;
  char channel=0;
  set_adc_right();
  start_conv();
  while(1==1){
   short diff=(left-right);
   if(diff<0){diff=-diff;}
   if(diff>5){
   OCR0B=diff/5;
   }else{
    OCR0B=0; 
   }
   if((ADCSRA&64)==0){
      switch(channel){
        case 0:
         right=ADCL|(ADCH<<8); 
         set_adc_left();
         start_conv();
         channel=1;
         break;
        case 1:
         left=ADCL|(ADCH<<8); 
         set_adc_right();
         start_conv();
         channel=0;
      }
   }
  }
}
