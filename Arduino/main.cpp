#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

// Data wire is plugged into pin 12 on the Arduino
#define ONE_WIRE_BUS 12
// Ten wire is plugged into pin 3 on the Arduino
#define TEN_WIRE 11
// Bluetooth module
#define BT_Tx 6
#define BT_Rx 7
#define TEN_HIGH_MAX 3
#define TEN_LOW_MAX 8

// Commands
#define CMD_SHOW				"SHOW"
#define CMD_SAVE				"SAVE"
#define CMD_LOAD				"LOAD"
#define CMD_STEP_ADD			"STEP_ADD"
#define CMD_STEP_DEL			"STEP_DEL"
#define CMD_STEP_UPDATE 		"STEP_UPDATE"
#define CMD_STEP_INSERT 		"STEP_INSERT"
#define CMD_START				"START"
#define CMD_START_AT			"START_AT"
#define CMD_STOP				"STOP"
#define CMD_SET_TEN_HIGHT_MAX	"SET_THI"
#define CMD_SET_TEN_LOW_MAX		"SET_TLO"

SoftwareSerial blueToothSerial(BT_Rx,BT_Tx);

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

unsigned long prevMillis;
unsigned long durationMillis = 0;
byte steps = 0;
int step = 1;
// {{temperature,duration in minutes,0-go|1-wait,0-fix temperature|1-do not fix temperature}}
unsigned long params[10][4];// = {{25,1},{27,1}}; //{65,1},

unsigned int iTenHigh = 0;
unsigned int iTenLow = 0;
unsigned int iTenHighMax = TEN_HIGH_MAX;
unsigned int iTenLowMax = TEN_LOW_MAX;

bool goNext = false;
bool isWait = false;

String cmd = "SHOW";
String args = "";

void displayData(String str){
    Serial.print(str);
    blueToothSerial.print(str);
}

String cmd_Read(){
	String cmd = "";
	char ch;

	while (ch != '\r'){
		if(blueToothSerial.available()){
			ch = (char)blueToothSerial.read();
			if (ch != '\r' && ch != '\n')
				cmd.concat(ch);
		}
	}

	while(blueToothSerial.available())
		blueToothSerial.read();

	cmd.toUpperCase();
	//blueToothSerial.println(cmd);

	return cmd;
}

bool readYes(){
	char ch;

	displayData("msgWaitYes\r\n");

	if(blueToothSerial.available()){
		ch = (char)blueToothSerial.read();
	}

	return ch == 'y';
}

void cmd_Show(){
	int iStep = 1;
	char buffer[256];
	//displayData("|Step|Duration|Temperature|IsWait|Temperature|\r\n");
	while (steps >= iStep){
		//displayData(",");
		displayData(itoa(iStep, buffer, 10));
		displayData(",");
		displayData(itoa(params[iStep-1][0], buffer, 10));
		displayData(",");
		displayData(itoa(params[iStep-1][1], buffer, 10));
		displayData(",");
		displayData(itoa(params[iStep-1][2], buffer, 10));
		displayData(",");
		displayData(itoa(params[iStep-1][3], buffer, 10));
		displayData(";");
		iStep++;
	}
	displayData(";THI:");
	displayData(itoa(iTenHighMax, buffer, 10));
	displayData("TLO");
	displayData(itoa(iTenLowMax, buffer, 10));
	displayData(";");
}

void cmd_StepAdd(unsigned long lTemperature, unsigned long lTime, unsigned long lWait, unsigned long lFixTemp){
	int curStep = steps + 1;
	params[curStep - 1][0] = lTemperature;
	params[curStep - 1][1] = lTime;
	params[curStep - 1][2] = lWait;
	params[curStep - 1][3] = lFixTemp;
	steps++;
}

void cmd_StepDel(){
	if(steps > 0){
		params[steps - 1][0] = 0;
		params[steps - 1][1] = 0;
		params[steps - 1][2] = 0;
		params[steps - 1][3] = 0;
		steps--;
	}
}

void cmd_StepInsert(int iStep, unsigned long lTemperature, unsigned long lTime){

}

void cmd_StepUpdate(int iStep, unsigned long lTemperature, unsigned long lTime, unsigned long lWait, unsigned long lFixTemp){
	params[iStep - 1][0] = lTemperature;
	params[iStep - 1][1] = lTime;
	params[iStep - 1][2] = lWait;
	params[iStep - 1][3] = lFixTemp;
}

void cmd_Save(){
	int addr = 0;
	unsigned long lVal;
	byte* pVal;
	int valSize = sizeof(lVal);
	byte val = 0;
	int iStep = 1;
	EEPROM.write(addr, steps);
	addr++;
	if (steps > 0)
		while(iStep <= steps){
			for(int cell=0;cell<4;cell++){
				lVal = params[iStep-1][cell];
				pVal = (byte*)&lVal;
				for(int i=0;i<valSize;i++){
					val = (byte)*pVal;
					EEPROM.write(addr, val);
					Serial.print(val);
					Serial.print(",");
					pVal++;
					addr++;
				}
			}
			Serial.println(";");
			iStep++;
		}
	lVal = iTenHighMax;
	pVal = (byte*)&lVal;
	val = (byte)*pVal;
	EEPROM.write(addr, val);
	pVal++;
	addr++;
	lVal = iTenLowMax;
	pVal = (byte*)&lVal;
	val = (byte)*pVal;
	EEPROM.write(addr, val);
	pVal++;
	addr++;
}

void cmd_Load(){
	int addr = 0;
	unsigned long lVal;
	byte* pVal;
	int valSize = sizeof(lVal);
	byte val = 0;
	int iStep = 1;
	steps = EEPROM.read(addr);
	Serial.print("steps:");
	Serial.println(steps);
	addr++;

	if (steps > 0)
		while(iStep <= steps){
			for(int cell=0;cell<4;cell++){
				pVal = (byte*)&lVal;
				for(int i=0;i<valSize;i++){
					val = EEPROM.read(addr);
					* pVal = val;
					pVal++;
					addr++;
				}
				params[iStep-1][cell] = lVal;
				Serial.print(lVal);
				Serial.print(",");
			}
			iStep++;
			Serial.println("");
		}
	pVal = (byte*)&lVal;
	val = EEPROM.read(addr);
	* pVal = val;
	pVal++;
	addr++;
	iTenHighMax = lVal;

	pVal = (byte*)&lVal;
	val = EEPROM.read(addr);
	* pVal = val;
	pVal++;
	addr++;
	iTenLowMax = lVal;
}

bool setTemperature(unsigned long iTemp){
	//Serial.println("Req temp");
	sensors.requestTemperatures(); // Send the command to get temperatures
	//Serial.println("Requested");
	float tempr =  sensors.getTempCByIndex(0);
	//Serial.println("Temp res");
	//displayData("Temp res");
	char buffer[256];
	displayData(itoa(tempr, buffer, 10));
	displayData(",");
	displayData(itoa(iTemp, buffer, 10));
	displayData(",");
	//Serial.println("params");
//	Serial.println("thi:" + iTenHighMax);
//	Serial.println("tlo:" + iTenLowMax);
	if ((tempr <= iTemp) && (iTenHigh < iTenHighMax)){
		digitalWrite(TEN_WIRE, HIGH);
		displayData("H");
		iTenHigh++;
		iTenLow = 0;
	}else{
		digitalWrite(TEN_WIRE, LOW);
		displayData("L");
		iTenLow++;
		if(iTenLow >= iTenLowMax)
			iTenHigh = 0;
	}
	displayData("\r\n");

	return (tempr >= (iTemp - 1)) and (tempr <= (iTemp + 10));
}

int cmd_Start(int iStep){
	// Meshing
	if (steps >= iStep){
		unsigned long timePause = 60u * 1000u * params[iStep-1][1];
		char buffer[256];
		displayData(itoa(iStep, buffer, 10));
		displayData(",");
		displayData(itoa(params[iStep-1][1], buffer, 10));
		displayData(",");
		displayData(itoa(durationMillis / 60u / 1000u, buffer, 10));
		displayData(",");

		int maxTemp = params[iStep-1][0];
		if(params[iStep-1][3] == 1 && isWait){
			maxTemp = 0;
			if(readYes())
				iStep++;
		}

		Serial.println(iStep);

		if (setTemperature(maxTemp)){
			Serial.println(maxTemp);
			unsigned long curMillis = millis();
			durationMillis = durationMillis + (curMillis - prevMillis);
			prevMillis = curMillis;
			if (durationMillis > timePause){
				if(params[iStep-1][2] == 0){
					goNext = true;
				}else if((params[iStep-1][2] == 1) && (readYes())){
					goNext = true;
				}else
					isWait = true;
				if(goNext){
					durationMillis = 0;
					prevMillis = millis();
					iStep++;
					displayData("Step: ");
					displayData(itoa(iStep, buffer, 10));
					displayData("\r\n");
					goNext = false;
					isWait = false;
				}
			}
		}else{
			prevMillis = millis();
		}
	}else
		displayData("Finished\r\n");

	// Check gravity

	// Boiling with melt

	// Boiling + hop

	delay(1000);

	return iStep;
}

void setup(void)
{
	// init pins
	pinMode(TEN_WIRE, OUTPUT);

	// start serial port
	prevMillis= millis();
	Serial.begin(9600);

    pinMode(BT_Rx, INPUT);
    pinMode(BT_Tx, OUTPUT);
    blueToothSerial.begin(38400);

	// Start up the library
	sensors.begin();

	// Load brewering receipt from EEPROM
	//cmd_Load();
}

void loop(void)
{
//	if (blueToothSerial.available())
//		cmd = "";

	if(cmd != CMD_START){
		String sCmd = cmd_Read();
		prevMillis = millis();
		durationMillis = 0;

		if (sCmd != ""){
			int iPos = sCmd.indexOf(" ");
			if(iPos > 0){
				cmd = sCmd.substring(0, iPos);
				args = sCmd.substring(iPos+1, sCmd.length());
			}else
				cmd = sCmd;
		}
	}

	if (cmd == CMD_SHOW){
		cmd_Show();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_STOP){
		cmd = "";
	}else if (cmd == CMD_SET_TEN_HIGHT_MAX){
		String sTenHighMax = args;
		iTenHighMax = sTenHighMax.toInt();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_SET_TEN_LOW_MAX){
		String sTenLowMax = args;
		iTenLowMax = sTenLowMax.toInt();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_STEP_ADD){
		int curInd = args.indexOf(",");
		String sTemperature = args.substring(0, curInd);
		int prevInd = curInd;
		curInd = args.indexOf(",", prevInd+1);
		String sTime = args.substring(prevInd + 1, curInd);
		prevInd = curInd;
		curInd = args.indexOf(",", prevInd+1);
		String sWait = args.substring(prevInd + 1, curInd);
		prevInd = curInd;
		curInd = args.length();
		String sFixTemp = args.substring(prevInd + 1, curInd);
		//temperature,time
		unsigned long lTemperature, lTime, lWait, lFixTemp;
		lTemperature = sTemperature.toInt();
		lTime = sTime.toInt();
		lWait = sWait.toInt();
		lFixTemp = sFixTemp.toInt();
		cmd_StepAdd(lTemperature, lTime, lWait, lFixTemp);
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_STEP_DEL){
		cmd_StepDel();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_STEP_INSERT){
		//step,temperature,time
		int iStep;
		unsigned long lTemperature, lTime;
		cmd_StepInsert(iStep, lTemperature, lTime);
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_STEP_UPDATE){
		//step,temperature,time
		int iStep;
		unsigned long lTemperature, lTime, lWait, lFixTemp;
		cmd_StepUpdate(iStep, lTemperature, lTime, lWait, lFixTemp);
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_LOAD){
		cmd_Load();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_SAVE){
		cmd_Save();
		displayData("OK\r\n");
		cmd = "";
	}else if (cmd == CMD_START_AT){
		step = args.toInt();
		step = cmd_Start(step);
		cmd = CMD_START;
		args = "";
	}else if (cmd == CMD_START){
		step = cmd_Start(step);
		if(step > steps)
			cmd = "";
	}else{
		displayData("Error");
//		displayData("Help:");
//		displayData(CMD_SHOW);
//		displayData(CMD_SAVE);
//		displayData(CMD_LOAD);
//		displayData(CMD_STEP_ADD);
//		displayData(CMD_STEP_DEL);
//		displayData(CMD_STEP_UPDATE);
//		displayData(CMD_STEP_INSERT);
//		displayData(CMD_START);
//		displayData(CMD_START_AT);
//		displayData(CMD_STOP);
//		displayData(CMD_SET_TEN_HIGHT_MAX);
//		displayData(CMD_SET_TEN_LOW_MAX);
	}
}

int main(void) {
	init();
	setup();

	while(true) {
	loop();
	}
}
