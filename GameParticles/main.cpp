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

PerformanceTimer T1;
PerformanceTimer T2;

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

	// initialize the camera matrix
	// IDENT_MATRIX is the camera matrix;

	// setup the translation matrix
	Matrix transMatrix(Matrix::IDENTITY_MATRIX);
	Vect4D trans(0.0f, 3.0f, 3.0f);
	transMatrix.setTransMatrix(&trans);

	// multiply them together
	Matrix tmp = transMatrix;

	// get the inverse matrix
	Matrix inverseCameraMatrix = Matrix::Inverse(tmp);

	OpenGLDevice::SetCameraMatrixFloat((const float *)&inverseCameraMatrix);
	
	// counter for printing
	int i = 0;
		
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
			//updateTimer.Reset();
			//drawTimer.Reset();

			//float T1_time = T1.TimeInSeconds();
			//float T2_time = T2.TimeInSeconds();
			//T1.Reset();
			//T2.Reset();

			//printf("LoopTime: T1:%f ms  T2:%f ms  drawTotal:%f\n", T1_time * 1000.0f, T2_time * 1000.0f, drawTime * 1000.0f);
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
