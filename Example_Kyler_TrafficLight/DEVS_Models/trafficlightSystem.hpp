/**
 * Kyler Verge
 * Carleton University
 * November 21st, 2023
 *
 * Model of Traffic Light System
 *
 */

#ifndef __TRAFFIC_LIGHT_SYSTEM_HPP__
#define __TRAFFIC_LIGHT_SYSTEM_HPP__

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
#include <trafficlight.hpp>

namespace cadmium::trafficlightSystem {
    class trafficlightSystem : public Coupled {
        public:
        trafficlightSystem(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto trafficlight = addComponent<TrafficLight>("trafficLight");

            // Connect any non-input/output models with coupling
            // (NOT APPLICABLE FOR THIS MODEL)

#ifdef EMBED

            // Declare and initialize all embedded input/output models
            auto digitalInput = addComponent<DigitalInput>("digitalInput",GPIO_PORT_P5,GPIO_PIN1);

            //PWM Outputs
            auto mspRed = addComponent<DigitalOutput>("mspRed", GPIO_PORT_P2,GPIO_PIN0); //MSP432 Red RGB PIN0 = Red
            auto mspGreen = addComponent<DigitalOutput>("mspGreen", GPIO_PORT_P2,GPIO_PIN1); //MSP432 Red RGB PIN1 = Green

            //LCD Output
            auto lcdOutputToggle = addComponent<LCDOutput>("lcdOutputToggle");
            auto lcdOutputTemperature = addComponent<LCDOutput>("lcdOutputTemperature");

            // Connect IO models with coupling to the system

            //trafficlight IO
            addCoupling(digitalInput->out,trafficlight->in);

            //Light Outputs
            addCoupling(trafficlight->outMspRed, mspRed->in);
            addCoupling(trafficlight->outMspGreen, mspGreen->in);

            //LCD Output
            addCoupling(trafficlight->lcdToggle, lcdOutputToggle->in);

#else

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto textInput = addComponent<cadmium::lib::IEStream<bool>>("textInput","input.txt");

            // Connect the input files to the rest of the simulation with coupling
            addCoupling(textInput->out,trafficLight->in);

#endif
        }
    };
} // namespace cadmium::blinkySystem

#endif // __BLINKY_SYSTEM_HPP__
