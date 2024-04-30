/**
 * Kyler Verge
 * Carleton University
 * November 27th, 2023
 *
 * Model that
 * Acquires temperature from msp microcontroller
 */

#ifndef __TEMPERATURE_SIGNAL_HPP__
#define __TEMPERATURE_SIGNAL_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::temperatureSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct TemperatureSignalState {

        // sigma is a mandatory variable, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        double temperatureFound;

        bool mspRedOn;
        bool mspBlueOn;

        int buzzerDuty;

        // Set the default values for the state constructor for this specific model
        TemperatureSignalState(): sigma(0), temperatureFound(0) , mspRedOn(false), mspBlueOn(false), buzzerDuty(0) {}
    };

#if !defined NO_LOGGING || !defined EMBED 
    /**
     * This function is used to output the current state of the model to the .csv output
     * file after each internal transition.  This is used to verify the accuracy of the
     * simulation.
     *
     * In this model, each time the internal transition function is invoked the current
     * output from the out port is listed, followed by the model's current state for
     * state.lightOn, and state.sigma.
     *
     * Note that state.sigma is NOT mandatory to include here, but it is listed due to
     * the desired program logic.
     *
     * @param out output stream.
     * @param s state to be represented in the output stream.
     * @return output stream with sigma and lightOn already inserted.
     */
    std::ostream& operator<<(std::ostream &out, const TemperatureSignalState& state) {
        out << ", Temperature: " << state.temperatureFound;
        return out;
    }
#endif

    // Atomic DEVS model of TrafficLightTemperature
    class TemperatureSignal : public Atomic<TemperatureSignalState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<double> acquiredTemperature;

        //Output ports
        Port<bool> outMspRed; //Update 1
        Port<bool> outMspBlue; //Update 1

        Port<int> outBuzzer; //Update 2

        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a trafficlightTemperature object and a TrafficLightTemperatureState object
         * are created, using the same id.
         *
         * @param id ID of the new trafficlightTemperature model object, will be used to identify results on the output file
         */
        TemperatureSignal(const std::string& id): Atomic<TemperatureSignalState>(id, TemperatureSignalState()) {

            // Initialize ports for the model

            // Input Ports
            acquiredTemperature = addInPort<double>("acquiredTemperature");

            // Output Ports
            outMspRed = addOutPort<bool>("outMspRed"); //Update 1
            outMspBlue = addOutPort<bool>("outMspBlue"); //Update 1

            outBuzzer = addOutPort<int>("outBuzzer"); //Update 2

            // Initialize variables for the model's behavior

            state.sigma = 1.0;

        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0.
         *
         * In this model, the value of state.lightOn is toggled.
         *
         * With each invoke, the rgbCounter increments,
         * then loops after case 3 back to case 0.
         * sets the state of Red and Green lights
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(TemperatureSignalState& state) const override {


        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         */
        void externalTransition(TemperatureSignalState& state, double e) const override {

            if(!acquiredTemperature->empty()) {
                for(const auto i : acquiredTemperature->getBag()){
                    state.temperatureFound = i;
                    if(state.temperatureFound < 26.5){
                        state.mspBlueOn = true;
                        state.mspRedOn = false;
                        state.buzzerDuty = 0;
                    }
                    else{
                        state.mspRedOn = true;
                        state.mspBlueOn = false;
                        state.buzzerDuty = 5;
                    }

                }
            }

        }

        /**
         * This function outputs any desired state values to their associated ports.
         *
         * In this model, the value of state.lightOn is sent via the out port.  Once
         * the value of state.ligthOn reaches the I/O model, that model will update
         * the status of the LED.
         *
         * @param state reference to the current model state.
         */
        void output(const TemperatureSignalState& state) const override {

            outMspRed->addMessage(state.mspRedOn);
            outMspBlue->addMessage(state.mspBlueOn);

            outBuzzer->addMessage(state.buzzerDuty);

        }

        /**
         * It returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const TemperatureSignalState& state) const override {

            return state.sigma;

        }
    };
} // namespace cadmium::

#endif // __TEMPERATURE_HPP__
