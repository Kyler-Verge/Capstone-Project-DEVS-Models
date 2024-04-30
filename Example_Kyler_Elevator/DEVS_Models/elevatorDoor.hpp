/**
 * Kyler Verge
 * Carleton University
 * February 4th, 2024
 *
 * An atomic DEVS model for the Embedded Cadmium Elevator example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * The elevatorDoor model represents the door in the elevatorSystem. When the elevatorDoor gets
 * an input value from elevatorNum that doesn't equal the stored value, the elevatorDoor will
 * turn on the blue LED signaling the door is now closed. This state will be sent to elevatorNum
 * to block any further inputs. This input value is then sent to elevatorMove to handle moving
 * logic
 *
 */


#ifndef __ELEVATOR_DOOR_HPP__
#define __ELEVATOR_DOOR_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::elevatorSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct ElevatorDoorState {

        // sigma is a mandatory variable for atomic models, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        int floorNum;
        int floorNumToMove;
        bool lightOn;
        std::string currentStatus; //String used to display current info on elevatorDoor LOG

        // Set the default values for the state constructor for this specific model
        ElevatorDoorState(): sigma(0), floorNum(1), floorNumToMove(1), lightOn(false), currentStatus("") {}
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
    std::ostream& operator<<(std::ostream &out, const ElevatorDoorState& state) {
        out << ", DoorStatus: " << state.currentStatus << ",DoorLight: " << state.lightOn << ",DoorFloorNum: " << state.floorNum << ",DoorFloorNumToMove: " << state.floorNumToMove;
        return out;
    }
#endif

    // Atomic DEVS model of GarageDoor.
    class ElevatorDoor : public Atomic<ElevatorDoorState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<int> inElevatorNum;
        Port<int> inElevatorMove;

        // Output ports
        Port<int> outFloorToMove;
        Port<bool> outDoorStatus;

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
        ElevatorDoor(const std::string& id): Atomic<ElevatorDoorState>(id, ElevatorDoorState()) {

            // Initialize ports for the model

            // Input Ports
            inElevatorNum  = addInPort<int>("inElevatorNum");

            inElevatorMove = addInPort<int>("inElevatorMove");

            // Output Ports
            outFloorToMove = addOutPort<int>("outFloorToMove");

            outDoorStatus = addOutPort<bool>("outDoorStatus");

            // Initialize variables for the model's behvaiour

            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            //state.sigma = std::numeric_limits<double>::infinity(); //EMBED
            state.sigma = 0.11; //LOG

        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0.
         *
         * In this model, the transition function does nothing.
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(ElevatorDoorState& state) const override {

            if(state.lightOn == true){
                state.sigma = std::numeric_limits<double>::infinity(); //EMBED
                //state.currentStatus.append("$");

            }
            //state.sigma = std::numeric_limits<double>::infinity(); //EMBED
            else if(state.lightOn == false){
                state.sigma = 0.11;
                //state.currentStatus.append("!");

            }

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
        void externalTransition(ElevatorDoorState& state, double e) const override {

            // First check if there are un-handled inputs for the "inElevatorNum" port
            if(!inElevatorNum->empty()){

                // The variable x is created to handle the external input values in sequence.
                // The getBag() function is used to get the next input value.
                for(int x : inElevatorNum->getBag()){

                    //Received floor input equals the floor num this atomic model currently holds. So open the door (lightOn = false)
                    if (x==state.floorNum){
                        state.lightOn = false;
                        state.sigma = 0.11;
                        //state.currentStatus.append("#");

                    }
                    else{
                        //Received floor input does not equal the floor num this atomic model currently holds
                        //This means the elevator will have to move, so the door will close (lightOn = true)
                        state.lightOn = true;
                        state.floorNumToMove = x;
                        //state.currentStatus.append("?");
                    }
                    //state.sigma = 0.12;
                }
            }


            //Receive input from elevatorMove, the current floor number the elevatorMove model is on
            if(!inElevatorMove->empty()){

                for(int y : inElevatorMove->getBag()){
                    state.floorNum = y;
                }
            }

            //Receive input from elevatorMove, the current status of the buzzer indicating movement
            //if(!inElevatorBuzz->empty()){
                //for(int z : inElevatorBuzz->getBag()){
                    //.buzzer = z;
                    //If the current floorNum is equal to floorNumToMove and the buzzer is off, indicates the elevator has stopped moving

                //}
            //}
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
        void output(const ElevatorDoorState& state) const override {

            outDoorStatus->addMessage(state.lightOn);
            outFloorToMove->addMessage(state.floorNumToMove);

        }

        /**
         * Returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const ElevatorDoorState& state) const override {
            return state.sigma;
        }
    };
} // namespace cadmium::elevatorDoorSystem

#endif // __ELEVATOR_DOOR_HPP__
