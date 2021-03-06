// dGPS.h
// For use with the Dexter Industries / diCircuits.com dGPS
// The dGPS can be found here:  http://dexterindustries.com/Arduino-GPS_Shield.html


#include "SoftwareSerial.h"
#include "pins_arduino.h"
//#include "WProgram.h"
#include "Arduino.h"

#ifndef dGPS_h
#define dGPS_h

class dGPS
{
	public: 
		dGPS();
		void init();
		bool update(double,double);

		long Time();
		char *Status();
		double Lat();
		double Lon();
		double Vel();
		double Head();
		long Date();
                double Dist();
                double Azim();
                char *Mode();
                char *Checks();				

	private:
		// int _pin;
		char linea[300];
		//int _rxPin;
		//int _txPin;
		int cont;
		int bien;
		int conta;
		int indices[13];
		char comandoGPR[7]; // = "$GPRMC";
		double conv_coords(double in_coords);
		char *subStr (char *str, char *delim, int index);
		
};

#endif



