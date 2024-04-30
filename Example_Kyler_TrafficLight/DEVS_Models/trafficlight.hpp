/**
 * Kyler Verge
 * Carleton University
 * November 21st, 2023
 *
 * Model that simulates a traffic light
 * Changes from Green to Yellow to Red Lights, with times set for each
 */

#ifndef __TRAFFIC_LIGHT_HPP__
#define __TRAFFIC_LIGHT_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::trafficlightSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct TrafficLightState {

        // sigma is a mandatory variable, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        bool lightOn;
        bool fastToggle;

        bool mspRedOn;   //Red light LED2 (Update 1)
        bool mspGreenOn; //Green light LED2 (Update 1)

        int rgbCounter; //counter for the next light state

        std::string currentToggle; //String used to display

        // Set the default values for the state constructor for this specific model
        TrafficLightState(): sigma(0), lightOn(false), fastToggle(false), mspRedOn(true), mspGreenOn(false), rgbCounter(0) {}
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
    std::ostream& operator<<(std::ostream &out, const TrafficLightState& state) {
        out << ", Status: " << state.lightOn << ", sigma: " << state.sigma << ", rgbCounter: " << stage.rgbCounter
            << ", Red Light: " << state.mspRedOn << ", Green Light: " << state.mspGreenOn;
        return out;
    }
#endif

    // Atomic DEVS model of Blinky2
    class TrafficLight : public Atomic<TrafficLightState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<bool> in;

        // PWMOutput ports
        Port<bool> outMspRed;
        Port<bool> outMspGreen;

        // Declare variables for the model's behaviour
        double greenredLightTime;
        double yellowLightTime;

        Port<std::string> lcdToggle;

        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a trafficlight object and a TrafficLightState object
         * are created, using the same id.
         *
         * @param id ID of the new trafficlight model object, will be used to identify results on the output file
         */
        TrafficLight(const std::string& id): Atomic<TrafficLightState>(id, TrafficLightState()) {

            // Initialize ports for the model

            // Input Ports
            in  = addInPort<bool>("in");

            // Output Ports
            outMspRed = addOutPort<bool>("outMspRed");
            outMspGreen = addOutPort<bool>("outMspGreen");

            lcdToggle = addOutPort<std::string>("lcdToggle");

            // Initialize variables for the model's behavior
            greenredLightTime = 6.0;
            yellowLightTime = 2.0;
            state.sigma = greenredLightTime;

            //Set a string for each of the string variables, and send it to the
            //corresponding output port. This displays the initial strings on the LCD screen
            //upon debugging.
            state.currentToggle = "BSP_LCD_DrawString(0,0,Traffic Light V1,LCD_WHITE)";
            lcdToggle->addMessage(state.currentToggle);
            state.currentToggle = "BSP_LCD_DrawString(0,1, GR = 6s Y = 2s ,LCD_WHITE)";
            lcdToggle->addMessage(state.currentToggle);
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
        void internalTransition(TrafficLightState& state) const override {

            //Increment the RGB Counter , and update the RGB states accordingly

            state.rgbCounter++;
            state.rgbCounter %= 3;

            //If the Light is Green or Red, set the time to 6s
            if(state.rgbCounter == 1 || state.rgbCounter == 2){
                state.sigma = greenredLightTime;
            }
            //If the Light is Yellow, set the time to 2s
            else{
                state.sigma = yellowLightTime;
            }

            switch (state.rgbCounter) {

                        //Red Light display
                        case 0:
                            state.mspRedOn = true;
                            state.mspGreenOn = false;
                            break;

                        //Green Light display
                        case 1:
                            state.mspRedOn = false;
                            state.mspGreenOn = true;
                            break;

                        //Yellow Light display (R + G = Y)
                        case 2:;
                            state.mspRedOn = true;
                            state.mspGreenOn = true;
                            break;
                        }
        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         */
        void externalTransition(TrafficLightState& state, double e) const override {

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
        void output(const TrafficLightState& state) const override {

            outMspRed->addMessage(state.mspRedOn); //Update 1
            outMspGreen->addMessage(state.mspGreenOn); //Update 1

            lcdToggle->addMessage(state.currentToggle);

        }

        /**
         * It returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const TrafficLightState& state) const override {

            return state.sigma;

        }
    };
} // namespace cadmium::trafficlightSystem

#endif // __TRAFFIC_HPP__
