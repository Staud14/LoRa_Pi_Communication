/*************************************************************************
This Program provides an interface to an Semtech SX12xx chip.
Copyright (C) 2019  Patrik Staudenmayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/


#define DEBUG


#include <iostream>
#include <string>
#include "LoRaClass.h"
#include "JSON.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"
#define USER_CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/user_config.json"


using namespace std;
using namespace rapidjson;


int main(void)
{
	JSON config;

	JSON user_config;

	if (!config.open(CONFIG_PATH)) {
#ifdef DEBUG
		cout << "config.json could not be found." << endl;
#endif
		exit(EXIT_FAILURE);
	}

	user_config.setPath(USER_CONFIG_PATH);

	

/*
	//Testing JSON.h
	cout << "spi: " << config.getSPI() << endl;
	cout << "spi_frequency: " << config.getSPI_frequency() << endl;
	cout << "ss_pin: " << config.getSS_pin() << endl;
	cout << "reset_pin: " << config.getResetPin() << endl;
	cout << "dio0_pin: " << config.getDIO0_pin() << endl;
	cout << "frequency: " << config.getFrequency() << endl;
	cout << "power: " << config.getPower() << endl;
	cout << "rfo_pin: " << config.getRFO_pin() << endl;
	cout << "pa_boost_pin: " << config.getPAboostPin() << endl;
	cout << "mode: " << config.getMode() << endl;
*/	

	if (user_config.open(USER_CONFIG_PATH))			//Checks if user_config exists
	{
		//Setting up SPI
		if (user_config.hasSPI())
		{
			LoRa.setSPIPort(user_config.getSPI());
		}
		else
		{
			LoRa.setSPIFrequency(user_config.getSPI());
		}

		if (user_config.hasSPI_frequency())
		{
			LoRa.setSPIPort(user_config.getSPI_frequency());
		}
		else
		{
			LoRa.setSPIFrequency(user_config.getSPI_frequency());
		}

		//Set transmission frequency
		if (user_config.hasFrequency())
		{
			LoRa.setFrequency(user_config.getFrequency());
		}
		else
		{
			LoRa.setFrequency(config.getFrequency());
		}

		//Set Pins
		if (user_config.hasSS())
		{
			LoRa.setSS(user_config.getSS_pin());
		}
		else
		{
			LoRa.setSS(config.getSS_pin());
		}

		if (user_config.hasReset())
		{
			LoRa.setReset(user_config.getResetPin());
		}
		else
		{
			LoRa.setReset(config.getResetPin());
		}

		if (user_config.hasDIO0())
		{
			LoRa.setDIO0(user_config.getDIO0_pin());
		}
		else
		{
			LoRa.setDIO0(config.getDIO0_pin());
		}

		//Set Tx power and RFO pin
		if (user_config.hasPower())
		{
			LoRa.setTxPower(user_config.getPower(), config.getPAboostPin());
		}
		else if (user_config.hasPAboost())
		{
			LoRa.setTxPower(config.getPower(), user_config.getPAboostPin());
		}
		else if(user_config.hasPower() && user_config.hasPAboost())
		{
			LoRa.setTxPower(user_config.getPower(), user_config.getPAboostPin());
		}
		else
		{
			LoRa.setTxPower(config.getPower(), config.getPAboostPin());
		}
	}
	else
	{
		//Setting up SPI
		LoRa.setSPIPort(config.getSPI());
		LoRa.setSPIFrequency(config.getSPI_frequency());

		//Set transmission frequency
		LoRa.setFrequency(config.getFrequency());

		//Set Pins
		LoRa.setPins(config.getSS_pin(), config.getResetPin(), config.getDIO0_pin());

		//Set Tx power and RFO pin
		LoRa.setTxPower(config.getPower(), config.getPAboostPin());
	}

	

	//Begin LoRa
	LoRa.begin();

	int counter;
	string buffer;

	

	while (true) 
	{
		

		// send packet
		buffer.append("Hello World ");
		buffer.append(to_string(counter));

		cout << buffer << endl;

		LoRa.beginPacket();
		LoRa.print(buffer);
		LoRa.endPacket();

		counter++;
		buffer.clear();
		delay(5000);
	}

	LoRa.end();
	
	

	string i;
	cin >> i;
	return 0;
}