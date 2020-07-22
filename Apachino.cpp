#include <Arduino.h>
/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */


#include "Apachino.h"

SdFat sd;
SdFile file;
SdFile dirFile;



void init_sdcart(){
  if (!sd.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
    Serial.println("SD card initalization error");
    sd.initErrorHalt();
  }

  Serial.println(FreeStack());

  if (!dirFile.open(WWW, O_RDONLY)) {
    Serial.println("error web directory");
    sd.initErrorHalt();
  }

}

bool read_send(EthernetClient client, char filename[]){


    if (file.open(&dirFile,filename,O_RDONLY)) {

          client.println();
          while(file.available()){
            client.write(file.read());
          }

          file.close();
          return true;
    } else{
      if(!read_send(client,MAINPAGE)){
          client.print("<html>conteudo: ");
          client.print(filename);
          client.println(" nao encontrado</html>");
          Serial.println("conteudo nao encontrado");
      }
      return false;
    }

    return false;
}




bool checkExt(char content[], char ext[]){

         if(String(content).indexOf(String(ext)) != -1){
            return true;
         }
         else
            return false;
}

void response_content(EthernetClient client, char content[]){

          String contentType = "Content-Type: ";
          client.println("HTTP/1.1 200 OK");

                if(checkExt(content,JPG)){
                   contentType += "file/";
                   contentType += JPG;}
                else if(checkExt(content,PDF)){
                   contentType += "file/";
                   contentType += PDF;   }
                else if(checkExt(content,HTML)){
                    contentType += "text/html";}
                else if(checkExt(content,CSS)){
                    contentType += "text/css";}
                else if(checkExt(content,JS)){
                    contentType += "text/javascript";}
                else if(checkExt(content,ICO)){
                    contentType += "image/x-ico";}
                else if(checkExt(content,WEBP)){
                    contentType += "file/webp";}
                else if(checkExt(content,PNG)){
                    contentType += "image/png";}
                else
                    contentType += "text/html";

                Serial.println(contentType);  // the connection will be closed after completion of the response
                client.println(contentType);  // the connection will be closed after completion of the response

                client.println("Connection: keep-alive");  // the connection will be closed after completion of the response


          client.println();

          read_send(client,content);


}


int processHtppRequest(EthernetServer server){

	  // listen for incoming clients
	  EthernetClient client = server.available();
	  if (client) {
	    Serial.println("new client");
	    String request ="";
	    String contentName = "";
	    // an http request ends with a blank line
	    boolean currentLineIsBlank = true;

	    while (client.connected()) {
	      if (client.available()) {
	        char c = client.read();
	        request += c;
	        //Serial.write(c);
	        // if you've gotten to the end of the line (received a newline
	        // character) and the line is blank, the http request has ended,
	        // so you can send a reply

	        if (c == '\n' && currentLineIsBlank) {
	          // send a standard http response header
	          response_content(client,contentName.c_str());
	          contentName = "";
	          break;
	        }
	        if (c == '\n') {
	          // you're starting a new line
	          currentLineIsBlank = true;

	          if(request.indexOf("GET") != -1){
	              contentName = request.substring(request.indexOf('/')+1,request.indexOf(" HTTP"));
	              request ="";
	          }

	        } else if (c != '\r') {
	          // you've gotten a character on the current line

	          currentLineIsBlank = false;
	        }
	      }
	    }
	          //Serial.println(request);

	    // give the web browser time to receive the data
	    //delay(1);
	    // close the connection:
	    client.stop();
	    Serial.println("client disconnected");
	  }

	return 0;
}

bool check_ethernet(){
// Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		//Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
		while (true) {
			delay(1); // do nothing, no point running without Ethernet hardware
		}
	}

	if (Ethernet.linkStatus() == LinkOFF) {
//    Serial.println("Ethernet cable is not connected.");
		return false;
	}

	return true;
}
