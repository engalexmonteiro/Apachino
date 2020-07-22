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
#include<SPI.h>
#include "SdFat.h"
#include "FreeStack.h"

// SD card chip select pin.
#define SD_CS_PIN 4
#define WWW "/wwwb"
#define MAINPAGE "login.html"
#define JPG "jpg"
#define PNG "png"
#define PDF "pdf"
#define HTML "html"
#define ICO  "ico"
#define WEBP "webp"
#define JS "js"
#define CSS "css"

extern SdFat sd;
extern SdFile file;
extern SdFile dirFile;


void init_sdcart();

bool check_ethernet();

int processHtppRequest(EthernetServer server);

#endif /* APACHINO_H_ */
