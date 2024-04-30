/**
 * Kyler Verge
 * Carleton University
 * March 8th, 2024
 *
 * This atmoic model takes input from elevatorDoor atmoic model, a floor number.
 * This input is compared to the current floor number this model as stored.
 * This comparison is done in intervals of 2.0 seconds, based on whether it is
 * below or above, the stored value will be incremented by one. Once the stored value
 * is equivalent to the input value, the elevator will stop "moving"
 *
 */


#ifndef __ELEVATOR_MOVE_HPP__
#define __ELEVATOR_MOVE_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::elevatorSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct ElevatorMoveState {

        // sigma is a mandatory variable for atomic models, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        int floorNum;
        int floorToMove;
        int buzzerDuty;

        std::string currentStatus; //String used to display current info on the garage door

        std::string currentStatuss;

        // Set the default values for the state constructor for this specific model
        ElevatorMoveState(): sigma(0), floorNum(1), floorToMove(1), buzzerDuty(0), currentStatuss("")  {}
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
    std::ostream& operator<<(std::ostream &out, const ElevatorMoveState& state) {
        out << ", MoveStatus: " << state.currentStatuss << ",MoveFloorNum: " << state.floorNum << ",MoveFloorToMove: " << state.floorToMove;
        return out;
    }
#endif

    // Atomic DEVS model of GarageDoor.
    class ElevatorMove : public Atomic<ElevatorMoveState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<int> inMoveFloor;

        // Output ports
        Port<int> outMoveFloor;
        Port<int> outMoveBuzzer;

        Port<std::string> lcdStatus;

        // Declare variables for the model's behaviour
        // (NOT APPLICABLE TO THIS MODEL)


        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a GarageDoor object and a ElevatorMoveState object
         * are created, using the same id.
         *
         * @param id ID of the new GarageDoor model object.
         */
        ElevatorMove(const std::string& id): Atomic<ElevatorMoveState>(id, ElevatorMoveState()) {

            // Initialize ports for the model

            // Input Ports
            inMoveFloor  = addInPort<int>("inMoveFloor");

            // Output Ports
            outMoveFloor = addOutPort<int>("outMoveFloor");
            outMoveBuzzer = addOutPort<int>("outMoveBuzzer");

            lcdStatus = addOutPort<std::string>("lcdStatus");


            // Initialize variables for the model's behvaiour
            // (NOT APPLICABLE TO THIS MODEL)


            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            state.sigma = 2.0; //EMBED

            //Initial string display on lcd screen
            state.currentStatus = "BSP_LCD_DrawString(0,0,Elevator V1.30,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,1,TL=1 TR=2,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,2,BL=3 BR=4,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,3,TopButtonInput,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,5,DFloor:" + std::to_string(state.floorToMove) + " CFloor:" + std::to_string(state.floorNum) + ",LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0. sigma is 2.0, meaning every 2 seconds
         *
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(ElevatorMoveState& state) const override {

            //TEST if internalTransition gets invoked
            if(state.floorNum < state.floorToMove){
                state.buzzerDuty = 2;
                state.floorNum += 1;
                state.currentStatus = "BSP_LCD_DrawString(0,5,DFloor:" + std::to_string(state.floorToMove) + " CFloor:" + std::to_string(state.floorNum) + ",LCD_WHITE)";
                //state.currentStatuss.append("U "); //LOG
            }
            else if(state.floorNum > state.floorToMove){
                state.buzzerDuty = 2;
                state.floorNum -= 1;
                state.currentStatus = "BSP_LCD_DrawString(0,5,DFloor:" + std::to_string(state.floorToMove) + " CFloor:" + std::to_string(state.floorNum) + ",LCD_WHITE)";
                //state.currentStatuss.append("D "); //LOG
            }
            else if(state.floorNum == state.floorToMove){
                state.buzzerDuty = 0;
            }

        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model.
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         * @param x reference to the model input port set.
         */
        void externalTransition(ElevatorMoveState& state, double e) const override {

            if(!inMoveFloor->empty()){
                for(int x : inMoveFloor->getBag()){
                    if(state.floorToMove != x){
                        state.floorToMove = x;
                        state.currentStatuss.append("IN:" + std::to_string(x)); //LOG
                    }
                }
            }
        }

        /**
         * This function outputs any desired state values to their associated ports.
         *
         * In this model, it outputs the already processed ElevatorMoveState::lightOn
         * via the outLED port.  The I/O model that receives the message (digitalOutput)
         * will update the status of the LED.
         *
         * @param state reference to the current model state.
         */
        void output(const ElevatorMoveState& state) const override {

            outMoveFloor->addMessage(state.floorNum);
            outMoveBuzzer->addMessage(state.buzzerDuty);
            lcdStatus->addMessage(state.currentStatus);

        }

        /**
         * Returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const ElevatorMoveState& state) const override {
            return state.sigma;
        }
    };
} // namespace cadmium::ElevatorSystem

#endif // __ELEVATOR_MOVE_HPP__
