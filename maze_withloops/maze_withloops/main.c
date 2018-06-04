#include <pololu/3pi.h>
#include <stdio.h>
#include <avr/pgmspace.h>
int i,j,ind=0;
int bx=17,by=17;
int mat[20][28];
int arrayX[30];
int arrayY[30];
char array[30];
unsigned int read_battery_millivolts_3pi()
{
	//return readAverage(6,10)*5000L*3/2/1023;
	return 0;
}
const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "PID Line";
const char demo_name_line2[] PROGMEM = "follower";
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
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
void simplify_path()
{

	if(ind < 3 || array[ind-2] != 'B')
	return;
	int total_angle = 0;
	for(i=1;i<=3;i++)
	{
		switch(array[ind-i])
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

	total_angle = total_angle % 360;

	switch(total_angle)
	{
		case 0:
		array[ind - 3] = 'S';
		break;
		case 90:
		array[ind - 3] = 'R';
		break;
		case 180:
		array[ind - 3] = 'B';
		break;
		case 270:
		array[ind - 3] = 'L';
		break;
	}

	ind -= 2;
}
void init(){
	for( i=0;i<26;i++)
	{
		for( j=0;j<34;j++)
		{
			mat[i][j]=0;
		}
	}
}


void move(char dir)
{  array[ind]=dir;
	if(dir=='S')
	{
		if(bx==17 && by==17)
		{
			mat[bx][by]=mat[bx][by]+1;
			arrayX[ind]=bx; arrayY[ind]=by;
			bx--; ind++;
			mat[bx][by]=mat[bx][by]+1;
			bx--;
			
		}
		else
		{
			if(arrayX[ind-1]==bx)
			{
				if(arrayY[ind-1]<by)
				{
					mat[bx][by]=mat[bx][by]+1;
					arrayX[ind]=bx; arrayY[ind]=by;
					by++; ind++;
					mat[bx][by]=mat[bx][by]+1;
					by++;
				}
				else
				{
					mat[bx][by]=mat[bx][by]+1;
					arrayX[ind]=bx; arrayY[ind]=by;
					by--; ind++;
					mat[bx][by]=mat[bx][by]+1;
					by--;
				}
			}
			else
			{
				if(arrayX[ind-1]<bx)
				{
					mat[bx][by]=mat[bx][by]+1;
					arrayX[ind]=bx; arrayY[ind]=by;
					bx++; ind++;
					mat[bx][by]=mat[bx][by]+1;
					bx++;
				}
				else
				{
					mat[bx][by]=mat[bx][by]+1;
					arrayX[ind]=bx; arrayY[ind]=by;
					bx--; ind++;
					mat[bx][by]=mat[bx][by]+1;
					bx--;
				}
			}
		}
	}
	else if(dir=='R')
	{
		if(arrayY[ind-1]==by)
		{
			if(arrayX[ind-1]<bx)
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				by--; ind++;
				mat[bx][by]=mat[bx][by]+1;
				by--;
			}
			else
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				by++; ind++;
				mat[bx][by]=mat[bx][by]+1;
				by++;
			}
		}
		else
		{
			if(arrayY[ind-1]<by)
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				bx++; ind++;
				mat[bx][by]=mat[bx][by]+1;
				bx++;
			}
			else
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				bx--; ind++;
				mat[bx][by]=mat[bx][by]+1;
				bx--;
			}
		}
	}
	else if(dir=='L')
	{
		if(arrayY[ind-1]==by)
		{
			if(arrayX[ind-1]<bx)
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				by++; ind++;
				mat[bx][by]=mat[bx][by]+1;
				by++;
			}
			else
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				by--; ind++;
				mat[bx][by]=mat[bx][by]+1;
				by--;
			}
		}
		else
		{
			if(arrayY[ind-1]<by)
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				bx--; ind++;
				mat[bx][by]=mat[bx][by]+1;
				bx--;
			}
			else
			{
				mat[bx][by]=mat[bx][by]+1;
				arrayX[ind]=bx; arrayY[ind]=by;
				bx++; ind++;
				mat[bx][by]=mat[bx][by]+1;
				bx++;
			}
		}
	}
	else
	{
		if(by==arrayY[ind-1])
		{
			if(bx<arrayX[ind-1])
			{
				arrayX[ind]=bx; arrayY[ind]=by;
				ind++;
				mat[bx][by]++;
				bx++;
				mat[bx][by]++;
				bx++;
				by=arrayY[ind-1];
				
			}
			else
			{
				arrayX[ind]=bx; arrayY[ind]=by;
				ind++;
				mat[bx][by]++;
				bx--;
				mat[bx][by]++;
				bx--;
				by=arrayY[ind-1];
				
			}
		}
		else
		{
			if(by>arrayY[ind-1])
			{
				arrayX[ind]=bx; arrayY[ind]=by;
				ind++;
				mat[bx][by]++;
				by--;
				mat[bx][by]++;
				by--;
				bx=arrayX[ind-1];
				
			}
			else
			{
				arrayX[ind]=bx; arrayY[ind]=by;
				ind++;
				mat[bx][by]++;
				by++;
				mat[bx][by]++;
				by++;
				bx=arrayX[ind-1];
				
			}
		}
	}
}
char direction(int l,int r,int s)
{ int lval=-1,rval=-1,sval=-1;
	if(s==1)
	{
		if(bx==17 && by==17)
		sval=0;
		else
		{
			if(arrayX[ind-1]==bx)
			{
				if(arrayY[ind-1]<by)
				sval=mat[bx][by+1];
				else
				sval=mat[bx][by-1];
			}
			else
			{
				if(arrayX[ind-1]<bx)
				sval=mat[bx+1][by];
				else
				sval=mat[bx-1][by];
			}
		}
	}
	if(r==1)
	{
		if(arrayY[ind-1]==by)
		{
			if(arrayX[ind-1]<bx)
			rval=mat[bx][by-1];
			else
			rval=mat[bx][by+1];
		}
		else
		{
			if(arrayY[ind-1]<by)
			rval=mat[bx+1][by];
			else
			rval=mat[bx-1][by];
		}
	}
	if(l==1)
	{
		if(arrayY[ind-1]==by)
		{
			if(arrayX[ind-1]<bx)
			lval=mat[bx][by+1];
			else
			lval=mat[bx][by-1];
		}
		else
		{
			if(arrayY[ind-1]<by)
			lval=mat[bx-1][by];
			else
			lval=mat[bx+1][by];
		}
	}
	if(l==1)
	{
		if(r==1){
			if(rval>=lval)
			{
				if(s==1){
					if(sval>=lval){move('L');return 'L';}
					else{move('S');return 'S';}
				}
				else{move('L');return 'L';}
			}
			else
			{
				if(s==1){
					if(sval>=lval){move('R');return 'R';}
					else{move('S');return 'S';}
				}
				else{move('R');return 'R';}
			}
		}
		else
		{
			if(s==1){
				if(sval>=lval){move('L');return 'L';}
				else{move('S');return 'S';}
			}
			else
			{
				move('L');return 'L';
			}
		}
	}
	else if(r==1)
	{
		if(s==0){move('R');return 'R';}
		else{if(sval>=rval){move('R');return 'R';}}
	}
	else if(s==1)
	{
		move('S');return 'S';
	}
	else
	{
		move('B');
		return 'B';
	}
}

void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;

	for(i=0;i<5;i++) {

		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

		char c = display_characters[calibrated_values[i]/101];


		print_character(c);
	}
}

void fin()
{
	mat[bx][by]=5;
}
void follow_segment()
{
	int last_proportional = 0;
	long integral=0;
	while(1)
	{

		unsigned int sensors[5];
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		int proportional = ((int)position) - 2000;

		int derivative = proportional - last_proportional;
		integral += proportional;

		last_proportional = proportional;

		int power_difference = proportional/20 + integral/10000 + derivative*3/2;

		const int max = 60; // the maximum speed
		if(power_difference > max)
		power_difference = max;
		if(power_difference < -max)
		power_difference = -max;
		if(power_difference < 0)
		set_motors(max+power_difference,max);
		else
		set_motors(max,max-power_difference);

		if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
		{

			return;
		}
		else if(sensors[0] > 200 || sensors[4] > 200)
		{

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

		unsigned int sensors[5];
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		int proportional = ((int)position) - 2000;

		int derivative = proportional - last_proportional;
		integral += proportional;
		last_proportional = proportional;

		int power_difference = proportional/20 + integral/10000 + derivative*4;

		const int max = 140; // the maximum speed
		if(power_difference > max)
		power_difference = max;
		if(power_difference < -max)
		power_difference = -max;
		if(power_difference < 0)
		set_motors(max+power_difference,max);
		else
		set_motors(max,max-power_difference);

		if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
		{set_motors(0,0);
			return;
			
		}
		else if(sensors[0] > 200 || sensors[4] > 200)
		{set_motors(0,0);
			return;
			
		}
		else if(sensors[0] > 100 && sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200 && sensors[4] > 100){
			
			set_motors(0,0);

			break;
		}
		
	}
	
}
void turn(char dir){
	switch(dir){
		case 'L':
		set_motors(-80,80);
		delay_ms(200);
		break;
		case 'R':
		set_motors(80,-80);
		delay_ms(200);
		break;
		case 'B':
		set_motors(80,-80);
		delay_ms(400);
		break;
		case 'S':
	break;}}
	void initialize(){
		unsigned int counter; // used as a simple timer
		unsigned int sensors[5]; // an array to hold sensor values
		pololu_3pi_init(2000);
		load_custom_characters(); // load the custom characters
		lcd_goto_xy(0,1);
		delay_ms(500);
		clear();
		lcd_goto_xy(0,1);
		delay_ms(1000);
		while(!button_is_pressed(BUTTON_A)){}
		wait_for_button_release(BUTTON_A);
		delay_ms(1000);
		for(counter=0;counter<80;counter++){
			if(counter < 20 || counter >= 60)
			set_motors(40,-40);
			else
			set_motors(-40,40);
			calibrate_line_sensors(IR_EMITTERS_ON);
			delay_ms(20);
		}
		set_motors(0,0);
		while(!button_is_pressed(BUTTON_B)){
			unsigned int position = read_line(sensors,IR_EMITTERS_ON);
			clear();
			print_long(position);
			lcd_goto_xy(0,1);
			display_readings(sensors);
			delay_ms(100);
		}
		wait_for_button_release(BUTTON_B);
		clear();
		print("Go!");
	}
	int main(void) {
		float start, finished;
		init();
		initialize();
		start=millis();
		while(1){
			follow_segment();
			set_motors(50,50);
			delay_ms(50);
			unsigned char found_left=0;
			unsigned char found_straight=0;
			unsigned char found_right=0;
			unsigned int sensors[5];
			read_line(sensors,IR_EMITTERS_ON);
			if(sensors[0] > 100)
			found_left = 1;
			if(sensors[4] > 100)
			found_right = 1;
			set_motors(40,40);
			delay_ms(200);
			read_line(sensors,IR_EMITTERS_ON);
			if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
			found_straight = 1;
			if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
			break;
			char dir=direction(found_left, found_right,found_straight);
			turn(dir);
			simplify_path();
			finished=millis();
			clear();
			long elapsed=(long)(start-finished)/450;
			print_long(elapsed);
			int num=elapsed/450;
			while(num>1){
				direction(0,0,1);
				simplify_path();
				num--;
			}

			start=millis();
		}
		fin();
		clear();
		for(i=0;i<ind;i++){
		print_character(array[i]);}
		for(int i=0;i<4;i++){set_motors(50,50);delay(50);}
		while(!button_is_pressed(BUTTON_B))
		{
			set_motors(0,0);
		}
		delay(1000);
		while(1)
		{

			int i;
			for(i=0;i<ind;i++)
			{

				follow_segment();
				
				set_motors(50,50);
				delay_ms(50);
				set_motors(40,40);
				delay_ms(200);

				turn(array[i]);
			}
			

			follow_segment();
			for(int i=0;i<7;i++){set_motors(50,50);delay(50);}

			while(!button_is_pressed(BUTTON_B))
			{
				set_motors(0,0);
			}
		}
		
		return 0;
	}