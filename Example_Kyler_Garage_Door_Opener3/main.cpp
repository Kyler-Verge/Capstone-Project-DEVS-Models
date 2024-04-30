#include <simulation/rt_root_coordinator.hpp>
#include <limits>
#ifdef EMBED
    #include <simulation/rt_clock/ti_clock.hpp>
    #ifndef NO_LOGGING
        #include <simulation/logger/stdout.hpp>
    #endif
#else
    #include <simulation/rt_clock/chrono.hpp>
    #ifndef NO_LOGGING
        #include <simulation/logger/csv.hpp>
    #endif
#endif


// We must include our "top model" which is a coupled model used to
// hold other models inside of it
#include <garageSystem.hpp>

using namespace cadmium::garageSystem;

int main(int argc,char* argv[]){

    // Declare and initialize the top model
    auto model = std::make_shared<garageSystem>("garageSystem");

#ifdef EMBED
    BSP_LCD_Init(); // can comment this line out if not using the LCD screen - it will reduce embedded startup time
    auto clock = cadmium::TIClock();
    auto rootCoordinator = cadmium::RealTimeRootCoordinator(model,clock);
    #ifndef NO_LOGGING
        rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");
    #endif
#else
    auto rootCoordinator = cadmium::RootCoordinator(model);
    #ifndef NO_LOGGING

        // For simulation purposes, set the name of the output file
        rootCoordinator.setLogger<cadmium::CSVLogger>("garageLog.csv",",");
    #endif
#endif
    rootCoordinator.start();
#ifdef EMBED
    // For embedded execution, we want to simulate the entire time we are in debug mode
    rootCoordinator.simulate(std::numeric_limits<double>::infinity());
#else
    // For simulations, we can set the number of seconds we want to simulate
    rootCoordinator.simulate(20.0);
#endif
    rootCoordinator.stop();
    return 0;
}
