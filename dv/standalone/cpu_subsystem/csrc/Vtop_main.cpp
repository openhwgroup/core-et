// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: main() simulation loop, created with --main

#include "verilated.h"
#include "verilated_vpi.h"
#include "Vtop.h"
#include "et_tb.hpp"

#include <iostream>

//======================

std::size_t sim_ticks = 0;
bool finish_timer_set = false;
std::size_t finish_after_ticks = 0;

void log_time()
{
#ifdef FORCE_CYCLE_LOG
    std::cout << "MAIN: Ticks " << sim_ticks << std::endl;
#else
    if (sim_ticks%(DEFAULT_CLOCK_PERIOD*1000) == 0)
        std::cout << "MAIN: Ticks " << sim_ticks << std::endl;
#endif
}

void finish_after(std::size_t ticks)
{
    finish_timer_set = true;
    finish_after_ticks = sim_ticks + ticks;
    std::cout << "Finish set to  "<< finish_after_ticks << std::endl;    
}

bool check_done()
{
    return finish_timer_set && finish_after_ticks <= sim_ticks;
}

inline void timing_loop(auto & contextp, auto & topp)
{
    while (VL_LIKELY(!contextp->gotFinish())) {
        VerilatedVpi::doInertialPuts();        
        topp->eval();
        // Call VPI value change callbacks
        VerilatedVpi::callValueCbs();
        // Advance time
        if (!topp->eventsPending()) break;        
        contextp->time(topp->nextTimeSlot());
        log_time();
        sim_ticks+=DEFAULT_CLOCK_PERIOD;
        if (check_done())
        {
            std::cout << "MAIN: Done received" << std::endl;
            break;
        }
    }
}

#if 0
void no_timing_loop()
{
    int clk = 1;
    while (VL_LIKELY(!contextp->gotFinish())) {
        VerilatedVpi::doInertialPuts();        
        topp->tb_clk = clk;
        // Evaluate model
        topp->eval();
        // Call VPI value change callbacks
        VerilatedVpi::callValueCbs();
        // Advance time
        //if (!topp->eventsPending()) break;        
        //contextp->time(topp->nextTimeSlot());
        log_time();
        sim_ticks+=DEFAULT_CLOCK_PERIOD/2;
        if (check_done())
        {
            std::cout << "MAIN: Done received" << std::endl;
            break;
        }
        clk = !clk; 
    }
}
#endif

int main(int argc, char** argv, char**)
{
    // Setup context, defaults, and parse command line
    Verilated::debug(0);
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    contextp->traceEverOn(true);
    contextp->threads(1);
    contextp->commandArgs(argc, argv);

    // Construct the Verilated model, from Vtop.h generated from Verilating
    const std::unique_ptr<Vtop> topp{new Vtop{contextp.get(), ""}};

    // Construct tb
    const std::unique_ptr<et_tb> tb_main { new et_tb{} };
    //const std::unique_ptr<main_tb> tb_main { new main_tb{} };    
    log_time();
    
    // Simulate until $finish
    timing_loop(contextp, topp );
    
    if (VL_LIKELY(!contextp->gotFinish())) {
        VL_DEBUG_IF(VL_PRINTF("+ Exiting without $finish; no events left\n"););
    }

    // Execute 'final' processes
    topp->final();

    // Print statistical summary report
    contextp->statsPrintSummary();

    return 0;
}
