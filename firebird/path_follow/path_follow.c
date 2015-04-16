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


//Move on an arc of radius for theta degrees
void trace_arc(double radius, double theta, int direction, int detect){
	theta=theta*PI/180;
	
    double sweep=theta/5;
	
	double distance = 2*radius*sin(sweep/2), completed, angle_completed;
		
	//lcd_print(1,1,distance*100,4);
	for(int i=0; i<5; i++){
		angle_soft(sweep*180/PI, direction);
		
		completed = forward_mm((distance)*10);
		
		if(detect && completed > 0){
			angle_completed=(i+completed/distance)*sweep + 2*atan(SHARP_THRESHOLD/10/radius);
			i=angle_completed/sweep;
			
			lcd_print(2,11,i,1);
			
			angle_hard(90,direction);
			trace_arc(SHARP_THRESHOLD/10,2*atan(radius/SHARP_THRESHOLD*10)*180/PI, -direction, 0);
			angle_hard(90,direction);
			
			forward_mm(distance-angle_completed+i*sweep);
		}
	}
	angle_soft(30,direction);
	/*velocity(255,(int) (255*(radius-7.5)/(radius+7.5)));
    
    double theta_done=0;
    exec_time=0;

    while(theta_done < theta ){
        theta_done = 24*exec_time/(radius+7.5)/1000;

        PORTJ = 0x01;

		lcd_print(1,1,exec_time,5);
    }*/

    //left_distance=0;
    //right_distance=0;

    motion_set(STOP);
}

int main()
{
    init_devices();

    motion_set(STOP);
	
	int arc_done=0;
    while(1){
		
		if(arc_done<arc_count){
			lcd_print(1,1,arc_done,1);
			
			unsigned int radius = bytes[arc_done*9 + 0]*(1<<24)
								+bytes[arc_done*9 + 1]*(1<<16)
								+bytes[arc_done*9 + 2]*(1<<8)
								+bytes[arc_done*9 + 3]*(1<<0),
								
						 theta = bytes[arc_done*9 + 4]*(1<<24)
								+bytes[arc_done*9 + 5]*(1<<16)
								+bytes[arc_done*9 + 6]*(1<<8)
								+bytes[arc_done*9 + 7]*(1<<0);
								
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
	
	trace_arc(50,90,CLOCKWISE,1);
	trace_arc(50,90,ANTICLOCKWISE,1);
	
    return 0;
}
