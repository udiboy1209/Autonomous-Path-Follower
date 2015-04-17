#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.c"
#include "sensor.c"
#include "motor.c"
//#include "timer.c"
#include "xbee.c"

#define PI 3.141592

void LED_bargraph_config (void)
{
     DDRJ = 0xFF;  //PORT J is configured as output
     PORTJ = 0x00; //Output is set to 0
}

// Initialize each device to be used ( PORT values, DDR values, etc. )
void init_devices()
{
    cli();
    lcd_init();
    motor_init();
    sensor_init();
    //timer_init();
	uart0_init();
    LED_bargraph_config();
    sei();
}


//Move on an arc of radius cm for theta degrees 
//detect is a boolean used to swtich on/off obstacle detection

// Due to limitation of PWM
// we are currently breaking path into 5 segments
// and each segment is traveresed by moving on straight line 
// for the chord length distance and rotating theta/5 degrees everytime
void trace_arc(double radius, double theta, int direction, int detect){
	theta=theta*PI/180;
	
    double sweep=theta/5;
	
	double distance = 2*radius*sin(sweep/2), completed, angle_completed;
		
	for(int i=0; i<5; i++){
        // For each iteration, rotate sweep angle
		angle_soft(sweep*180/PI, direction);
		
		completed = forward_mm((distance)*10); // Returns positive distance of obstacle detected else 0;
		
		if(detect && completed > 0){ // If should detect obstacles, and obstacle detected
            // angle_completed is the angle of the arc 
            // covered by bot after avoiding obstacle
			angle_completed=(i+completed/distance)*sweep + 2*atan(SHARP_THRESHOLD/10/radius); 

            // i is the segment of the arc 
            // the bot should be on after avoiding obstacle
			i=angle_completed/sweep;
			
			angle_hard(90,direction); // Align perpendicluar to obstacle

            // trace another arc around the obstalce 
            // of radius THRESHOLD and theta as calculated
			trace_arc(SHARP_THRESHOLD/10,2*atan(radius/SHARP_THRESHOLD*10)*180/PI, -direction, 0); 

            // Align to the path again
			angle_hard(90,direction);
			
            // Cover remaining distance of i'th segment
			forward_mm(distance-(angle_completed-i*sweep)*radius);
		}
	}
    motion_set(STOP);
}

int main()
{
    init_devices();

    motion_set(STOP);
	
    // No. of arcs covered by bot
	int arc_done=0;

    // Main loop
    // Waits for bytes[] array to fill up with xbee data
    // xbee data consists of a series of packets
    // each packet is 9 bytes in size
    // first 4 bytes is an integer defining radius
    // next 4 bytes is an integer defining theta in degrees
    // last byte is 'a' or 'c', defining ANTICLOCKWISE or CLOCKWISE direction
    while(1){
		
        // If less than arcs recieved from xbee
        // then run trace_arc
		if(arc_done<arc_count){
			lcd_print(1,1,arc_done,1);

            // Radius is calculated as integer using 1st 4 bytes of the packet			
            // Radius is obtained in cm
			unsigned int radius = bytes[arc_done*9 + 0]*(1<<24)
								+bytes[arc_done*9 + 1]*(1<<16)
								+bytes[arc_done*9 + 2]*(1<<8)
								+bytes[arc_done*9 + 3]*(1<<0),
								
            // Theta is calculated as integer using next 4 bytes of packet
            // Theta is obtained in degrees
						 theta = bytes[arc_done*9 + 4]*(1<<24)
								+bytes[arc_done*9 + 5]*(1<<16)
								+bytes[arc_done*9 + 6]*(1<<8)
								+bytes[arc_done*9 + 7]*(1<<0);
								
            // Direction is set using last (9th) byte of packet
			int direction;
			if(bytes[arc_done*9+8]=='c')
				direction=CLOCKWISE;
			else
				direction=ANTICLOCKWISE;	
			
			lcd_print(1,1,radius,3);
			lcd_print(1,5,theta,3);
			lcd_print(1,9,bytes[arc_done*9+8],2);	
			
			trace_arc(radius,theta,direction,1);
			
			arc_done++;
		}
		
		PORTJ=0x00;
		
		if(IR_reading(IR_W)<IR_THRESHOLD)
			PORTJ|=1<<0;
		if(IR_reading(IR_NW)<IR_THRESHOLD)
			PORTJ|=1<<1;
		if(IR_reading(IR_N)<IR_THRESHOLD)
			PORTJ|=1<<2;
		if(IR_reading(IR_NE)<IR_THRESHOLD)
			PORTJ|=1<<3;
		if(IR_reading(IR_E)<IR_THRESHOLD)
			PORTJ|=1<<4;
			
	}
	
    return 0;
}
