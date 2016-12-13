README
Harsh Aurora, 1st Oct 2016
MicroProcessor Systems, Fall 2016, Lab 2

This project contains example code on initializing the System clock, System timer Systick, an ADC and a GPIO pin

System Clock and SysTick: Refer to file sysclk_config.c
	Understand how the system clock is set to 168 MHz by studying the clock tree diagram in the tutorial slides 
	and/or datasheet
	Systick is configured to timout every 10 ms (100 Hz). For the purpose of Lab 2, you will need to figure out
	how to change this so that it times out every 1 ms (1000 Hz)
	
GPIO: Refer to file gpio_example.c
	Pin D12 is initialized and toggled. This pin is connected to the on board Green LED.
	
ADC: Refer to file adc_example.c
	ADC1 is initialized with input coming from Channel 5. This channel is connected to GPIO Pin A5. Therefore, 
	Pin A5 must be initialized in Analog mode as well. To see change in the ADC voltage displayed, connect a 
	female-to-male jumper cable to pin A5 and lightly tough the exposed metal with your finger. BE CAREFUL.
	
Main Program: Refer to file main.c
	All the individual components are initialzed, and the gpio and adc example code are run at intervals of 1 sec