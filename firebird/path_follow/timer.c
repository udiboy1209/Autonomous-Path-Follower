unsigned long exec_time=0;

void timer_init(){
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
            
    // set compare match register to desired timer count:
    OCR1A = 15624; // Will interrupt on 1ms increment
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS10);

    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    exec_time += 1; // Increment execution time 
}
