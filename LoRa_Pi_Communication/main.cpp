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

#include <csignal>

#include <QtGlobal>

#include "JSON.h"
#include "LoRaClass.h"
#include "logutils.h"

#define CONFIG_PATH                                                            \
    "/home/pi/projects/LoRa_Pi_Communication/LoRa_Pi_Communication/"           \
    "config.json"
#define USER_CONFIG_PATH                                                       \
    "/home/pi/projects/LoRa_Pi_Communication/LoRa_Pi_Communication/"           \
    "user_config.json"
#define LOG_PATH                                                               \
    "/home/pi/projects/LoRa_Pi_Communication/LoRa_Pi_Communication/log.txt"

using namespace std;

// Initializer
void initLoRa(JSON::JSON &config, JSON::JSON &user_config);

// Signal handlers
void handlerSIGTERM(int signum);
void handlerSIGABRT(int signum);
void handlerSIGINT(int signum);

int main(void) {
    JSON::JSON config;
    JSON::JSON user_config;

    // Registrate signal handler
    std::signal(SIGTERM, handlerSIGTERM);
    std::signal(SIGABRT, handlerSIGABRT);
    std::signal(SIGINT, handlerSIGINT);

    // Initializing logger
    if (!logutils::initLogging()) {
#ifdef DEBUG
        std::cout << "Logger could not start" << std::endl;
#endif
        abort();
    }

    //Loading config
    try {
        config.open(CONFIG_PATH);

        if ((!config.hasSPI()) || (!config.hasSPI_frequency()) ||
            (!config.hasSS()) || (!config.hasReset()) || (!config.hasDIO0()) ||
            (!config.hasFrequency()) || (!config.hasPower()) ||
            (!config.hasRFO()) || (!config.hasPAboost()) ||
            (!config.hasMode())) {
            qFatal("config: Config is damaged, settings are missing");
        }
    } catch (JSON::configuration_empty &e) {
        qFatal("config: Config is empty");
    } catch (JSON::configuration_nonexistent &e) {
        qFatal("config: Config is nonexisting");
    }

    //Loading user_config
    user_config.open(USER_CONFIG_PATH);




    // Testing JSON.h
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

    initLoRa(config, user_config);
    qInfo("config: Config was loaded succesfully");

#ifdef DEBUG
    std::cout << "Config loaded" << std::endl;
#endif // DEBUG

    /*
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
  */

    string i;
    cin >> i;
    return 0;
}

void initLoRa(JSON::JSON &config, JSON::JSON &user_config) {
    if (user_config.open(USER_CONFIG_PATH)) // Checks if user_config exists
    {
        // Setting up SPI
        if (user_config.hasSPI()) {
            LoRa.setSPIPort(user_config.getSPI());
#ifdef DEBUG
            std::cout << "spi "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setSPIFrequency(config.getSPI());
        }

        if (user_config.hasSPI_frequency()) {
            LoRa.setSPIPort(user_config.getSPI_frequency());
#ifdef DEBUG
            std::cout << "spi_frequency "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setSPIFrequency(config.getSPI_frequency());
        }

        // Set transmission frequency
        if (user_config.hasFrequency()) {
            LoRa.setFrequency(user_config.getFrequency());
#ifdef DEBUG
            std::cout << "frequency "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setFrequency(config.getFrequency());
        }

        // Set Pins
        if (user_config.hasSS()) {
            LoRa.setSS(user_config.getSS_pin());
#ifdef DEBUG
            std::cout << "ss "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setSS(config.getSS_pin());
        }

        if (user_config.hasReset()) {
            LoRa.setReset(user_config.getResetPin());
#ifdef DEBUG
            std::cout << "reset "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setReset(config.getResetPin());
        }

        if (user_config.hasDIO0()) {
            LoRa.setDIO0(user_config.getDIO0_pin());
#ifdef DEBUG
            std::cout << "dio0 "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setDIO0(config.getDIO0_pin());
        }

        // Set Tx power and RFO pin
        if (user_config.hasPower()) {
            LoRa.setTxPower(user_config.getPower(), config.getPAboostPin());
#ifdef DEBUG
            std::cout << "power "
                      << "user_config" << endl;
#endif // DEBUG
        } else if (user_config.hasPAboost()) {
            LoRa.setTxPower(config.getPower(), user_config.getPAboostPin());
#ifdef DEBUG
            std::cout << "PAboost "
                      << "user_config" << endl;
#endif // DEBUG
        } else if (user_config.hasPower() && user_config.hasPAboost()) {
            LoRa.setTxPower(user_config.getPower(),
                            user_config.getPAboostPin());
#ifdef DEBUG
            std::cout << "power; PAboost "
                      << "user_config" << endl;
#endif // DEBUG
        } else {
            LoRa.setTxPower(config.getPower(), config.getPAboostPin());
        }
    } else {
        // Setting up SPI
        LoRa.setSPIPort(config.getSPI());
        LoRa.setSPIFrequency(config.getSPI_frequency());

        // Set transmission frequency
        LoRa.setFrequency(config.getFrequency());

        // Set Pins
        LoRa.setPins(config.getSS_pin(), config.getResetPin(),
                     config.getDIO0_pin());

        // Set Tx power and RFO pin
        LoRa.setTxPower(config.getPower(), config.getPAboostPin());
    }
}

void handlerSIGTERM(int signum) {
    // Cleaning up LoRa
    LoRa.end();
    exit(signum);
}

void handlerSIGABRT(int signum) {
    // Cleaning up LoRa
    LoRa.end();
    exit(signum);
}

void handlerSIGINT(int signum) {
    // Cleaning up LoRa
    LoRa.end();
    exit(signum);
}
