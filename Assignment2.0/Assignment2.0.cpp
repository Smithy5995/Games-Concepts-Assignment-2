//Mark Smith - Games Concept Assignment 2

//Includes all the libraries necessary and any namspaces
#include <TL-Engine.h>
#include <math.h>
#include <sstream>
using namespace tle;

void main()
{
	//Creates th 3D engine and starts the program in windowed mode
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	//This is the default folder where all the assets are pulled from
	myEngine->AddMediaFolder("D:\\Uni Work\\Programming Software\\TL-Engine\\TL-Engine\\Media");

	//Creates all the needed meshes for the program
	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.X");
	IMesh* waterMesh = myEngine->LoadMesh("surface.X");
	IMesh* island1Mesh = myEngine->LoadMesh("island1.X");
	IMesh* frogMesh = myEngine->LoadMesh("frog.X");
	IMesh* transitMesh = myEngine->LoadMesh("transit.X");
	IMesh* roverMesh = myEngine->LoadMesh("rover.X");
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");

	//Creates all the needed models for the program
	//Assigns the meshes to models and positions them onscreen
	IModel* skybox = skyboxMesh->CreateModel(0, -1000, 0);
	IModel* water = waterMesh->CreateModel(0, -5, 0);
	IModel* island1 = island1Mesh->CreateModel(0, -5, 40);
	IModel* frog1 = frogMesh->CreateModel(-10, 0, 15);
	IModel* frog2 = frogMesh->CreateModel(0, 0, 15);
	IModel* frog3 = frogMesh->CreateModel(10, 0, 15);
	IModel* transit = transitMesh->CreateModel(50, 0, 35);
	IModel* transit2 = transitMesh->CreateModel(0, 0, 35);
	IModel* rover = roverMesh->CreateModel(-50, 0, 45);
	IModel* rover2 = roverMesh->CreateModel(0, 0, 45);
	IModel* DummyFrog1 = dummyMesh->CreateModel();
	IModel* DummyFrog2 = dummyMesh->CreateModel();
	IModel* DummyFrog3 = dummyMesh->CreateModel();

	//Sets the font and size for later use
	IFont* myFont = myEngine->LoadFont("Times New Roman", 100);

	//Creates the camera at the specified position and rotates it
	//This is so it hangs in the air, looking down on the island
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 40, -60);
	camera->RotateX(20);

	//Keys assigned to a name, making it easier to read
	//For example, the EKeyCode 192 isn't defined in the TL-Engine
	//Now I know it has to do with going 'Forward'
	const EKeyCode EscapeKey(Key_Escape);
	const EKeyCode MouseToggle(Key_Tab);
	const EKeyCode Forward(EKeyCode(192));
	const EKeyCode Back(EKeyCode(191));
	const EKeyCode Left(Key_Z);
	const EKeyCode Right(Key_X);
	const EKeyCode Pause(Key_P);

	//Rotates the vehicles as there inital value is facing North
	transit->RotateY(-90);
	transit2->RotateY(-90);
	rover->RotateY(90);
	rover2->RotateY(90);

	//Sets the boundaries for the island which stops the frogs and vehicles
	//from moving off the island
	int leftLimit = -50;
	int rightLimit = 50;
	int middleLimit = 0;
	int topLimit = 65;
	int bottomLimit = 15;

	int arraynum0 = 0;
	int arraynum1 = 1;
	int arraynum2 = 2;
	int arraynum3 = 3;

	int frogYscale = 0;

	int textX = 600;
	int textY = 300;

	int boundaryX = 10;
	float boundaryZ = 7.5;

	//Rotations to stop the use of magic numbers
	int rotationY90 = 90;
	int rotationY180 = 180;

	//Two string arrays for the frogs and vehicles, the frog array
	//has four states so show exactly what it's doing
	//The vehicle array show whether it's driving or stopped
	string frogstates[4]{ "Waiting", "Crossing", "Alive", "Dead" };
	string carstates[2]{ "Driving", "Stopped" };
	//The boundry states for vehicles have four states which are declared
	//in the main game loop so they are continously updated
	double roverboundaries[4];
	double rover2boundaries[4];
	double transitboundaries[4];
	double transit2boundaries[4];

	string frog1state;
	string frog2state;
	string frog3state;

	string transitstate;
	string transit2state;
	string roverstate;
	string rover2state;

	//Initialises the first frog to the Crossing state and the rest
	//to the Waiting state
	frog1state = frogstates[1];
	frog2state = frogstates[0];
	frog3state = frogstates[0];

	//Initialises the vehicles to the Driving state
	transitstate = carstates[0];
	transit2state = carstates[0];
	roverstate = carstates[0];
	rover2state = carstates[0];

	//Cycles through the three frog skins as the game is loading
	//This pre-loads all the skins from the asset folder so there is no pause
	//When there is a collision or when a frog reaches the end
	frog1->SetSkin("frog_blue.bmp");
	frog1->SetSkin("frog_red.bmp");
	frog1->SetSkin("frog.bmp");

	//The constant speed at which the game runs
	//The value will vary from PC to PC so it will need changing
	const float kGameSpeed = 0.0025;
	const float kfrogSpeed = 0.0025;
	const float ktransitSpeed = 0.0025;
	const float kroverSpeed = 0.0025;

	float frameTime = myEngine->Timer();
	float timer = 20.0;

	//Initialised the pause state to false when the game launches
	bool paused = false;

	  //-------------------------------------------------------------------------------------\\
	 //    GAME LOOP --- GAME LOOP --- GAME LOOP --- GAME LOOP --- GAME LOOP --- GAME LOOP    \\
	//-----------------------------------------------------------------------------------------\\

	while (myEngine->IsRunning())
	{
		if (myEngine->KeyHit(Pause))
		{
			paused = !paused;
		}
		if (paused == false)
		{
			//Draw the scene
			myEngine->DrawScene();

			//When the vehicles are in the driving state they move along the X axis
			if (transitstate == carstates[arraynum0])
			{
				transit->MoveX(-kGameSpeed);
				transit2->MoveX(-kGameSpeed);
			}
			if (roverstate == carstates[arraynum0])
			{
				rover->MoveX(kGameSpeed + kroverSpeed);
				rover2->MoveX(kGameSpeed + kroverSpeed);
			}

			if (frog1state == frogstates[arraynum1])
			{
				if (myEngine->KeyHeld(Right))
				{
					if (frog1->GetX() <= rightLimit)
					{
						frog1->ResetOrientation();
						frog1->RotateY(rotationY90);
						frog1->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Left))
				{
					if (frog1->GetX() >= leftLimit)
					{
						frog1->ResetOrientation();
						frog1->RotateY(-rotationY90);
						frog1->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Forward))
				{
					if (frog1->GetZ() <= topLimit)
					{
						frog1->ResetOrientation();
						frog1->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Back))
				{
					if (frog1->GetZ() >= bottomLimit)
					{
						frog1->ResetOrientation();
						frog1->RotateY(rotationY180);
						frog1->MoveLocalZ(kfrogSpeed);
					}
				}

				if (frog1->GetZ() >= topLimit)
				{
					frog1state = frogstates[arraynum2];
					frog2state = frogstates[arraynum1];
					frog1->SetSkin("frog_blue.bmp");
				}
				else if (frog1->GetX() > transitboundaries[arraynum0] && frog1->GetX() < transitboundaries[arraynum1] &&
					frog1->GetZ() > transitboundaries[arraynum2] && frog1->GetZ() < transitboundaries[arraynum3] ||
					frog1->GetX() > transit2boundaries[arraynum0] && frog1->GetX() < transit2boundaries[arraynum1] &&
					frog1->GetZ() > transit2boundaries[arraynum2] && frog1->GetZ() < transit2boundaries[arraynum3] ||
					frog1->GetX() > roverboundaries[arraynum0] && frog1->GetX() < roverboundaries[arraynum1] &&
					frog1->GetZ() > roverboundaries[arraynum2] && frog1->GetZ() < roverboundaries[arraynum3] ||
					frog1->GetX() > rover2boundaries[arraynum0] && frog1->GetX() < rover2boundaries[arraynum1] &&
					frog1->GetZ() > rover2boundaries[arraynum2] && frog1->GetZ() < rover2boundaries[arraynum3])
				{
					frog1state = frogstates[arraynum3];
					frog2state = frogstates[arraynum1];
					frog1->SetSkin("frog_red.bmp");
					frog1->ScaleY(frogYscale);
				}
			}

			if (frog2state == frogstates[arraynum1])
			{
				if (myEngine->KeyHeld(Right))
				{
					if (frog2->GetX() <= rightLimit)
					{
						frog2->ResetOrientation();
						frog2->RotateY(rotationY90);
						frog2->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Left))
				{
					if (frog2->GetX() >= leftLimit)
					{
						frog2->ResetOrientation();
						frog2->RotateY(-rotationY90);
						frog2->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Forward))
				{
					if (frog2->GetZ() <= topLimit)
					{
						frog2->ResetOrientation();
						frog2->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Back))
				{
					if (frog2->GetZ() >= bottomLimit)
					{
						frog2->ResetOrientation();
						frog2->RotateY(rotationY180);
						frog2->MoveLocalZ(kfrogSpeed);
					}
				}

				if (frog2->GetZ() >= topLimit)
				{
					frog2state = frogstates[arraynum2];
					frog3state = frogstates[arraynum1];
					frog2->SetSkin("frog_blue.bmp");
				}
				else if (frog2->GetX() > transitboundaries[arraynum0] && frog2->GetX() < transitboundaries[arraynum1] &&
					frog2->GetZ() > transitboundaries[arraynum2] && frog2->GetZ() < transitboundaries[arraynum3] ||
					frog2->GetX() > transit2boundaries[arraynum0] && frog2->GetX() < transit2boundaries[arraynum1] &&
					frog2->GetZ() > transit2boundaries[arraynum2] && frog2->GetZ() < transit2boundaries[arraynum3] ||
					frog2->GetX() > roverboundaries[arraynum0] && frog2->GetX() < roverboundaries[arraynum1] &&
					frog2->GetZ() > roverboundaries[arraynum2] && frog2->GetZ() < roverboundaries[arraynum3] ||
					frog2->GetX() > rover2boundaries[arraynum0] && frog2->GetX() < rover2boundaries[arraynum1] &&
					frog2->GetZ() > rover2boundaries[arraynum2] && frog2->GetZ() < rover2boundaries[arraynum3])
				{
					frog2state = frogstates[arraynum3];
					frog3state = frogstates[arraynum1];
					frog2->SetSkin("frog_red.bmp");
					frog2->ScaleY(frogYscale);
				}
			}

			if (frog3state == frogstates[arraynum1])
			{
				if (myEngine->KeyHeld(Right))
				{
					if (frog3->GetX() <= rightLimit)
					{
						frog3->ResetOrientation();
						frog3->RotateY(rotationY90);
						frog3->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Left))
				{
					if (frog3->GetX() >= leftLimit)
					{
						frog3->ResetOrientation();
						frog3->RotateY(-rotationY90);
						frog3->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Forward))
				{
					if (frog3->GetZ() <= topLimit)
					{
						frog3->ResetOrientation();
						frog3->MoveLocalZ(kfrogSpeed);
					}
				}
				else if (myEngine->KeyHeld(Back))
				{
					if (frog3->GetZ() >= bottomLimit)
					{
						frog3->ResetOrientation();
						frog3->RotateY(rotationY180);
						frog3->MoveLocalZ(kfrogSpeed);
					}
				}

				if (frog3->GetZ() >= topLimit)
				{
					frog3state = frogstates[arraynum2];
					frog3->SetSkin("frog_blue.bmp");
				}
				else if (frog3->GetX() > transitboundaries[arraynum0] && frog3->GetX() < transitboundaries[arraynum1] &&
					frog3->GetZ() > transitboundaries[arraynum2] && frog3->GetZ() < transitboundaries[arraynum3] ||
					frog3->GetX() > transit2boundaries[arraynum0] && frog3->GetX() < transit2boundaries[arraynum1] &&
					frog3->GetZ() > transit2boundaries[arraynum2] && frog3->GetZ() < transit2boundaries[arraynum3] ||
					frog3->GetX() > roverboundaries[arraynum0] && frog3->GetX() < roverboundaries[arraynum1] &&
					frog3->GetZ() > roverboundaries[arraynum2] && frog3->GetZ() < roverboundaries[arraynum3] ||
					frog3->GetX() > rover2boundaries[arraynum0] && frog3->GetX() < rover2boundaries[arraynum1] &&
					frog3->GetZ() > rover2boundaries[arraynum2] && frog3->GetZ() < rover2boundaries[arraynum3])
				{
					frog3state = frogstates[arraynum3];
					frog3->SetSkin("frog_red.bmp");
					frog3->ScaleY(frogYscale);
				}
			}

			//When all three frogs have changed to the third/fourth state which is safe/dead state
			//Text comes up saying you won/lost and also changed the vehicle state to stopped
			//This stops them from moving and gives the illusion the game has stopped
			if (frog1state == frogstates[arraynum2] && frog2state == frogstates[arraynum2] && frog3state == frogstates[arraynum2])
			{
				myFont->Draw("Congratulations, you win!!", textX, textY, kGreen, kCentre);
				transitstate = carstates[arraynum1];
				transit2state = carstates[arraynum1];
				roverstate = carstates[arraynum1];
				rover2state = carstates[arraynum1];
			}
			if (frog1state == frogstates[arraynum3] && frog2state == frogstates[arraynum3] && frog3state == frogstates[arraynum3])
			{
				myFont->Draw("Unfortunately, you fail!!", textX, textY, kRed, kCentre);
				transitstate = carstates[arraynum1];
				transit2state = carstates[arraynum1];
				roverstate = carstates[arraynum1];
				rover2state = carstates[arraynum1];
			}

			//When the vehicles reach their limit, which is the other side of the island
			//They both reset back to their original position
			if (rover->GetX() >= rightLimit)
			{
				rover->SetX(leftLimit);
			}
			if (rover2->GetX() >= rightLimit)
			{
				rover2->SetX(middleLimit);
			}
			if (transit->GetX() <= leftLimit)
			{
				transit->SetX(rightLimit);
			}
			if (transit2->GetX() <= leftLimit)
			{
				transit2->SetX(middleLimit);
			}

			//The amount of time is displayed since the last Timer() is called
			//Time is then subtracted after each frame
			frameTime = myEngine->Timer();
			timer -= frameTime;

			stringstream timerText;
			timerText << timer;
			myFont->Draw(timerText.str(), timer, timer, kWhite);

			//Attaches the frogs and camera to dummy models to avoid changing camera angles
			frog1->AttachToParent(DummyFrog1);
			frog2->AttachToParent(DummyFrog2);
			frog3->AttachToParent(DummyFrog3);
			camera->AttachToParent(DummyFrog1);

			//Sets the X and Z boundaries of the vehicles for collision
			transitboundaries[arraynum0] = transit->GetX() - boundaryX;
			transitboundaries[arraynum1] = transit->GetX() + boundaryX;
			transitboundaries[arraynum2] = transit->GetZ() - boundaryZ;
			transitboundaries[arraynum3] = transit->GetZ() + boundaryZ;

			transit2boundaries[arraynum0] = transit2->GetX() - boundaryX;
			transit2boundaries[arraynum1] = transit2->GetX() + boundaryX;
			transit2boundaries[arraynum2] = transit2->GetZ() - boundaryZ;
			transit2boundaries[arraynum3] = transit2->GetZ() + boundaryZ;

			roverboundaries[arraynum0] = rover->GetX() - boundaryX;
			roverboundaries[arraynum1] = rover->GetX() + boundaryX;
			roverboundaries[arraynum2] = rover->GetZ() - boundaryZ;
			roverboundaries[arraynum3] = rover->GetZ() + boundaryZ;

			rover2boundaries[arraynum0] = rover2->GetX() - boundaryX;
			rover2boundaries[arraynum1] = rover2->GetX() + boundaryX;
			rover2boundaries[arraynum2] = rover2->GetZ() - boundaryZ;
			rover2boundaries[arraynum3] = rover2->GetZ() + boundaryZ;
		}
		if (myEngine->KeyHit(EscapeKey))
		{
			myEngine->Stop(); //Closes the application window
		}
	}

	//Purges the 3D engine from memory
	myEngine->Delete();
}
