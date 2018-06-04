/* 3pi_app2_pid - an application for the Pololu 3pi Robot
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 10/10/2017 11:47:57 AM
 *  Author: User
 */

/*
*
 * 3pi-linefollower-pid - demo code for the Pololu 3pi Robot
 *
 * This code will follow a black line on a white background, using a
 * PID-based algorithm.
 * http://www.pololu.com/docs/0J21
 * http://www.pololu.com
 * http://forum.pololu.com
 *
 */

// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <pololu/3pi.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

char path[100] = "";
unsigned char path_length = 0; // the length of the path
int flag=1;

unsigned int read_battery_millivolts_3pi()
{
	//return readAverage(6,10)*5000L*3/2/1023;
	return 0;
}


// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "PID Line";
const char demo_name_line2[] PROGMEM = "follower";

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";

// Data for generating the characters used in load_custom_characters
// and display_readings.  By reading levels[] starting at various
// offsets, we can generate all of the 7 extra characters needed for a
// bargraph.  This is also stored in program space.
const char levels[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

// This function loads custom characters into the LCD.  Up to 8
// characters can be loaded; we use them for 7 levels of a bar graph.
void load_custom_characters()
{
	lcd_load_custom_character(levels+0,0); // no offset, e.g. one bar
	lcd_load_custom_character(levels+1,1); // two bars
	lcd_load_custom_character(levels+2,2); // etc...
	lcd_load_custom_character(levels+3,3);
	lcd_load_custom_character(levels+4,4);
	lcd_load_custom_character(levels+5,5);
	lcd_load_custom_character(levels+6,6);
	clear(); // the LCD must be cleared for the characters to take effect
}

// This function displays the sensor readings using a bar graph.
void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;

	for(i=0;i<5;i++) {
		// Initialize the array of characters that we will use for the
		// graph.  Using the space, an extra copy of the one-bar
		// character, and character 255 (a full black box), we get 10
		// characters in the array.
		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

		// The variable c will have values from 0 to 9, since
		// calibrated values are in the range of 0 to 1000, and
		// 1000/101 is 9 with integer math.
		char c = display_characters[calibrated_values[i]/101];

		// Display the bar graph character.
		print_character(c);
	}
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	unsigned int counter; // used as a simple timer
	unsigned int sensors[5]; // an array to hold sensor values

	// This must be called at the beginning of 3pi code, to set up the
	// sensors.  We use a value of 2000 for the timeout, which
	// corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
	pololu_3pi_init(2000);
	load_custom_characters(); // load the custom characters
	
	// Play welcome music and display a message
	print_from_program_space(welcome_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(welcome_line2);
	play_from_program_space(welcome);
	delay_ms(500);

	clear();
	print_from_program_space(demo_name_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(demo_name_line2);
	delay_ms(1000);

	// Display battery voltage and wait for button press
	while(!button_is_pressed(BUTTON_A))
	{
		int bat = read_battery_millivolts();
		if(bat<=4600)
		{
			
			delay(500);
			
		}
		else{
		clear();
		print_long(bat);
		print("mV");
		lcd_goto_xy(0,1);
		print("Press B");
		}
		delay_ms(100);
	}

	// Always wait for the button to be released so that 3pi doesn't
	// start moving until your hand is away from it.
	wait_for_button_release(BUTTON_A);
	delay_ms(1000);

	// Auto-calibration: turn right and left while calibrating the
	// sensors.
	for(counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
			set_motors(40,-40);
		else
			set_motors(-40,40);

		// This function records a set of sensor readings and keeps
		// track of the minimum and maximum values encountered.  The
		// IR_EMITTERS_ON argument means that the IR LEDs will be
		// turned on during the reading, which is usually what you
		// want.
		calibrate_line_sensors(IR_EMITTERS_ON);

		// Since our counter runs to 80, the total delay will be
		// 80*20 = 1600 ms.
		delay_ms(20);
	}
	set_motors(0,0);

	// Display calibrated values as a bar graph.
	while(!button_is_pressed(BUTTON_B))
	{
		// Read the sensor values and get the position measurement.
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		// Display the position measurement, which will go from 0
		// (when the leftmost sensor is over the line) to 4000 (when
		// the rightmost sensor is over the line) on the 3pi, along
		// with a bar graph of the sensor readings.  This allows you
		// to make sure the robot is ready to go.
		clear();
		print_long(position);
		lcd_goto_xy(0,1);
		display_readings(sensors);

		delay_ms(100);
	}
	wait_for_button_release(BUTTON_B);

	clear();

	print("Go!");		

	// Play music and wait for it to finish before we start driving.
	play_from_program_space(go);
	while(is_playing());
}

void turn(char dir)
{
switch(dir)
{
	case 'L':
	// Turn left.
	set_motors(-80,80);
	delay_ms(200);
	break;
	case 'R':
	// Turn right.
	set_motors(80,-80);
	delay_ms(200);
	break;
	case 'B':
	// Turn around.
	set_motors(80,-80);
	delay_ms(400);
	break;
	case 'S':
	// Don't do anything!
	break;
}
}

void follow_segment()
{
	int last_proportional = 0;
	long integral=0;
	while(1)
	{
	// Get the position of the line.
	unsigned int sensors[5];
	unsigned int position = read_line(sensors,IR_EMITTERS_ON);
	// The "proportional" term should be 0 when we are on the line.
	int proportional = ((int)position) - 2000;
	// Compute the derivative (change) and integral (sum) of the
	// position.
	int derivative = proportional - last_proportional;
	integral += proportional;
	// Remember the last position.
	last_proportional = proportional;
	// Compute the difference between the two motor power settings,
	// m1 - m2. If this is a positive number the robot will turn
	// to the left. If it is a negative number, the robot will
	// turn to the right, and the magnitude of the number determines
	// the sharpness of the turn.
	int power_difference = proportional/20 + integral/10000 + derivative*3/2;
	// Compute the actual motor settings. We never set either motor
	// to a negative value.
	const int max = 60; // the maximum speed
	if(power_difference > max)
	power_difference = max;
	if(power_difference < -max)
	power_difference = -max;
	if(power_difference < 0)
	set_motors(max+power_difference,max);
	else
	set_motors(max,max-power_difference);
	// We use the inner three sensors (1, 2, and 3) for
	// determining whether there is a line straight ahead, and the
	// sensors 0 and 4 for detecting lines going to the left and
	// right.
	if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
	{
		// There is no line visible ahead, and we didn't see any
		// intersection. Must be a dead end.
		return;
	}
	else if(sensors[0] > 200 || sensors[4] > 200)
	{
		// Found an intersection.
		
		return;
	}
	
	}
	
}
 void follow()
 {
	 int last_proportional = 0;
	 long integral=0;
	 while(1)
	 {
		 // Normally, we will be following a line. The code below is
		 // similar to the 3pi-linefollower-pid example, but the maximum
		 // speed is turned down to 60 for reliability.
		 // Get the position of the line.
		 unsigned int sensors[5];
		 unsigned int position = read_line(sensors,IR_EMITTERS_ON);
		 // The "proportional" term should be 0 when we are on the line.
		 int proportional = ((int)position) - 2000;
		 // Compute the derivative (change) and integral (sum) of the
		 // position.
		 int derivative = proportional - last_proportional;
		 integral += proportional;
		 // Remember the last position.
		 last_proportional = proportional;
		 // Compute the difference between the two motor power settings,
		 // m1 - m2. If this is a positive number the robot will turn
		 // to the left. If it is a negative number, the robot will
		 // turn to the right, and the magnitude of the number determines
		 // the sharpness of the turn.
		 int power_difference = proportional/20 + integral/10000 + derivative*4;
		 // Compute the actual motor settings. We never set either motor
		 // to a negative value.
		 const int max = 140; // the maximum speed
		 if(power_difference > max)
		 power_difference = max;
		 if(power_difference < -max)
		 power_difference = -max;
		 if(power_difference < 0)
		 set_motors(max+power_difference,max);
		 else
		 set_motors(max,max-power_difference);
		 // We use the inner three sensors (1, 2, and 3) for
		 // determining whether there is a line straight ahead, and the
		 // sensors 0 and 4 for detecting lines going to the left and
		 // right.
		 if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
		 {set_motors(0,0);
			 // There is no line visible ahead, and we didn't see any
			 // intersection. Must be a dead end.
			 return;
			 
		 }
		 else if(sensors[0] > 200 || sensors[4] > 200)
		 {set_motors(0,0);
			 // Found an intersection.
			 return;
			 
		 }
		 else if(sensors[0] > 100 && sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200 && sensors[4] > 100){
			 
			 set_motors(0,0);
			 flag=0;
			 break;
		 }
		 
	 }
	 
 }


char select_turn(unsigned char found_left, unsigned char found_straight,unsigned char found_right)
{
	// Make a decision about how to turn. The following code
	// implements a left-hand-on-the-wall strategy, where we always
	// turn as far to the left as possible.
	if(found_left)
	return 'L';
	else if(found_straight)
	return 'S';
	else if(found_right)
	return 'R';
	else
	return 'B';
}

void simplify_path()
{

	// only simplify the path if the second-to-last turn was a 'B'
	if(path_length < 3 || path[path_length-2] != 'B')
	return;
	int total_angle = 0;
	int i;
	for(i=1;i<=3;i++)
	{
		switch(path[path_length-i])
		{
			case 'R':
			total_angle += 90;
			break;
			case 'L':
			total_angle += 270;
			break;
			case 'B':
			total_angle += 180;
			break;
		}
	}
	// Get the angle as a number between 0 and 360 degrees.
	total_angle = total_angle % 360;
	// Replace all of those turns with a single one.
	switch(total_angle)
	{
		case 0:
		path[path_length - 3] = 'S';
		break;
		case 90:
		path[path_length - 3] = 'R';
		break;
		case 180:
		path[path_length - 3] = 'B';
		break;
		case 270:
		path[path_length - 3] = 'L';
		break;
	}
	// The path is now two steps shorter.
	path_length -= 2;
}

// This is the main function, where the code starts.  All C programs
// must have a main() function defined somewhere.

int main()
{
 initialize();
while(1)
{
	// FIRST MAIN LOOP BODY
	follow_segment();
	
	// Drive straight a bit.  This helps us in case we entered the
	// intersection at an angle.
	// Note that we are slowing down - this prevents the robot
	// from tipping forward too much.
	set_motors(50,50);
	delay_ms(50);
	
	// These variables record whether the robot has seen a line to the
	// left, straight ahead, and right, whil examining the current
	// intersection.
	unsigned char found_left=0;
	unsigned char found_straight=0;
	unsigned char found_right=0;
	
	// Now read the sensors and check the intersection type.
	unsigned int sensors[5];
	read_line(sensors,IR_EMITTERS_ON);
	
	// Check for left and right exits.
	if(sensors[0] > 100)
	found_left = 1;
	if(sensors[4] > 100)
	found_right = 1;
	
	// Drive straight a bit more - this is enough to line up our
	// wheels with the intersection.
	set_motors(40,40);
	delay_ms(200);
	
	// Check for a straight exit.
	read_line(sensors,IR_EMITTERS_ON);
	if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
	found_straight = 1;
	
	// Check for the ending spot.
	// If all three middle sensors are on dark black, we have
	// solved the maze.
	if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
	{
	 break; 
	 }
	
	
	// Intersection identification is complete.
	// If the maze has been solved, we can follow the existing
	// path.  Otherwise, we need to learn the solution.
	unsigned char dir = select_turn(found_left, found_straight, found_right);
	
	// Make the turn indicated by the path.
	turn(dir);
	
	// Store the intersection in the path variable.
	path[path_length] = dir;
	path_length ++;
	
	// You should check to make sure that the path_length does not
	// exceed the bounds of the array.  We'll ignore that in this
	// example.
	
	// Simplify the learned path.
	simplify_path();
	
	
}
for(int i=0;i<4;i++){set_motors(50,50);delay(50);}
while(!button_is_pressed(BUTTON_B))
{
	set_motors(0,0);
}
delay(1000);
// Now enter an infinite loop - we can re-run the maze as many
// times as we want to.
while(1)
{
	// Beep to show that we finished the maze.
	// Wait for the user to press a button...
	
	int i;
	for(i=0;i<path_length;i++)
	{
		// SECOND MAIN LOOP BODY
		follow_segment();
		
		// Drive straight while slowing down, as before.
		set_motors(50,50);
		delay_ms(50);
		set_motors(40,40);
		delay_ms(200);
		
		// Make a turn according to the instruction stored in
		// path[i].
		turn(path[i]);
	}
	
	// Follow the last segment up to the finish.
	follow_segment();
	for(int i=0;i<7;i++){set_motors(50,50);delay(50);}
	// Now we should be at the finish!  Restart the loop.
	while(!button_is_pressed(BUTTON_B))
	{
		set_motors(0,0);
	}
}



	
		return 0;
		}
