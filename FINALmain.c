/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */
int  cont=0;
int temperatura=5;

void clk_init (void)
{
// FIRC = 4 MHz. BusClk = 4 MHz
// UART0: FIRC. UART1: BusClk. UART2: BusClk. TPM: FIRC. IIC: BusClk. PIT:
	MCG_C1|=(1<<6) + (1<<1); //MCGOUTCLK : IRCLK. CG: Clock gate, MCGIRCLK enable
	MCG_C2|=1; //Mux IRCLK : FIRC (4 MHz) pag 116
	MCG_SC=0; //Preescaler FIRC 1:1 pag 116
	SIM_CLKDIV1=0; //OUTDIV4=OUTDIV1= 1:1 pag 116. Busclk 4 MHz
	SIM_SOPT2|=15<<24; //Seleccion MCGIRCLK tanto para UART0 como para TPM
}
void GPIO_init(){
	SIM_SCGC5|=(1<<9)+(1<<11)+(1<<13); // Reloj de interfaz puerto A, C y E	
	
	PORTA_PCR16|=(1<<8)+(9<<16);	//Se hace GPIO puerto A16
	PORTA_PCR17|=(1<<8)+(9<<16);	//Se hace GPIO puerto A17
	//por defecto al configurar el GPIO es una entrada
	
	//estos son 4 ventiladores y las dos celdas controladas por 6 transistores
	PORTC_PCR7|=(1<<8);	//GPIO VENT1
	PORTC_PCR0|=(1<<8);	//GPIO VENT2
	PORTC_PCR3|=(1<<8);	//GPIO VENT3
	PORTC_PCR4|=(1<<8);	//GPIO VENT4
	PORTC_PCR5|=(1<<8);	//GPIO CELDA 1
	PORTC_PCR6|=(1<<8);	//GPIO CELDA 2
	GPIOC_PDDR|=(1<<7)+(1<<0)+(1<<3)+(1<<4)+(1<<5)+(1<<6);//OUTPUT todos los pines
	
	//RW va directo a tierra para escribir siempre
	PORTE_PCR30|=(1<<8);//GPIO RS
	PORTE_PCR29|=(1<<8);//GPIO EN
	PORTE_PCR23|=(1<<8);//GPIO D0
	PORTE_PCR22|=(1<<8);//GPIO D1
	PORTE_PCR21|=(1<<8);//GPIO D2
	PORTE_PCR20|=(1<<8);//GPIO D3
	PORTE_PCR5|=(1<<8);//GPIO D4
	PORTE_PCR4|=(1<<8);//GPIO D5
	PORTE_PCR3|=(1<<8);//GPIO D6
	PORTE_PCR2|=(1<<8);//GPIO D7
	GPIOE_PDDR|=(1<<30)+(1<<29)+(1<<23)+(1<<22)+(1<<21)+(1<<20)+(1<<5)+(1<<4)+(1<<3)+(1<<2);//OUTPUT todos los pines
}
void SysON(){
	GPIOC_PDOR|=(1<<7)+(1<<0)+(1<<3)+(1<<4)+(1<<5)+(1<<6);//ENCENDER
}
void SysOFF(){
	GPIOC_PDOR=0;
}
void delay(){
	//50ms aprox
	int i=0;
	volatile unsigned int j = 0;
	for (i;i<25;i++){
		for (j; j<10000; j++){
			1+1;
			
		}	
	}
}
void clearDisp(){
	delay();
	//siempre hacer un clear display antes de inicializar 0b00000001
	GPIOE_PDOR|=(1<<29);//Prender EN
	delay();//esperar antes de escribir comando
	GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
	GPIOE_PDOR|=(0<<2)+(0<<3)+(0<<4)+(0<<5)+(0<<20)+(0<<21)+(0<<22)+(1<<23);//clear disp
	delay();//esperar para apagar EN
	GPIOE_PDOR&= ~(1<<29);//apagar EN
	GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
	delay();
}
void LCD_init(){
		//recuerda que EN va en alto cuando se quiere escribir un comando y se apaga en cuanto se termina el comando
		//recuerda en RS debe de estar en 0 para indicar la escritura de un comando 
		
		//esperar a que prenda la pantalla
		delay();
		delay();
		delay();
		clearDisp();//limpia la pantalla antes de inicializar
		
		
		//primer comando 0b00110000, prender enable y apagarlo al final
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(0<<4)+(0<<5)+(0<<20)+(0<<21)+(0<<22)+(1<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(01<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//segundo comando 0b00110000, prender enable y apagarlo al final
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(0<<22)+(0<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//tercer comando 0b00110000, prender enable y apagarlo al final
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(0<<22)+(0<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//cuarto comando 0b00110000
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(0<<22)+(0<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//quinto comando 0b00001110
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(0<<4)+(0<<5)+(1<<20)+(1<<21)+(1<<22)+(0<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//sexto comando 0b00000110
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(0<<30);//DEJAR RS EN 0 PARA ESCRITURA DE COMANDOS POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(0<<4)+(0<<5)+(0<<20)+(1<<21)+(1<<22)+(0<<23);//ejecutar primer comando +(0<<30) que significa RS en 0 para indicar comando
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		
		//aqui termina la inicialización
		
		
		//espera un tiempo para escribir en la pantalla
		delay();
}
void Celsius(){
	//escribir simbolo grado
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(1<<2)+(1<<3)+(0<<4)+(1<<5)+(1<<20)+(1<<21)+(1<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir letra C
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(0<<4)+(0<<5)+(0<<20)+(0<<21)+(1<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
			
		//escribir letra e
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(0<<5)+(0<<20)+(1<<21)+(0<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir letra l
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(0<<5)+(1<<20)+(1<<21)+(0<<22)+(0<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
			
		//escribir letra s
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(1<<22)+(1<<23);////escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir letra i
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(0<<5)+(1<<20)+(0<<21)+(0<<22)+(1<<23);////escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir letra u
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(1<<5)+(0<<20)+(1<<21)+(0<<22)+(1<<23);////escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir letra s
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(1<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(1<<22)+(1<<23);////escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando

}
void escribir5Grados(){
			
	//escribir grado numerico
	GPIOE_PDOR|=(1<<29);//Prender EN
	delay();//esperar antes de escribir comando
	GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
	GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(1<<21)+(0<<22)+(1<<23);//escribir bits
	delay();//esperar para apagar EN
	GPIOE_PDOR&= ~(1<<29);//apagar EN
	GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
	
	Celsius();//complementar con simbolo y palabra
	
}
void escribir6Grados(){
	
		
	//escribir grado numerico
	GPIOE_PDOR|=(1<<29);//Prender EN
	delay();//esperar antes de escribir comando
	GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
	GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(1<<21)+(1<<22)+(0<<23);//escribir bits
	delay();//esperar para apagar EN
	GPIOE_PDOR&= ~(1<<29);//apagar EN
	GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
	
	Celsius();//complementar con simbolo y palabra
	
}
void escribir7Grados(){
	
		
		//escribir grado numerico
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(1<<21)+(1<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		Celsius();//complementar con simbolo y palabra
			
}
void escribir8Grados(){		
		//escribir grado numerico
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(1<<20)+(0<<21)+(0<<22)+(0<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		Celsius();//complementar con simbolo y palabra
			
}
void escribir9Grados(){		
		//escribir grado numerico
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(1<<20)+(0<<21)+(0<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		Celsius();//complementar con simbolo y palabra
			
}
void escribir10Grados(){		
		//escribir grado numerico (primero 1)
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(0<<22)+(1<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		//escribir grado numerico (segundo 0)
		GPIOE_PDOR|=(1<<29);//Prender EN
		delay();//esperar antes de escribir comando
		GPIOE_PDOR|=(1<<30);//DEJAR RS EN 1 PARA ESCRITURA DE caracter POR SEPARADO
		GPIOE_PDOR|=(0<<2)+(0<<3)+(1<<4)+(1<<5)+(0<<20)+(0<<21)+(0<<22)+(0<<23);//escribir bits
		delay();//esperar para apagar EN
		GPIOE_PDOR&= ~(1<<29);//apagar EN
		GPIOE_PDOR=0;//apagar todos los bits para el siguiente comando
		
		Celsius();//complementar con simbolo y palabra
			
}
void PORTA_IRQHandler()
{
	//////////INCREMENTAR//////////
	if (PORTA_PCR17 & (1<<24)){
		PORTA_PCR17=PORTA_PCR17|(1<<24);
		cont++;
		if (cont==1){
			clearDisp();
			escribir6Grados();
			temperatura=6;			
		}
		if (cont==2){
			clearDisp();
			escribir7Grados();
			temperatura=7;			
		}
		if (cont==3){
			clearDisp();
			escribir8Grados();
			temperatura=8;			
		}
		if (cont==4){
			clearDisp();
			escribir9Grados();
			temperatura=9;			
		}
		if (cont==5){
			clearDisp();
			escribir10Grados();
			temperatura=10;			
		}
		if (cont>5){
			cont=5;
			temperatura=10;
		}
	}
	//////////DECREMENTAR/////////
	if (PORTA_PCR16 & (1<<24)){
		PORTA_PCR16=PORTA_PCR16|(1<<24);
		cont--;
		if (cont==4){
			clearDisp();
			escribir9Grados();
			temperatura=9;			
		}
		if (cont==3){
			clearDisp();
			escribir8Grados();
			temperatura=8;			
		}
		if (cont==2){
			clearDisp();
			escribir7Grados();
			temperatura=7;			
		}
		if (cont==1){
			clearDisp();
			escribir6Grados();
			temperatura=6;			
		}
		if (cont==0){
			clearDisp();
			escribir5Grados();
			temperatura=5;			
		}
		if (cont<0){
			cont=0;
			temperatura=5;
		}
	}
}

int main(void)
{	
	clk_init();
	GPIO_init();
	LCD_init();
	escribir5Grados();
	NVIC_ISER=(1<<30);
	SysOFF();
	
	
	
	while(1);
	return 0;
}
