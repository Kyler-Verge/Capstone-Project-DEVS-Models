/**
 * Hazel Griffith
 * ARSLab - Carleton University
 * August 21st, 2023
 *
 * An atomic DEVS model for the Embedded Cadmium GarageDoorOpener example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * The garageDoor model represents the door in the garageSystem. When the garageDoor
 * receives a true/false authorization from the in port (the garageLock), it sends
 * a message to the out port (DigitalOutput) to cause the red LED on the Boosterpack
 * to turn on/off.
 */


#ifndef __GARAGE_DOOR_HPP__
#define __GARAGE_DOOR_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::garageSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct GarageDoorState {

        // sigma is a mandatory variable for atomic models, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        bool lightOn;

        // Set the default values for the state constructor for this specific model
        GarageDoorState(): sigma(0), lightOn(false)  {}
    };
#if !defined NO_LOGGING || !defined EMBED
    /**
     * This function is used to output the current state of the model to the .csv output
     * file after each internal transition.  This is used to verify the accuracy of the
     * simulation.
     *
     * In this model, each time the internal transition function is invoked the current
     * output from the out port is listed, followed by the model's current state for
     * state.lightOn.
     *
     * @param out output stream.
     * @param s state to be represented in the output stream.
     * @return output stream with lightOn already inserted.
     */
    std::ostream& operator<<(std::ostream &out, const GarageDoorState& state) {
        out << ",Light: " << state.lightOn;
        return out;
    }
#endif

    // Atomic DEVS model of GarageDoor.
    class GarageDoor : public Atomic<GarageDoorState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<bool> in;

        // Output ports
        Port<bool> outLED;

        // Declare variables for the model's behaviour
        // (NOT APPLICABLE TO THIS MODEL)


        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a GarageDoor object and a GarageDoorState object
         * are created, using the same id.
         *
         * @param id ID of the new GarageDoor model object.
         */
        GarageDoor(const std::string& id): Atomic<GarageDoorState>(id, GarageDoorState()) {

            // Initialize ports for the model

            // Input Ports
            in  = addInPort<bool>("in");

            // Output Ports
            outLED = addOutPort<bool>("outLED");

            //lcdToggle = addOutPort<std::string>("lcdToggle");

            // Initialize variables for the model's behvaiour
            // (NOT APPLICABLE TO THIS MODEL)


            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            state.sigma = 0.1;

        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0.
         *
         * In this model, the transition function does nothing.
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(GarageDoorState& state) const override {

        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model.
         *
         * In this Model, GarageDoorState::lightOn is updated if authorization is received.
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         * @param x reference to the model input port set.
         */
        void externalTransition(GarageDoorState& state, double e) const override {

            // First check if there are un-handled inputs for the "in" port
            if(!in->empty()){

                // The variable x is created to handle the external input values in sequence.
                // The getBag() function is used to get the next input value.
                for( const auto x : in->getBag()){
                    if (x==true){
                        state.lightOn = !state.lightOn; // if authorized, we update the state of our model to turn on the LED
                    }
                }
            }
        }

        /**
         * This function outputs any desired state values to their associated ports.
         *
         * In this model, it outputs the already processed GarageDoorState::lightOn
         * via the outLED port.  The I/O model that receives the message (digitalOutput)
         * will update the status of the LED.
         *
         * @param state reference to the current model state.
         */
        void output(const GarageDoorState& state) const override {

            outLED->addMessage(state.lightOn);

        }

        /**
         * Returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const GarageDoorState& state) const override {
            return state.sigma;
        }
    };
} // namespace cadmium::GarageDoorSystem

#endif // __GARAGE_DOOR_HPP__
