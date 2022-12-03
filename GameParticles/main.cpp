//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

// -----------------------------------------------------------
// ASSUME: MMX - SSE4.1 are allowed on the test machine
//         Do NOT use any setting above this level, grade->0
// -----------------------------------------------------------
#include "OpenGLDevice.h"
#include "Settings.h"
#include "ParticleEmitter.h"

int main()
{
	Trace::out("Num Particle: %.1e time:%.1f\n",(float)NUM_PARTICLES,MAX_LIFE);

	srand(1);

	// initialize timers:------------------------------

		// Create a timer objects
		PerformanceTimer updateTimer;
		PerformanceTimer drawTimer;

	// create an emitter:-------------------------------
		ParticleEmitter emitter;

	// Get the inverse Camera Matrix:-------------------

		// multiply them together
		Matrix tmp;

		// counter for printing
		int i = 0;
		// initialize the camera matrix
		// IDENT_MATRIX is the camera matrix;

		// setup the translation matrix
		Matrix transMatrix;
		Vect4D trans(0.0f, 3.0f, 3.0f);
		transMatrix.setTransMatrix(&trans);

		// multiply them together
		tmp = ParticleEmitter::IDENT_MATRIX * transMatrix;

		// get the inverse matrix
		Matrix inverseCameraMatrix;
		tmp.Inverse(inverseCameraMatrix);

		// ------------------------------------------------
		//  Set the Camera Matrix
		//  Note: 
		//       this method is using doubles... 
		//       there is a float version (hint)
		// ------------------------------------------------
		OpenGLDevice::SetCameraMatrixFloat((const float *)&inverseCameraMatrix);
	
	// main update loop... do this forever or until some breaks 
	while(OpenGLDevice::IsRunning())
	{
		// start update timer ---------------------------------------
		updateTimer.Tic();

			// update the emitter
			emitter.update();

		// stop update timer: -----------------------------------------
		updateTimer.Toc();

		// start draw timer: ----------------------------------------
		drawTimer.Tic();

			// draw particles
			emitter.draw();
		
		// stop draw timer: -----------------------------------------
		drawTimer.Toc();

		// LEAVE the loop below alone
		if( i++ > PRINT_COUNT ) 
		{
			i = 0;
			float updateTime = updateTimer.TimeInSeconds();
			float drawTime = drawTimer.TimeInSeconds();
			printf("LoopTime: update:%f ms  draw:%f ms  tot:%f\n",updateTime * 1000.0f, drawTime * 1000.0f, (updateTime + drawTime) *1000.0f);
		}
	}
	
    return 0;
}

// WinMain required for all win32 applications (Windows entry point)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	OpenGLDevice::Initialize(hInstance);
	main();
}

// --- End of File ---
