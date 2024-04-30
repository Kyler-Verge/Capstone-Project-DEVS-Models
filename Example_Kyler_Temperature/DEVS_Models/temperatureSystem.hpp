/**
 * Kyler Verge
 * Carleton University
 * November 27th, 2023
 *
 * Model of Temperature System
 *
 */

#ifndef __TEMPERATURE_SYSTEM_HPP__
#define __TEMPERATURE_SYSTEM_HPP__

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
#include <temperature.hpp>
#include <temperatureSignal.hpp>

namespace cadmium::temperatureSystem {
    class temperatureSystem : public Coupled {
        public:
        temperatureSystem(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto temperature = addComponent<Temperature>("temperature");
            auto temperatureSignal = addComponent<TemperatureSignal>("temperatureSignal");

            // Connect any non-input/output models with coupling
            addCoupling(temperature->out, temperatureSignal->acquiredTemperature);

#ifdef EMBED

            // Declare and initialize all embedded input/output models
            auto temperatureInput = addComponent<TemperatureSensorInput>("temperatureInput"); //MSP432 Temperature sensor

            //LCD Output
            auto lcdOutputTemperature = addComponent<LCDOutput>("lcdOutputTemperature");

            auto mspRed = addComponent<DigitalOutput>("mspRed", GPIO_PORT_P2,GPIO_PIN0); //MSP432 Red RGB PIN0 = Red
            auto mspBlue = addComponent<DigitalOutput>("mspBlue", GPIO_PORT_P2,GPIO_PIN2); //MSP432 Red RGB PIN2 = Blue

            auto buzzerOutput = addComponent<PWMOutput>("buzzerOutput", GPIO_PORT_P2, GPIO_PIN7);

            // Connect IO models with coupling to the system

            addCoupling(temperatureInput->out, temperature->inTemperature);

            addCoupling(temperature->lcdTemperature, lcdOutputTemperature->in);

            addCoupling(temperatureSignal->outMspRed, mspRed->in);
            addCoupling(temperatureSignal->outMspBlue, mspBlue->in);
            addCoupling(temperatureSignal->outBuzzer, buzzerOutput->in);

#else

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto textInput = addComponent<cadmium::lib::IEStream<bool>>("textInput","input.txt");

            // Connect the input files to the rest of the simulation with coupling
            addCoupling(textInput->out,trafficLight->in);

#endif
        }
    };
} // namespace cadmium::

#endif // __TEMPERATURE_SYSTEM_HPP__
