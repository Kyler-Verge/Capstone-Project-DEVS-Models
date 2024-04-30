/**
 * Kyler Verge
 * Carleton University
 * November 13th, 2023
 *
 * A coupled DEVS model for the Embedded Cadmium GarageDoorOpener example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * garageSystem is the top model for this example.  It contains the garageLock
 * and garageDoor, and connects these atomic models to IO Models (digitalInput,
 * joystickInput, and digitalOutput) as defined in the IO_Models folder.
 *
 * The joystick is divided into 4 numbered quadrants, similar to a Cartesian
 * plane, with an additional section in the centre.  The user moves the joystick
 * to one of the numbered quadrants seen below, and presses the top button on the
 * Boosterpack to enter each digit of a 4-digit password.  After entering the
 * correct password (1234) the user moves the joystick to the centre and presses
 * the top button again to confirm.  If the password was correctly entered, the
 * red LED on the Boosterpack will be illuminated.  The user can then re-enter
 * the password and confirm it to turn off the LED.
 *
 *     _____________
 *    |      |      |
 *    |  2 __|__ 1  |
 *    |___|     |___|
 *    |   |_____|   |
 *    |  3   |   4  |
 *    |______|______|
 *
 */

#ifndef __GARAGE_SYSTEM_HPP__
#define __GARAGE_SYSTEM_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <modeling/devs/coupled.hpp>

#ifdef EMBED
    #include "../../IO_Models/accelerometerInput.hpp"
    #include "../../IO_Models/digitalInput.hpp"
    #include "../../IO_Models/digitalOutput.hpp"
    #include "../../IO_Models/joystickInput.hpp"
    #include "../../IO_Models/lcdOutput.hpp"
    #include "../../IO_Models/lightSensorInput.hpp"
    #include "../../IO_Models/microphoneInput.hpp"
    #include "../../IO_Models/pwmOutput.hpp"
    #include "../../IO_Models/temperatureSensorInput.hpp"
#else
    #include <lib/iestream.hpp>
#endif

// We include any models that are directly contained within this coupled model
#include "garageLock.hpp"
#include "garageDoor.hpp"
#include "temperatureGarage.hpp"

namespace cadmium::garageSystem {
    class garageSystem : public Coupled {
        public:
        garageSystem(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto garageLock = addComponent<GarageLock>("garageLock");
            auto garageDoor = addComponent<GarageDoor>("garageDoor");
            auto temperatureGarage = addComponent<TemperatureGarage>("garageTemperature");

            // Connect any non-input/output models with coupling
            addCoupling(garageLock->out,garageDoor->in);
            addCoupling(temperatureGarage->out, garageLock->acquiredTemperature);

        #ifdef EMBED

            // Declare and initialize all embedded input/output models
            // Embedded Inputs
            auto digitalInput = addComponent<DigitalInput>("digitalInput",GPIO_PORT_P5,GPIO_PIN1);
            auto joystickInput = addComponent<JoystickInput>("joystickInput");
            auto submitInput = addComponent<DigitalInput>("submitInput", GPIO_PORT_P3,GPIO_PIN5);

            auto temperatureInput = addComponent<TemperatureSensorInput>("temperatureInput"); //MSP432 Temperature sensor

            // Embedded Outputs
            auto digitalOutput = addComponent<DigitalOutput>("digitalOutput",GPIO_PORT_P2,GPIO_PIN2);

            auto lcdOutputStatus = addComponent<LCDOutput>("lcdOutputStatus");

            auto lcdOutputTemperature = addComponent<LCDOutput>("lcdOutputTemperature");

            auto lcdOutputFrozenStatus = addComponent<LCDOutput>("lcdOutputFrozenStatus");


            // Connect IO models with coupling to the system
            // Embedded Inputs
            addCoupling(digitalInput->out,garageLock->inInput);
            addCoupling(submitInput->out,garageLock->inSubmit);
            addCoupling(temperatureInput->out, temperatureGarage->inTemperature);

            addCoupling(joystickInput->outX,garageLock->inX);
            addCoupling(joystickInput->outY,garageLock->inY);

            // Embedded Outputs
            addCoupling(garageDoor->outLED,digitalOutput->in);
            addCoupling(garageLock->lcdStatus, lcdOutputStatus->in);
            addCoupling(temperatureGarage->lcdTemperature, lcdOutputTemperature->in);
            addCoupling(garageLock->lcdFrozenStatus, lcdOutputFrozenStatus->in);

        #else

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto buttonInput = addComponent<cadmium::lib::IEStream<bool>>("buttonInput",std::move("buttonInput.txt"));
            auto buttonSubmit = addComponent<cadmium::lib::IEStream<bool>>("buttonSubmit",std::move("buttonSubmit.txt"));
            auto joyStickXInput = addComponent<cadmium::lib::IEStream<int>>("joyStickXInput",std::move("joyStickXInput.txt"));
            auto joyStickYInput = addComponent<cadmium::lib::IEStream<int>>("joyStickYInput",std::move("joyStickYInput.txt"));

            // Connect the input files to the rest of the simulation with coupling
            addCoupling(buttonInput->out,garageLock->inInput);
            addCoupling(buttonSubmit->out,garageLock->inSubmit);
            addCoupling(joyStickXInput->out,garageLock->inX);
            addCoupling(joyStickYInput->out,garageLock->inY);

        #endif
        }
    };
} // namespace cadmium::garageSystem

#endif // __GARAGE_SYSTEM_HPP__
