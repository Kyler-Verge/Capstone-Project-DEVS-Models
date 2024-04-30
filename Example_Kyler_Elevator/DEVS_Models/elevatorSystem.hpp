/**
 * Kyler Verge
 * Carleton University
 * March 8th, 2024
 *
 * A coupled DEVS model for the Embedded Cadmium Elevator example on the
 * MSP432P401R Microcontroller used with the Educational Boosterpack MK II.
 *
 * elevatorSystem is the top model for this example.  It contains the elevatorNum,
 * elevatorDoor, elevatorMove, and connects these atomic models to IO Models (digitalInput,
 * joystickInput, and digitalOutput) as defined in the IO_Models folder.
 *
 * The joystick is divided into 4 numbered quadrants, similar to a Cartesian
 * plane.  The user moves the joystick to one of the numbered quadrants seen below, and presses the top button on the
 * Boosterpack to enter a floor number. This input is sent to the door model, if the input is != to the floor number
 * the door model currently has it will close the door and send the door status to the elevatorNum model to lock
 * further inputs until the elevator has reached its destination. the door sends this new floor number to the
 * Move model to calculate how much time it will take to reach the destination. Once reached the Move model will
 * send a signal to the Door model to open the door so Num model can take in a input again.
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

#ifndef __ELEVATOR_SYSTEM_HPP__
#define __ELEVATOR_SYSTEM_HPP__

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
#include <elevatorNum.hpp>
#include <elevatorDoor.hpp>
#include <elevatorMove.hpp>

namespace cadmium::elevatorSystem {
    class elevatorSystem : public Coupled {
        public:
        elevatorSystem(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto elevatorNum = addComponent<ElevatorNum>("elevatorNum");
            auto elevatorDoor = addComponent<ElevatorDoor>("elevatorDoor");
            auto elevatorMove = addComponent<ElevatorMove>("elevatorMove");

            // Connect any non-input/output models with coupling
            //Number input received in elevatorNum is given to elevatorDoor
            addCoupling(elevatorNum->out,elevatorDoor->inElevatorNum);
            //elevatorDoors open or close state given to elevatorNum to lock further inputs
            addCoupling(elevatorDoor->outDoorStatus,elevatorNum->inDoorStatus);
            //elevatorDoor sends floor number to move to to elevatorMove
            addCoupling(elevatorDoor->outFloorToMove,elevatorMove->inMoveFloor);
            //elevatorMove sends floor number elevator is currently on to elevatorDoor
            addCoupling(elevatorMove->outMoveFloor,elevatorDoor->inElevatorMove);

        #ifdef EMBED

            // Declare and initialize all embedded input/output models
            // Embedded Inputs
            auto digitalInput = addComponent<DigitalInput>("digitalInput",GPIO_PORT_P5,GPIO_PIN1);
            auto joystickInput = addComponent<JoystickInput>("joystickInput");
            auto submitInput = addComponent<DigitalInput>("submitInput", GPIO_PORT_P3,GPIO_PIN5);

            // Embedded Outputs
            auto digitalOutput = addComponent<DigitalOutput>("digitalOutput",GPIO_PORT_P2,GPIO_PIN2);

            auto lcdOutputStatus = addComponent<LCDOutput>("lcdOutputStatus");

            auto buzzerOutput = addComponent<PWMOutput>("buzzerOutput", GPIO_PORT_P2, GPIO_PIN7);

            // Connect IO models with coupling to the system
            // Embedded Inputs
            addCoupling(digitalInput->out,elevatorNum->inInput);

            addCoupling(joystickInput->outX,elevatorNum->inX);
            addCoupling(joystickInput->outY,elevatorNum->inY);

            // Embedded Outputs
            //Turn on blue LED on MSP when elevatorDoor outLED = true
            addCoupling(elevatorDoor->outDoorStatus,digitalOutput->in);
            //elevatorNum LCD output displayed on MSP LCD screen
            //addCoupling(elevatorNum->lcdStatus, lcdOutputStatus->in);
            addCoupling(elevatorMove->lcdStatus, lcdOutputStatus->in);
            //Buzzer turns on when elevator is moving a floor
            addCoupling(elevatorMove->outMoveBuzzer, buzzerOutput->in);


        #else

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto buttonInput = addComponent<cadmium::lib::IEStream<bool>>("buttonInput",std::move("buttonInput.txt"));
            auto joyStickXInput = addComponent<cadmium::lib::IEStream<int>>("joyStickXInput",std::move("joyStickXInput.txt"));
            auto joyStickYInput = addComponent<cadmium::lib::IEStream<int>>("joyStickYInput",std::move("joyStickYInput.txt"));

            // Connect the input files to the rest of the simulation with coupling
            addCoupling(buttonInput->out,elevatorNum->inInput);
            addCoupling(joyStickXInput->out,elevatorNum->inX);
            addCoupling(joyStickYInput->out,elevatorNum->inY);

        #endif
        }
    };
} // namespace cadmium::elevatorSystem

#endif // __ELEVATOR_SYSTEM_HPP__
