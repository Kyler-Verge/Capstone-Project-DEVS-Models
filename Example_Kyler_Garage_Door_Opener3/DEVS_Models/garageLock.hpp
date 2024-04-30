/**
 * Kyler Verge
 * Carleton University
 * December 5th, 2023
 *
 * Updated version of Garage_Door_Opener
 *
 *
 * An atomic DEVS model for the Embedded Cadmium GarageDoorOpener example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * garageLock represents the lock for the garageSystem.  The garageLock receives
 * x and y input values (coordinates) from the joystickInput.  The input values
 * are converted to an integer, based on the diagram below and added to the password.
 * The in port, corresponding to a digitalInput, is used to confirm the password.
 * When the password is confirmed, the authorized state is sent to the garageDoor.
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


#ifndef __GARAGE_LOCK_HPP__
#define __GARAGE_LOCK_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::garageSystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct GarageLockState {

        // sigma is a mandatory variable for atomic models, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        double temperatureFound;
        bool authorized;
        std::string password;
        int xCoordinate;
        int yCoordinate;

        std::string currentStatus; //String used to display current info on the garage door and PW attempt
        std::string frozenStatus; //New String used to display status of garage door being frozen or not
        int inputNumber; //Number used to display inputs on the lcd screen

        // Set the default values for the state constructor for this specific model
        GarageLockState(): sigma(0), temperatureFound(0), authorized(false), password(""), xCoordinate(0), yCoordinate(0), inputNumber(0)  {}
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
    std::ostream& operator<<(std::ostream &out, const GarageLockState& state) {
        out << ",PasswordEntered: " << state.password << ",Authorized: " << state.authorized
            << ",xCoordinate: " << state.xCoordinate << ",yCoordinate: " << state.yCoordinate;
        return out;
    }
#endif

    // Atomic DEVS model of GarageLock
    class GarageLock : public Atomic<GarageLockState> {
     private:

     public:

        // Declare ports for the model
        // Input ports
        Port<int> inX;
        Port<int> inY;
        Port<bool> inInput;
        Port<bool> inSubmit;
        Port<double> acquiredTemperature;

        // Output ports
        Port<bool> out;
        Port<std::string> lcdStatus;
        Port<std::string> lcdFrozenStatus;

        // (NOT APPLICABLE FOR THIS MODEL) Declare variables for the model's behaviour

        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a GarageLock object and a GarageLockState object
         * are created, using the same id.
         *
         * @param id ID of the new GarageLock model object.
         */
        GarageLock(const std::string& id): Atomic<GarageLockState>(id, GarageLockState()) {

            // Initialize ports for the model

            // Input ports
            inX = addInPort<int>("inX");
            inY = addInPort<int>("inY");
            inInput  = addInPort<bool>("inInput");
            inSubmit = addInPort<bool>("inSubmit");
            acquiredTemperature = addInPort<double>("acquiredTemperature");

            // Output ports
            out = addOutPort<bool>("out");

            //lcdStatus = addOutPort<std::string>("lcdStatus");
            lcdStatus = addOutPort<std::string>("lcdToggle");
            lcdFrozenStatus = addOutPort<std::string>("lcdFrozen");

            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            state.sigma = 0.1;

            //Initial string display on lcd screen
            state.currentStatus = "BSP_LCD_DrawString(0,0,Garage Door Opener 3,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,1,TLeft=2 TRight=1,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,2,BLeft=3 BRight=4,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.currentStatus = "BSP_LCD_DrawString(0,3,TopInput BottomSubmit,LCD_WHITE)";
            lcdStatus->addMessage(state.currentStatus);
            state.frozenStatus = "BSP_LCD_DrawString(0,7,.,LCD_WHITE)";
            lcdFrozenStatus->addMessage(state.frozenStatus);
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
        void internalTransition(GarageLockState& state) const override {
            if (state.authorized == true) {
                state.authorized = false;
            }
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
        void externalTransition(GarageLockState& state, double e) const override {

            // First check if there are un-handled inputs for the "in" port
            if(!inInput->empty()){

                // The variable i is created to handle the external input values in sequence.
                // The getBag() function is used to get the next input value.
                for( const auto i : inInput->getBag()){

                    if (i==0){

                        // If the button (associated with the in port) was pressed, then the
                        // x and y coordinates determine the entered password digit

                        if ((state.xCoordinate > 600)&&(state.yCoordinate > 600)){
                            state.password.append("1");
                            state.currentStatus = "BSP_LCD_DrawString(" + std::to_string(state.inputNumber) + ",4,1,LCD_WHITE)";
                            state.inputNumber++;
                        } else if ((state.xCoordinate < 400)&&(state.yCoordinate > 600)){
                            state.password.append("2");
                            state.currentStatus = "BSP_LCD_DrawString(" + std::to_string(state.inputNumber) + ",4,2,LCD_WHITE)";
                            state.inputNumber++;
                        } else if ((state.xCoordinate < 400)&&(state.yCoordinate < 400)){
                            state.password.append("3");
                            state.currentStatus = "BSP_LCD_DrawString(" + std::to_string(state.inputNumber) + ",4,3,LCD_WHITE)";
                            state.inputNumber++;
                        } else if ((state.xCoordinate > 600)&&(state.yCoordinate < 400)){
                            state.password.append("4");
                            state.currentStatus = "BSP_LCD_DrawString(" + std::to_string(state.inputNumber) + ",4,4,LCD_WHITE)";
                            state.inputNumber++;

                        }
                    }
                }
            }

            //If bottom button on BSP is pressed, submit password attempt
            if(!inSubmit->empty()){
                for( const auto i : inSubmit->getBag()){
                    if (i==0){
                        std::string currentPassword = "1234";
                        if (state.password.compare(currentPassword) == 0){
                            state.authorized = true;
                        }
                        state.password = "";
                        state.currentStatus = "BSP_LCD_DrawString(0,4,       ,LCD_WHITE)";
                        state.inputNumber = 0;
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

            if(!acquiredTemperature->empty()) {
                for(const auto i : acquiredTemperature->getBag()){
                    state.temperatureFound = i;
                    if(state.temperatureFound <= 24.0){
                        state.frozenStatus = "BSP_LCD_DrawString(0,7,FROZEN,LCD_WHITE)";

                    }
                    else{
                        state.frozenStatus = "BSP_LCD_DrawString(0,7,WORKING,LCD_WHITE)";
                    }

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
        void output(const GarageLockState& state) const override {
            out->addMessage(state.authorized);
            lcdStatus->addMessage(state.currentStatus);
            lcdFrozenStatus->addMessage(state.frozenStatus);

        }

        /**
         * Returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const GarageLockState& state) const override {
            return state.sigma;
        }
    };
} // namespace cadmium::garageLockSystem

#endif // __GARAGE_LOCK_HPP__
