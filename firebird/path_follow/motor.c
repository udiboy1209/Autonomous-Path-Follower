// Constant definitions for wheel direction control
#define FORWARD 0x06
#define BACK 0x09
#define STOP 0x00
#define RIGHT 0x0A
#define SOFT_RIGHT 0x02
#define LEFT 0x05
#define SOFT_LEFT 0x04

#define CLOCKWISE 1
#define ANTICLOCKWISE -1

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning

void motor_pin_config (void)
{
    //Direction config pins PA0 PA1 PA2 PA3
    DDRA = DDRA | 0x0F; //Setting PORTA 0,1,2,3 as output

    //Init with forward configuration 0110
    PORTA = PORTA & 0xF6; //Setting PORTA 0,3 to low ( LB RB low)
    PORTA = PORTA | 0x06; //Setting PORTA 1,2 to high ( LH RH high)

    //Motor PWM velocity control pins PL3 PL4
    DDRL = DDRL | 0x18; //Setting PORTL 3,4, as output
    PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM

}
  
// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
    unsigned char PortARestore = 0;

    Direction &= 0x0F; 			// removing upper nibbel as it is not needed
    PortARestore = PORTA; 			// reading the PORTA's original status
    PortARestore &= 0xF0; 			// setting lower direction nibbel to 0
    PortARestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTA status
    PORTA = PortARestore; 			// setting the command to the port
}

void velocity (int left_motor, int right_motor)
{
    if(left_motor < 0){
        motion_set(LEFT);
        left_motor=-left_motor;
    }
    if(right_motor < 0){
        motion_set(RIGHT);
        right_motor=-right_motor;
    }

	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config()
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config()
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 5 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 5 pin
}

void left_position_encoder_interrupt_init() //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init() //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

void position_encoder_init()
{
}

void motor_init (void)    
{
    motor_pin_config();
    timer5_init();

	//Position encoder init
	left_encoder_pin_config();
	right_encoder_pin_config();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}

//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}


// rotation_resolution = distance moved by wheel on 1 degree HARD turn
const double rotation_resolution=4.090*5.44/5.338; //default 4.090 given


// Function used for turning robot by specified degrees
// Robot will do a SOFT turn (one wheel moving forward, one wheel stopped)
// based on direction ( CLOCKWISE->RIGHT | ANTICLOCKWISE->LEFT )
double angle_soft(double Degrees, int direction)
{
	if(direction==CLOCKWISE) motion_set(SOFT_RIGHT);
	else motion_set(SOFT_LEFT);
	
	double ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

    // division by resolution to get shaft count
    // rotation_resolution halved because in SOFT turn 
    // distance moved will be 1/2 of HARD turn
	ReqdShaftCount = Degrees/(rotation_resolution/2); 
    ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;
	
	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt && direction == ANTICLOCKWISE) |
                (ShaftCountLeft >= ReqdShaftCountInt && direction == CLOCKWISE))
            break;
	}
	motion_set(STOP); //Stop robot
	
    // Return error in angle turned ( Due to integer truncation inaccuracy )
	return Degrees-ReqdShaftCountInt*rotation_resolution;
}

// Robot will do a HARD turn (one wheel moving forward, one wheel moving backward)
// based on direction ( CLOCKWISE->RIGHT | ANTICLOCKWISE->LEFT )
double angle_hard(double Degrees, int direction)
{
	if(direction==CLOCKWISE) motion_set(RIGHT);
	else motion_set(LEFT);
	
	double ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = Degrees/rotation_resolution; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;
	
	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt && direction == ANTICLOCKWISE) |
                (ShaftCountLeft >= ReqdShaftCountInt && direction == CLOCKWISE))
            break;
	}
	motion_set(STOP); //Stop robot
	
    // Return error in angle turned ( Due to integer truncation inaccuracy )
	return Degrees-ReqdShaftCountInt*rotation_resolution;
}

//Function used for moving robot forward by specified distance
double forward_mm(double DistanceInMM)
{
	motion_set(FORWARD);
	
	double ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = DistanceInMM / 5.44; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
	
	ShaftCountRight = 0;
	while(1)
	{
        // Print sharp sensor distance value for debug
		lcd_print(2,1,sharp_distance(SHARP_N),5);
		
		if(ShaftCountRight > ReqdShaftCountInt) // complete distance covered
		{
			return 0;
		}
		
		if(foundObstacle()){ // Found obstacle in between. Start avoid sequence
            // Return the distance currently covered
			return ShaftCountRight*0.544;
		}
	}
	motion_set(STOP); //Stop robot
}
