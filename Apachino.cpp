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

//Check content extension
boolean checkExt(const char *content, const char *ext){

         if(strstr(content,ext)){
            return true;
         }
         else
            return false;
}

// Prepare and send response
void response_content(EthernetClient client, String contentS){

		  const char content[30] = MAINPAGE;

		  if(!contentS.equals("/"))
		  	  contentS.toCharArray(content, sizeof(content), 1);

          char contentType[30] = "Content-Type: ";
          client.println("HTTP/1.1 200 OK");

               if(checkExt(content,HTML)){
          	  	   sprintf(contentType,"%stext/%s",contentType,HTML);}
               if(checkExt(content,CSS)){
                   sprintf(contentType,"%stext/%s",contentType,CSS);}
               if(checkExt(content,JS)){
            	   sprintf(contentType,"%stext/%s",contentType,JS);}
               if(checkExt(content,ICO)){
            	   sprintf(contentType,"%simage/x-icon",contentType);}
               if(checkExt(content,JPG)){
            	   sprintf(contentType,"%sfile/%s",contentType,JPG);}
               if(checkExt(content,PDF)){
            	   sprintf(contentType,"%sfile/%s",contentType,PDF);}
               if(checkExt(content,WEBP)){
            	   sprintf(contentType,"%simage/%s",contentType,WEBP);}
               if(checkExt(content,PNG)){
            	   sprintf(contentType,"%sfile/%s",contentType,PNG);}

                client.println(contentType);  // the connection will be closed after completion of the response

                client.println("Connection: closed");  // the connection will be closed after completion of the response


          client.println();

          read_send(client,content);


}

boolean loged = false;

int processHtppRequest(EthernetServer server){

	  // listen for incoming clients
	  EthernetClient client = server.available();


	  if (client) {
	    Serial.println("new client");

	    char request[30]="";
	    uint8_t c_req = 0;
	    boolean post = false;
	    char contentName[20]="";

	    memset(request, 0, sizeof(request));
	    memset(contentName, 0, sizeof(contentName));

	    // an http request ends with a blank line
	    boolean currentLineIsBlank = true;

	    while (client.connected()) {
	      if (client.available()) {
	        char c = client.read();

	        if(c_req < sizeof(request)){
	        	request[c_req] = c;
	        	c_req++;
	        }

	        Serial.write(c);

	        if (c == '\n' && currentLineIsBlank) {

	          c_req = 0;

	        	  while(client.available() and (c_req < 30)){
	        		  request[c_req] =  (char)client.read();
	        		  c_req++;
	        	  }

	          Serial.println(request);
	          if(strstr(request,"admin")){
	          	  loged = true;}

	          if(loged){
	        	  response_content(client,contentName);
	          }else{
	        	  response_content(client,"/"MAINPAGE);
	          }
	          memset(contentName, 0, sizeof(contentName));

	          break;
	        }
	        if (c == '\n') {
	          // you're starting a new line
	          currentLineIsBlank = true;
	          String req = request;

	          if(strstr(request,"GET") || (post = strstr(request,"POST"))){
	              strcpy(contentName,req.substring(req.indexOf('/'),req.indexOf(" HTTP")).c_str());
	          }
	          req="";

	          memset(request, 0, sizeof(request));
	          c_req = 0;

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
	    Serial.flush();
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
