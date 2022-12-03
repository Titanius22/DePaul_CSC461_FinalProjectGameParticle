//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef SETTINGS_H
#define SETTINGS_H

// Make sure you leave these setting to the default on final submission

#define CPU_WITH_GRAPHICS 0  // Set CPU_WITH_GRAPHICS: 1 to verify Graphics and CPU performance
                             //   Make sure graphics looks correct and shutdown doesn't leak
                             //        Memory Leak - 20% grade
                             //        Changed Graphics behavior - 50% grade 
                             
                             // Set CPU_WITH_GRAPHICS: 0 to verify CPU performance without graphics
                             //   Test ONLY CPU performance (used for final grading)

#if CPU_WITH_GRAPHICS
    #define NUM_PARTICLES	(10 * 1000 )  //Vary setting between 5K to 200K for development
    #define MAX_LIFE		(20.0f)       //leave at 20 for final testing
    #define PRINT_COUNT     20
#else
    #define NUM_PARTICLES	(200 * 1000 ) //leave at 200K for final testing
    #define MAX_LIFE		(20.0f)       //leave at 20 for final testing
    #define PRINT_COUNT     20
#endif

#endif 

// --- End of File ---
