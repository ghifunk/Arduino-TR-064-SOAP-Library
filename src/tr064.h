/*!
 * @file tr064.cpp
 *
 * @mainpage Library for communicating via TR-064 protocol (e.g. Fritz!Box)
 *
 * @section intro_sec Introduction
 * 
 * This library allows for easy communication of TR-064 (and possibly TR-069) enabled devices,
 * such as Routers, smartplugs, DECT telephones etc.
 * Details, examples and the latest Version of this library can be found <a href='https://github.com/Aypac/Arduino-TR-064-SOAP-Library'>on my Github page</a>.
 * A descriptor of the protocol can be found <a href='https://avm.de/fileadmin/user_upload/Global/Service/Schnittstellen/AVM_TR-064_first_steps.pdf'>here</a>.
 * 
 *
 * @section dependencies Dependencies
 *
 * This library depends on:
 *	MD5Builder
 * ESP8266HTTPClient or HTTPClient, depending on the intended platform (ESP8266 or ESP32).
 *
 * @section author Author
 *
 * Written by René Vollmer "Aypac" in November 2016.
 *
 * @section license License
 *
 * MIT License, all text here must be included in any redistribution.
 *
 */



#ifndef tr064_h
#define tr064_h

#include "Arduino.h"
#include <MD5Builder.h>

#if defined(ESP8266)
	//if(Serial) Serial.println(F("Version compiled for ESP8266."));
	#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
	//if(Serial) Serial.println(F("Version compiled for ESP32."));
	#include <HTTPClient.h>
#else
	//INCOMPATIBLE!
#endif

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

class TR064 {
	public:
		TR064(int port, String ip, String user, String pass);
		void init();
		void initNonce();
		String action(String service, String act);
		String action(String service, String act, String params[][2], int nParam);
		String action(String service, String act, String params[][2], int nParam, String (*req)[2], int nReq);
		String xmlTakeParam(String inStr, String needParam);
		String xmlTakeInsensitiveParam(String inStr, String needParam);
		String xmlTakeSensitiveParam(String inStr, String needParam);
		String md5String(String s);
		String byte2hex(byte number);
	private:
		//TODO: More consistent naming.
		void initServiceURLs();
		String httpRequest(String url, String xml, String action);
		String httpRequest(String url, String xml, String action, bool retry);
		String generateAuthToken();
		String generateAuthXML();
		String findServiceURL(String service);
		String _xmlTakeParam(String inStr, String needParam);
		String _ip;
		int _port;
		String _user;
		String _pass;
		String _realm; //To be requested from the router
		String _secretH; //to be generated
		String _nonce = "";
		const String _requestStart = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">";
		const String _detectPage = "/tr64desc.xml";

		/* TODO: We should give access to this data for users to inspect the
		* possibilities of their device(s) - see #9 on Github.
		TODO: Remove 100 services limits here
		*/
		String _services[100][2];
		bool _error=false;
};

#endif