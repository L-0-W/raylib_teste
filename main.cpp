#include <iostream>
#include <print>
#include <raylib.h>
#include <vector>
#include <functional>
#include <fstream>
#include "rlgl.h"
#include <sstream>


void DrawGridColor(int slices, float spacing)
{
	int halfSlices = slices / 2;

	rlBegin(RL_LINES);
	for (int i = -halfSlices; i <= halfSlices; i++)
	{
		if (i == 0)
		{
			rlColor3f(1.0f, 1.0f, 0.5f);
		}
		else
		{
			rlColor4f(0.75f, 0.75f, 0.75f, 0.3f);
		}

		rlVertex3f((float)i * spacing, 0.0f, (float)-halfSlices * spacing);
		rlVertex3f((float)i * spacing, 0.0f, (float)halfSlices * spacing);

		rlVertex3f((float)-halfSlices * spacing, 0.0f, (float)i * spacing);
		rlVertex3f((float)halfSlices * spacing, 0.0f, (float)i * spacing);
	}
	rlEnd();
}

int main() {


	int screenWidth = 900;
	int screenHeight = 700;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "RayLib");
	SetWindowState(FLAG_WINDOW_TRANSPARENT);

	//ToggleFullscreen();

	SetTargetFPS(60);


	Image image = LoadImage("resources/cube.png");
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);

	Camera3D camera = { 0 };

	camera.position = { 2.0f, 5.0f, 25.0f };  
	camera.target = { 2.0f, 0.0f, 0.0f };      
	camera.up = { 0.0f, 1.0f, 0.0f };          
	camera.fovy = 45.0f;                                
	camera.projection = CAMERA_PERSPECTIVE;             

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	const int currentMonitor = GetCurrentMonitor();
	
	//screenWidth = GetMonitorWidth(currentMonitor);
	//screenHeight = GetMonitorHeight(currentMonitor);

	bool activateMenu = false;
	bool canDrawLineCursor = false;

	std::string search{};

	texture.width /= 16;
	texture.height /= 16;

	int downCount{-1};

	std::vector<Color> menuColors{RED, RED, RED, RED, RED, RED};

	Vector2 pos{ screenWidth / 2, screenHeight / 2 };

	std::vector<int> drawFunVector;
	std::vector<std::function<void()>> execLogicVec;

	while(!WindowShouldClose())
	{
		BeginDrawing();
			
			ClearBackground(Color{70, 53, 116, 255});

			for (auto const& draw : drawFunVector) {
				DrawCircle(pos.x, pos.y, 10.0f, BLUE);
			};

			for (auto const& exec : execLogicVec) {
				exec();
			};

			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T))
					activateMenu = !activateMenu;


			BeginMode3D(camera);
				DrawGridColor(90.0f, 1.0f);
			EndMode3D();

			Rectangle const rec{ 25, 15, 35, (screenHeight - 230) };
			DrawRectangleRounded(rec, 0.2f, 1, Color{16, 23, 23, 200});
			DrawTexture(texture, 32, 25, Color{ 98, 216, 245, 200 });


			if (activateMenu) {
				Rectangle const rec2{ screenWidth / 2 - 250, screenHeight / 2 - 245, 350, 245 };
				DrawRectangleRounded(rec2, 0.1, 1, Color{ 16, 23, 23, 170 });

				DrawText("3D", (screenWidth / 2 - 250) + 15, (screenHeight / 2 - 245) + 10, 5, menuColors[0]);
				DrawText("Cenario", (screenWidth / 2 - 250) + 25, (screenHeight / 2 - 245) + 25, 5, menuColors[1]);
				DrawText("Cubo", (screenWidth / 2 - 250) + 25, (screenHeight / 2 - 245) + 35, 5, menuColors[2]);


				DrawText("2D", (screenWidth / 2 - 250) + 15, (screenHeight / 2 - 245) + 55, 5, menuColors[3]);
				DrawText("Cenario", (screenWidth / 2 - 250) + 25, (screenHeight / 2 - 245) + 65, 5, menuColors[4]);
				DrawText("Circle", (screenWidth / 2 - 250) + 25, (screenHeight / 2 - 245) + 75, 5, menuColors[5]);

				if (IsKeyPressed(KEY_DOWN) && downCount < 5) {
					downCount++;

					for (int i = 0; i < menuColors.size(); i++)
					{
						if (i != downCount)
							menuColors[i] = RED;
					}

					menuColors[downCount] = GREEN;
					std::cout << downCount << "\n";
				};

				if (IsKeyPressed(KEY_UP) && downCount > 0) {
					downCount--;

					for (int i = 0; i < menuColors.size(); i++)
					{
						if (i != downCount)
							menuColors[i] = RED;
					}

					menuColors[downCount] = GREEN;
					std::cout << downCount << "\n";
				};

				if (IsKeyPressed(KEY_ENTER)) {
					if (downCount == 5) {
						drawFunVector.push_back(1);
						
						std::fstream fs;
						
						fs.open("circle.script", std::ios::out);
						
						if (!fs.is_open()) {
							fs.clear();
							fs.clear();
						}

						fs << "#include circleMesh" << "\n";
						fs.close();
						fs.clear();
					}
				}
			}

			if (IsKeyPressed(KEY_F5)) {
				std::ifstream fs{"circle.script", std::ios::binary};

				if (fs.is_open()) {
					std::string content((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

					std::cout << content << "\n";

					if (content.contains("circleMesh.position.x = 10.0f * deltaTime")) {
						std::cout << "executou" << "\n";
						execLogicVec.push_back([&pos]() { pos.x += 100.0f * GetFrameTime(); });
					}
				}

				fs.close();
				fs.clear();
			}
		

		EndDrawing();
	}


	CloseWindow();

	std::cout << "Ola Mundo!" << "\n";
	return 0;
}