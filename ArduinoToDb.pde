
import processing.serial.* ; //import to use serial port for communication
import de.bezier.data.sql.*; //import to connect to a database server

Serial myPort;  // Create object from Serial class
String val;

float ax;
float ay;
float az;

MySQL db;
int i = 0;

void setup(){
frameRate(1); // 1 frame per second
size(200 ,200) ;

//declare server name, username, password and database name
String server = "localhost";
String user = "root";
String pass = "";
String database = "accelerometer";
db = new MySQL(this , server , database , user , pass );

myPort = new Serial(this, "COM3", 9600);
}


void draw () {
  if ( myPort.available() > 0) { //if some data is coming in through serial port 
  val = myPort.readStringUntil('\n');
  if ( val != null ){ //if received string is non empty
  String[] myStringList = split ( val , ','); //process the string to obtain the 3 desired values from it
  if ( myStringList.length == 3) {
    ax=float(myStringList[0]);
    ay=float(myStringList[1]);
    az=float(myStringList[2]);
    if (db. connect ()){ //if a connection to db established, insert the values in the table
         db.query("INSERT INTO accelerometer_data(acc_x,acc_y,acc_z,label) VALUES(%f,%f,%f,'left')",ax,ay,az);
         println(i);
    }
    i++;
  } 
}
}
}
