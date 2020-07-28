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

//#define log

void init_sdcart(){

	if (!SD.begin(PIN_SD_CARD)) { return false; }

}



boolean read_send(EthernetClient client, const char *filename){

	 char path[22];
	 if(strcmp(filename,"/"))
		 sprintf(path,"%s%s",WWW,filename);
	 else
		 sprintf(path,"%s%s",WWW,MAINPAGE);
	 //Serial.println(path);

	 File webFile = SD.open(path,FILE_READ);

	  if (webFile) {
	    while(webFile.available()) {
	      client.write(webFile.read());
	    }
	    webFile.close();
	    return true;
    } else{
          client.print("<html>E100</html>");
          //client.println("<a href=\"logout.html\">sair</a></html>");
          //client.println(" nao encontrado</br> <a href=\"logout.html\">sair</a></html>");
          Serial.println("E:100");
    }

    return false;
}


uint8_t html_status(EthernetClient client){

	uint8_t i = 0;

    client.println(HTTPOK);
    client.print(CTENTP);
    client.print(TEXT);
    client.print("/");
    client.println(HTML);
    client.println(CONNEC);  // the connection will be closed after completion of the response
    client.println("");  // the connection will be closed after completion of the response
    read_send(client,"/header.htm");

	for(i=0;i<10;i++)
		client.println("Status Linha" + String(i) + "</br>");

    read_send(client,"/footer.htm");

	return 0;
}


// Prepare and send response
void response_content(EthernetClient client, const char *content){

          char contentType[16];
          client.println(HTTPOK);
          client.print(CTENTP);

               if(strstr(content,HTM)){
          	  	   sprintf(contentType,"%s/%s",TEXT,HTML);}
               else if(strstr(content,CSS)){
                   sprintf(contentType,"%s/%s",TEXT,CSS);}
               else if(strstr(content,JS)){
            	   sprintf(contentType,"%s/javascript",TEXT);}
               else if(strstr(content,ICO)){
            	   sprintf(contentType,"%s/x-%sn",IMAGE,ICO);}
               else if(strstr(content,WEBP)){
            	   sprintf(contentType,"%s/%s",IMAGE,WEBP);}
               else if(strstr(content,JPG)){
                   sprintf(contentType,"%s/%s",IMAGE,JPG);}
               else if(strstr(content,PNG)){
            	   sprintf(contentType,"%s/%s",IMAGE,PNG);}
               else if(strstr(content,PDF)){
                   sprintf(contentType,"%s%s",FILEN,"application/download");}
               else
            	   sprintf(contentType,"%s/%s",TEXT,HTML);

                client.println(contentType);
                client.println(CONNEC);  // the connection will be closed after completion of the response

                client.println();

          read_send(client,content);


}



int processHtppRequest(EthernetServer server){

	  // listen for incoming clients
	  EthernetClient client = server.available();


	  if (client) {


	    char request[40]="";
	    uint8_t c_req = 0;
	    boolean loged = false;
	    boolean logoff = false;
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


	          if(loged and !logoff){
	        	  if(strstr(contentName,"teste.html"))
	        		  html_status(client);
	        	  else
	        	  	  response_content(client,contentName);
	          }else{
	        	  if(logoff){
	        		  read_send(client,LOGOUTPG);
	        	  }
	        	  else
	        		  read_send(client,LOGINPG);
	          }
	          memset(contentName, 0, sizeof(contentName));

	          break;
	        }
	        if (c == '\n') {
	          // you're starting a new line
	          currentLineIsBlank = true;

	          if(strstr(request,"GET")){
	        	  sscanf(request,"GET %s HTTP/1.1",contentName);
	        	//  Serial.println(contentName);
	              //strcpy(contentName,req.substring(req.indexOf('/'),req.indexOf(" HTTP")).c_str());
	          }
	          if(strstr(request,LOGOUTPG)) logoff = true;
	          if(strstr(request,PASSWD)) loged = true;

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
	    delay(1);
	    // close the connection:
	    client.stop();
	    Serial.flush();

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
