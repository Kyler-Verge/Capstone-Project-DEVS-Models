/**
 * Kyler Verge
 * Carleton University
 * March 8th, 2024
 *
 * An atomic DEVS model for the Embedded Cadmium Elevator example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * This atmoic model takes input from the microcontrollers analog stick and top face input
 * button. Based on if the door is open or closed, this model will acquire input from the user
 * and pass it on to the elevatorDoor atomic model.
 *
 *     _____________
 *    |      |      |
 *    |  1 __|__ 2  |
 *    |___|     |___|
 *    |   |_____|   |
 *    |  3   |   4  |
 *    |______|______|
 *
 */


#ifndef __ELEVATOR_NUM_HPP__
#define __ELEVATOR_NUM_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::elevatorSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct ElevatorNumState {

        // sigma is a mandatory variable for atomic models, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        int xCoordinate;
        int yCoordinate;
        int floorNum; // Variable to keep track of floor number picked

        bool doorStatus; //Variable to keep track of the doors status

        std::string currentStatus; //String used to display current info on elevatorNum LOG

        // Set the default values for the state constructor for this specific model
        ElevatorNumState(): sigma(0), xCoordinate(0), yCoordinate(0), floorNum(1), doorStatus(false), currentStatus(""){}
    };

#if !defined NO_LOGGING || !defined EMBED
    /**
     * This function is used to output the current state of the model to the .csv output
     * file after each internal transition.  This is used to verify the accuracy of the
     * simulation.
     *
     * Insertion operator for GarageLockState objects.
     * It displays the value of PasswordEntered, authorized, xCoordinate, and yCoordinate.
     *
     * @param out output stream.
     * @param s state to be represented in the output stream.
     * @return output stream with password, authorized, x and y coordinates already inserted.
     */
    std::ostream& operator<<(std::ostream &out, const ElevatorNumState& state) {
        out << ",FloorNumberInputs: " << state.currentStatus << ",xCoordinate: " << state.xCoordinate << ",yCoordinate: " << state.yCoordinate << ",elevNumDoorStatus: " << state.doorStatus;
        return out;
    }
#endif

    // Atomic DEVS model of GarageLock
    class ElevatorNum : public Atomic<ElevatorNumState> {
     private:

     public:

        // Declare ports for the model
        // Input ports
        Port<int> inX;
        Port<int> inY;
        Port<bool> inInput;

        //Get input from elevatorDoor of status of Door
        Port<bool> inDoorStatus;

        // Output ports
        Port<int> out;

        // (NOT APPLICABLE FOR THIS MODEL) Declare variables for the model's behaviour

        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a GarageLock object and a GarageLockState object
         * are created, using the same id.
         *
         * @param id ID of the new GarageLock model object.
         */
        ElevatorNum(const std::string& id): Atomic<ElevatorNumState>(id, ElevatorNumState()) {

            // Initialize ports for the model

            // Input ports
            inX = addInPort<int>("inX");
            inY = addInPort<int>("inY");
            inInput  = addInPort<bool>("inInput");
            inDoorStatus = addInPort<bool>("inDoorStatus");

            // Output ports
            out = addOutPort<int>("out");

            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            state.sigma = 0.11;
        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0.
         *
         * In this model, it updates the GarageLock state authorized
         * to false if it is true.
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(ElevatorNumState& state) const override {

        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model.
         *
         * In this model, it updates GarageLockState::xCoordinate::yCoordinate,
         * appends new values to the entered password, and checks if entered
         * password matches the expected password.
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         */
        void externalTransition(ElevatorNumState& state, double e) const override {

            // First check if there are un-handled inputs for the "in" port
            if(!inInput->empty()){

                // The variable i is created to handle the external input values in sequence.
                // The getBag() function is used to get the next input value.
                for( const auto i : inInput->getBag()){

                    //If door is currently open, take a floor number input
                    if (state.doorStatus == false){

                        if (i==0){

                            // If the button (associated with the in port) was pressed, then the
                            // x and y coordinates determine the entered password digit

                            if ((state.xCoordinate < 400)&&(state.yCoordinate > 600)&&(state.floorNum != 1)){
                                state.floorNum = 1;
                                state.currentStatus.append("1 ");

                            } else if ((state.xCoordinate > 600)&&(state.yCoordinate > 600)&&(state.floorNum != 2)){
                                state.floorNum = 2;
                                state.currentStatus.append("2 ");

                            } else if ((state.xCoordinate < 400)&&(state.yCoordinate < 400)&&(state.floorNum != 3)){
                                state.floorNum = 3;
                                state.currentStatus.append("3 ");

                            } else if ((state.xCoordinate > 600)&&(state.yCoordinate < 400)&&(state.floorNum != 4)){
                                state.floorNum = 4;
                                state.currentStatus.append("4 ");
                            }
                        }
                    }
                    else{
                        state.currentStatus.append("DC ");

                    }
                }
            }

            // We then repeat the process of checking for un-handled inputs, getting the next value,
            // and updating the associated coordinates based on the position of the joystick

            if(!inX->empty()){
                for( const auto x : inX->getBag()){
                    state.xCoordinate = x;
                }
            }

            if(!inY->empty()){
                for( const auto y : inY->getBag()){
                    state.yCoordinate = y;
                }
            }

            //Get and set the door status
            if(!inDoorStatus->empty()){
                for( const auto d : inDoorStatus->getBag()){
                    state.doorStatus = d;
                    //state.currentStatus.append("Dor ");
                }
            }
        }

        /**
         * This function outputs any desired state values to their associated ports.
         *
         * In this model, it outputs the already processed GarageLockState::authorized
         * via the out port.  The model that receives the message (garageDoor) will have
         * an external transition triggered.
         *
         * @param state reference to the current model state.
         */
        void output(const ElevatorNumState& state) const override {

            out->addMessage(state.floorNum);

        }

        /**
         * Returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const ElevatorNumState& state) const override {
            return state.sigma;
        }
    };
} // namespace cadmium::elevatorNumSystem

#endif // __ELEVATOR_NUM_HPP__
