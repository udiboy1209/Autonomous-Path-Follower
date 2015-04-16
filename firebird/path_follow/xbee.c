//Function To Initialize UART0
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled

unsigned char bytes[126], data;
unsigned int arc_count=0, byte_count=0;

void uart0_init(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	// UBRR0L = 0x47; //11059200 Hz
	UBRR0L = 0x5F; // 14745600 Hzset baud rate lo
	UBRR0H = 0x00; //set baud rate hi
	UCSR0B = 0x98;
}

SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	data = UDR0;
	
	bytes[arc_count*9+byte_count]=data;
	byte_count++;
	
	if(byte_count==9){
		arc_count++;
		byte_count=0;
	}
	
	if(arc_count==14){
		arc_count=0;
	}
	
	UDR0 = data;	
	_delay_ms(100);
}	
