/*
 * Apachino.h
 *
 *  Created on: 22 de jul de 2020
 *      Author: User
 */

#ifndef APACHINO_H_
#define APACHINO_H_

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>


// SD card chip select pin.

const uint8_t PIN_SD_CARD = 4;
const char WWW[] = "/wwwb";
const char SRCDIR[] = "src";
const char MAINPAGE[] = "/index.htm";
const char LOGINPG[] = "/login.htm";
const char LOGOUTPG[] = "/logout.htm";
const char PASSWD[] = "YWRtaW46YWRtaW4=";


//Request Defines
const char HTTPOK[] = "HTTP/1.1 200 OK";
const char CTENTP[] = "Content Type: ";
const char CONNEC[] = "Connection: closed";

const char TEXT[] = "text";
const char IMAGE[] ="image";
const char FILEN[] = "file";
const char JPG[] = "jpg";
const char PNG[] = "png";
const char PDF[] = "pdf";
const char HTM[] = "htm";
const char HTML[] = "html";
const char ICO[] =  "ico";
const char WEBP[] = "webp";
const char JS[] = "js";
const char CSS[] = "css";



void init_sdcart();

bool check_ethernet();

int processHtppRequest(EthernetServer server);

#endif /* APACHINO_H_ */
