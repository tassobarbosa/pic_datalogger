/* Tasso Barbosa - tasso_barbosa@yahoo.com.br
 * Description: [hello world example on how to create a datalogger that create,
		 edit and save a file in a micro SD card using the uC pic18 and
		 its eeprom so it won't delete your data when turned off]
 * obs.: Card must be formatted as FAT32 before starting the code
 
 * I acknowledge all content contained herein, excluding libraries
 * code, is my own original work.
 */

#include <18f4550.h>
#use delay(clock = 4000000)
#fuses HS, NOWDT, PUT, NOLVP
#use rs232(baud=9600, UART1, errors)

 #define MMCSD_PIN_SCL     PIN_B1 //o
 #define MMCSD_PIN_SDI     PIN_C7 //i
 #define MMCSD_PIN_SDO     PIN_B0 //o
 #define MMCSD_PIN_SELECT  PIN_B4 //o

#include <mmcsd.c>
#include <fat.c>

main(){
char filename[]="/teste.txt";
char endline[]="\r\n";
char a[] = "a";
char ing[]="Hello World";
char pt[]="Ola mundo";
char fr[]="Bonjour le monde";
int ep_status = 0;
FILE stream;

//255 standard initial value
//check if is the first time the uC was initialized
ep_status = read_eeprom (1);
if(ep_status == 255){ 
      write_eeprom(1,0);
      output_high(PIN_C0);
      output_high(PIN_C1);
}
   else output_high(PIN_C1);
   
   delay_ms(500);
   output_low(PIN_C0);
   output_low(PIN_C1);

ep_status = read_eeprom (1);
   
//inicializa fat
if(fat_init()==0) output_high(PIN_C0);
   else output_high(PIN_C1);
   delay_ms(100);
   output_low(PIN_C0);
   output_low(PIN_C1);

//formata apenas se for primeira inicializacao
if(ep_status == 0){
   //formata cartao 2G para FAT32
   if(!format(2097152)) output_high(PIN_C0);
      else output_high(PIN_C1);
      delay_ms(100);
      output_low(PIN_C0);
      output_low(PIN_C1);
      // 1 - significa que ja foi inicializado
      write_eeprom(1,1);
   }
   
//cria arquivo denominado por 'filename'
if(mk_file(filename)==0)output_high(PIN_C0);
   else output_high(PIN_C1);
   delay_ms(100);
   output_low(PIN_C0);
   output_low(PIN_C1);

//cria uma stream do arquivo para edicoes - A: append
if(fatopen(filename, a, &stream) == GOODEC) output_high(PIN_C0);
   else output_high(PIN_C1);
   delay_ms(250);
   output_low(PIN_C0);
   output_low(PIN_C1);
   
//escreve 3 frases separadas por quebra de linha   
   fatputs(ing, &stream);
   fatputs(endline, &stream);
   fatputs(pt, &stream);
   fatputs(endline, &stream);
   fatputs(fr, &stream);
   fatputs(endline, &stream);   

//fecha stream do arquivo
if(fatclose(&stream) == GOODEC) output_high(PIN_C0);
   else output_high(PIN_C1);
   delay_ms(250);
   output_low(PIN_C0);
   output_low(PIN_C1);
   
//escreve o arquivo na posicao 0 da memoria   
if(mmcsd_write_byte(0, filename)==0)output_high(PIN_C0);
   else output_high(PIN_C1);
   delay_ms(250);
   output_low(PIN_C0);
   output_low(PIN_C1);   
}

