/*get time and pin1 and pin2 state and run a linux process on the linino server to write to database table
*/
#include <Bridge.h>
#include <Process.h>

// how often to run the pin read (in milliseconds)
const unsigned long RUN_INTERVAL_MILLIS = 60000; 

// the last time we ran the pin read (initialized to 60 seconds ago,
// so the pins are read immediately when we start up)
unsigned long lastRun = (unsigned long)-60000;
// variables for setting up time 
Process date;                 // process used to get the date
String timeString;


void setup() {
  // put your setup code here, to run once:
 
  // for debugging, wait until a serial console is connected. This can be removed after the code is debugged, along with all the Serial. commands
  Serial.begin(9600);
  delay(4000);
  while(!Serial);

  Serial.print("Initializing the bridge...");
  Bridge.begin();
  Serial.println("OK");

  // run an initial date process. Should return:
  // hh:mm:ss :
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D +%T");
    date.run();
    Serial.println(date.readString());
  }

}//end setup

void loop() {
  // put your main code here, to run repeatedly:
 // get the number of milliseconds this sketch has been running
  unsigned long now = millis();

  // run again if it's been RUN_INTERVAL_MILLIS milliseconds since we last ran
  if (now - lastRun >= RUN_INTERVAL_MILLIS) {

    // remember 'now' as the last time we checked the pins
    lastRun = now;
    // get the date and time
    if (!date.running())  {
      date.begin("date");
      date.addParameter("+%D +%T");
      date.run();
    }//end if
    
    // put the date and time into a string variable
    while (date.available()>0) {
      timeString = date.readString(); 
    }//end while
    
    //get the pin states and add them to the database
    String pin1 = String(getSensorValue(1));
    String pin2 = String(getSensorValue(2));
    runSqlQuery(timeString,pin1,pin2);
    } //end if
}// end loop
 // function to run the appending of the data to the database
 unsigned int runSqlQuery(String time, String pin1, String pin2){
   Process p;
   String cmd = "sqlite3 ";
   String paramstring1 = "-line ";
   // set the path and name of your database here
   String paramstring2 ="/mnt/sda1/arduino/www/test/test1.db ";
   // insert a row with time and sensor data 
   String paramstring3 ="'insert into testdata (\"time\",\"pin1\",\"pin2\") Values (\""+time+"\","+pin1+","+pin2+");'";
   // get the error code
   String paramstring4 =" ; echo $?";
   p.runShellCommand(cmd + paramstring1 + paramstring2 + paramstring3+ paramstring4);
   Serial.println("process run");
     // A process output can be read with the stream methods
   while (p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  // Ensure the last bit of data is sent.
  Serial.flush();

  }
//function to get sensor values with debugging information
int getSensorValue(int pinNum) {
  Serial.print("Reading pin "+ String(pinNum)+ "value...");
  int value = analogRead(pinNum);
  Serial.println("OK");
  return value;
}
