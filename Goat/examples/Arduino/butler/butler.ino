#include <ht1632c.h>
#include <IRremote.h>
#include <RCSwitch.h>

#define RECV_PIN  			4
#define TX_PIN    			5

#define TDEBUG
#define STATE_NORMAL 	0
#define STATE_ESCAPE 	1
#define STATE_CSI			2
#define STATE_EXT1  		3
#define STATE_EXT2  		4
#define TERM_ESC			0x1b
#define TERM_CR			13
#define TERM_LF				10
#define TERM_TAB			'\t'
#define TERM_BACK		'\b'
#define VROW            		10
#define VCOL            		30

#define IR_POWER 		0xFD00FF
#define IR_VOL_PLUS 	0xFD807F
#define IR_FUNC_STP 	0xFD40BF
#define IR_REWIND 	0xFD20DF
#define IR_PLAY			0xFDA05F
#define IR_FASTF		 	0xFD609F
#define IR_DOWN	 	0xFD10EF
#define IR_VOL_MIN 	0xFD906F
#define IR_UP			 	0xFD50AF
#define IR_NUM_0    	0xFD30CF
#define IR_EQ		    	0xFDB04F
#define IR_ST_REPT    	0xFD708F
#define IR_NUM_1    	0xFD08F7
#define IR_NUM_2    	0xFD8877
#define IR_NUM_3    	0xFD48B7
#define IR_NUM_4    	0xFD28D7
#define IR_NUM_5    	0xFDA857
#define IR_NUM_6    	0xFD6897
#define IR_NUM_7    	0xFD18E7
#define IR_NUM_8    	0xFD9867
#define IR_NUM_9    	0xFD58A7


RCSwitch mySwitch 	= RCSwitch();
ht1632c 	ledMatrix 	= ht1632c(&PORTD, 2, 3, 6, 7, GEOM_32x16, 2);

IRrecv irrecv(RECV_PIN);
decode_results results;
char     Vchar[VROW][VCOL];
uint8_t  Vcolor[VROW][VCOL];
uint8_t  Vx;
uint8_t  Vy;
uint8_t  cursor_x;
uint8_t  cursor_y;
uint8_t  color;
bool 	scroll;
bool     cursor;
char 	state;

   
void setup() {
	Serial.begin(115200);
	irrecv.enableIRIn(); // Start the receiver
	irrecv.blink13(true);
	// Transmitter is connected to Arduino Pin #5  
	mySwitch.enableTransmit(TX_PIN);
	ledMatrix.clear();
	Vx			=	0;
	Vy			=	0;
	cursor_x	=	0;
	cursor_y	=	0;
	color		=	GREEN;
	state 	= STATE_NORMAL;
	scroll	=	false;
	cursor	= true;
	for (uint8_t y=0; y<VROW;y++) {
		for (uint8_t x=0; x<VCOL; x++) {
			Vchar[y][x] = 0;
			Vcolor[y][x] = BLACK;
		}
	}
}

void term (int ch) {
  	static int number = 0;
	static int num[3];

    if (ch == TERM_ESC) {
		state = STATE_ESCAPE;
		number = 0;
	} else if (state == STATE_ESCAPE  && ch == '[') {
		state = STATE_CSI;
		number = 0;
	} else if (state == STATE_CSI && ch == 'K') {
	    // Erase Line
		state = STATE_NORMAL;
		number = 0;
		for (int x=cursor_x; x<VCOL; x++) {
			Vchar[cursor_y][x] = 0;
			Vcolor[cursor_y][x] = BLACK;
		}
	} else if ((state == STATE_CSI || state == STATE_EXT1 || state == STATE_EXT2)
	        && ch >= '0' && ch <= '9') {
		number = (10 * number) + ch -'0';
	} else if (state == STATE_CSI && number == 2 && ch == 'J') { 
		// Clear Screen
		number = 0;
		state = STATE_NORMAL;
		for (uint8_t y=0; y<VROW;y++) {
			for (uint8_t x=0; x<VCOL; x++) {
				Vchar[y][x] = 0;
				Vcolor[y][x] = BLACK;
			}
		}
		cursor_x=0;
		cursor_y=0;
		Vx=0;
		Vy=0;
	} else if (state == STATE_CSI && number > 0 && ch == 'A') {
		// Move cursor up "number" lines
		state = STATE_NORMAL;
		if ((cursor_y - number) >=  0)
			cursor_y -= number;
		else
			cursor_y = 0;
		number = 0;
		// Move cursor down "number" lines
		state = STATE_NORMAL;
		if ((cursor_y + number) <  VROW)
			cursor_y += number;
		else
			cursor_y = VROW-1;
		number = 0;
	} else if (state == STATE_CSI && number > 0 && ch == 'C') {
		// Move cursor right "number" lines
		state = STATE_NORMAL;
		if ((cursor_x + number) < VCOL)
			cursor_y += number;
		else
			cursor_y = VCOL-1;
		number = 0;
	} else if (state == STATE_CSI && number > 0 && ch == 'D') {
		// Move cursor left "number" lines
		state = STATE_NORMAL;
		if ((cursor_x - number) >= 0)
			cursor_y -= number;
		else
			cursor_y = 0;
		number = 0;
	} else if (state == STATE_CSI && ch == ';') {
		state = STATE_EXT1;
		num[0] = number;
		number = 0;
	} else if (state == STATE_EXT1 && ch == ';') {
		state = STATE_EXT2;
		num[1] = number;
		number = 0;
	} else 	if (state == STATE_EXT1 && ch == 'H') {
		// Move cursor at position y,x
		num[1] = number;
		number = 0;
		state = STATE_NORMAL;
		if (num[0] > (VROW-1)) 	num[0]= (VROW-1);
		if (num[0] < 0) 					num[0] = 0;
		if (num[1] > (VCOL-1) ) 		num[1] =  (VCOL-1) ;
		if (num[1] < 0)					num[1] = 0;
		
		cursor_y = num[0];
		cursor_x = num[1];
		Vx=cursor_x;
		Vy=cursor_y;
	} else if (state == STATE_EXT1 && ch == 'm') {
		// Set Graphics Mode:
		state = STATE_NORMAL;
		num[1] = number;
		number = 0;
		color= num[1];
	} else if (state == STATE_EXT1 && ch == 'o') {
		// Switch on:
		state = STATE_NORMAL;
		num[1] = number;
		number = 0;
		mySwitch.switchOn("10101", num[1]);
	} else if (state == STATE_EXT1 && ch == 'c') {
		// Switch off:
		state = STATE_NORMAL;
		num[1] = number;
		number = 0;
		mySwitch.switchOff("10101", num[1]);
	} else if (state == STATE_NORMAL && ch == TERM_CR) { 
	    /* \r */
		number = 0;
		if(cursor) {
			Vchar[cursor_y][cursor_x] = 0;
			Vcolor[cursor_y][cursor_x] = BLACK;
		}
		if(cursor_y < VCOL)
			cursor_y++;
		cursor_x = 0;
	} else if (state == STATE_NORMAL && ch == TERM_LF) { 
	    /* \n */
		number = 0;
		if(cursor) {
			Vchar[cursor_y][cursor_x] = 0;
			Vcolor[cursor_y][cursor_x] = BLACK;
		}
		if(cursor_y < VCOL)
			cursor_y++;
		cursor_x = 0;
	} else if (state == STATE_NORMAL && ch == TERM_BACK) { 
	    /* \n */
		number = 0;
		if(cursor) {
			Vchar[cursor_y][cursor_x] = 0;
			Vcolor[cursor_y][cursor_x] = BLACK;
		}
		if(cursor_x > 0) {
			cursor_x--;
		} else {
			cursor_x = 0;
			if (cursor_y > 0)	cursor_y--;
		}
	}else  {
		Vchar[cursor_y] [cursor_x] = ch;
		Vcolor[cursor_y] [cursor_x] = color;
		if(cursor_x < VCOL) {
			cursor_x++;
		} else if(cursor_y < VCOL) {
			cursor_y++;
			cursor_x=0;
		}
	} 
}

void loop() {
	ledMatrix.display(0, Vchar[Vy],     Vcolor[Vy],      1000, VCOL-1, scroll); 
	ledMatrix.display(1, Vchar[Vy+1], Vcolor[Vy+1], 1000, VCOL-1, scroll);
  
	if (Serial.available() > 0) {
		term( Serial.read() );
	}
	
	if(cursor) {
		Vchar[cursor_y][cursor_x] = '_';
		Vcolor[cursor_y][cursor_x] = GREEN | BLINK;
	}
    
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		switch(results.value) {
			case IR_POWER:
					Serial.print("W");
				break;
			case IR_VOL_PLUS:
					Serial.print("+");
				break;			
            case IR_FUNC_STP:
					Serial.print("F");
					if (scroll) 
						scroll=false;
					else
						scroll=true;
				break;
            case IR_REWIND:
					Serial.print("R");
				break;			
            case IR_PLAY:
					Serial.print("P");
				break;			
			case IR_FASTF:	
					Serial.print("F");
				break;			
            case IR_DOWN:
					Serial.print("D");
					if (Vy < VROW )
						Vy++;
				break;
            case IR_VOL_MIN:
					Serial.print("-");
				break;			
            case IR_UP:
					Serial.print("U");
					if (Vy > 0 )
						Vy--;
				break;			
            case IR_NUM_0:  
					Serial.print("0");
				break;			
            case IR_EQ:
					Serial.print("E");
					Vx=Vy=0;
				break;  			
            case IR_ST_REPT:
					Serial.print("S");
					term(TERM_ESC);
					term('[');
					term('2');
					term(';');
					term('2');
					term('m');
				break;			
            case IR_NUM_1:  
					Serial.print("1");	
				break;			
            case IR_NUM_2:  
					Serial.print("2");
				break;			
            case IR_NUM_3:  
					Serial.print("3");
				break;			
            case IR_NUM_4:  
					Serial.print("4");
				break;			
            case IR_NUM_5:  
					Serial.print("5");
				break;			
            case IR_NUM_6:
					Serial.print("6");
				break;			
			case IR_NUM_7:  
					Serial.print("7");	
				break;			
			case IR_NUM_8:		
					Serial.print("8");
				break;			
			case IR_NUM_9:
					Serial.print("9");
				break;			
		}            
		irrecv.resume(); // resume receiver
	}
}
