#include "Area2D.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <algorithm> // For std::min
#include <benchmark/benchmark.h>

#include <raylib.h>

#include "Timer.h"
#include "Bounds.h"
#include "Button.h"

int WIDTH = 1280, HEIGHT = 800, CIRCLERADIUS = 4;

Area2D area(Point2D(WIDTH, HEIGHT));

// Pointers to currently selected entities
std::vector<EntityHandle<EngineEntity>> currentSelection;

Point2D selectionPointA;
Point2D selectionPointB;
Rectangle selectionRectangle;

Button blueButton = Button(Vector2{ static_cast<float>(WIDTH) - 180, static_cast<float>(HEIGHT) - 60 }, 20, 20, BLUE);
Button greenButton = Button(Vector2{ static_cast<float>(WIDTH) - 150, static_cast<float>(HEIGHT) - 60 }, 20, 20, GREEN);
Button redButton = Button(Vector2{ static_cast<float>(WIDTH) - 120, static_cast<float>(HEIGHT) - 60 }, 20, 20, RED);
Button add10EntitiesButton = Button(Vector2{ static_cast<float>(40), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add100EntitiesButton = Button(Vector2{ static_cast<float>(80), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add1000EntitiesButton = Button(Vector2{ static_cast<float>(120), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add10KEntitiesButton = Button(Vector2{ static_cast<float>(160), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
//Button testAButton = Button(Vector2{ 50, static_cast<float>(HEIGHT) - 60 }, 20, 20, GRAY);
//Button testBButton = Button(Vector2{ 150, static_cast<float>(HEIGHT) - 60 }, 20, 20, GRAY);

const int SELECTION_TIMER{ 0 };
Timer timer;

void ColorSelection(EntityColor color)
{
	for (EntityHandle entityHandle : currentSelection)
	{
		EngineEntity* entity = entityHandle.getEntity();
		entity->setColor(color);
	}
}

void clearEntities()
{
	area.clear();  // Clear all entities from the Area2D object
	currentSelection.clear();  // Clear the current selection vector
}

void squareSelection(PositionalCache::Bounds boundingBox)
{ // Call the Area2D's square selection method and time it
	timer.startTimer(SELECTION_TIMER);
	currentSelection = area.squareSelection(boundingBox);
	timer.stopTimer(SELECTION_TIMER);
	timer.print();
}

void runTestA() // Testing selection of the same selection size and amount of repetitions, with different amounts of entities
{
	area.setTestingStatus(true);
	int selectionSize = 500;
	int numOfTestsPerEntityCount = 1000;
	std::vector<int> entityCounts = { 100, 10000, 100000, 400000 }; // Test for 100, 10k, 100k, 400k.

	printf("Running Test A (selection size: %d, tests per round: %d)\n", selectionSize, numOfTestsPerEntityCount);

	for (int entityCount : entityCounts)
	{
		clearEntities();
		area.addNEntities(entityCount);

		// Add a timer for this entity count if it doesn't already exist
		timer.addTimer(entityCount, "Selection with " + std::to_string(entityCount) + " entities");
		
		for (int i = 0; i < numOfTestsPerEntityCount; i++)
		{
			// Generate random top-left corner for the selection area
			int x = std::rand() % (WIDTH - selectionSize);
			int y = std::rand() % (HEIGHT - selectionSize);

			// Perform the selection and time it
			timer.startTimer(entityCount);
			currentSelection = area.squareSelection(PositionalCache::Bounds(Point2D(x, y), Point2D(x + selectionSize, y + selectionSize)));
            timer.stopTimer(entityCount);
		}
		std::cout << entityCount << " entities: " << timer.getAverageDuration(entityCount) << " seconds." << std::endl;
		
		timer.removeTimer(entityCount); 
	}
	clearEntities();
	printf("Done running Test A\n");
	area.setTestingStatus(false);
}


void runTestB() // Testing selection of the same amount of entities, and same amount of repetitions, with different selection sizes
{
	area.setTestingStatus(true);
	int entityCount = 100000;
	int numOfTestsPerSelectionSize = 1000;
	std::vector<int> selectionSizes = { 10, 100, 500, std::min(WIDTH, HEIGHT) - 1 };

	printf("Running Test B (entity count: %d, tests per round: %d)\n", entityCount, numOfTestsPerSelectionSize);

	clearEntities();
	area.addNEntities(entityCount);

	for (int selectionSize : selectionSizes)
	{
		// Add a timer for this entity count if it doesn't already exist
		timer.addTimer(selectionSize, "Selection of size " + std::to_string(selectionSize) + "x" + std::to_string(selectionSize));

		for (int i = 0; i < numOfTestsPerSelectionSize; i++)
		{
			// Generate random top-left corner for the selection area
			int x = std::rand() % (WIDTH - selectionSize);
			int y = std::rand() % (HEIGHT - selectionSize);

			// Perform the selection and time it
			timer.startTimer(selectionSize);
			currentSelection = area.squareSelection(PositionalCache::Bounds(Point2D(x, y), Point2D(x + selectionSize, y + selectionSize)));
			timer.stopTimer(selectionSize);
		}
		std::cout << selectionSize << "x" << selectionSize << " selection size: " << timer.getAverageDuration(selectionSize) << " seconds." << std::endl;
	
		timer.removeTimer(selectionSize);
	}
	clearEntities();
	printf("Done running Test B\n");
	area.setTestingStatus(false);
}

void Update()
{
	if (area.isTesting) {  // If benchmark is running, skip interactive actions
		return;
	}

	if (IsKeyPressed(KEY_C))
	{
		clearEntities();
	}

	if (IsMouseButtonPressed(0))
	{
		Vector2 mousePosition = GetMousePosition();
		
		// Update selection start point
		selectionPointA.setX(mousePosition.x);
		selectionPointA.setY(mousePosition.y);

		selectionRectangle.x = mousePosition.x;
		selectionRectangle.y = mousePosition.y;
		selectionRectangle.width = 0;
		selectionRectangle.height = 0;
	}

	if (IsMouseButtonReleased(0))
	{
		Vector2 mousePosition = GetMousePosition();

		// Check if the mouse clicked the button
		if (blueButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Blue);
		}
		else if (greenButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Green);
		}
		else if (redButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Red);
		}
		else if (add10EntitiesButton.isPressed(mousePosition, true))
		{
			area.addNEntities(10);
		}
		else if (add100EntitiesButton.isPressed(mousePosition, true))
		{
			area.addNEntities(100);
		}
		else if (add1000EntitiesButton.isPressed(mousePosition, true))
		{
			area.addNEntities(1000);
		}
		else if (add10KEntitiesButton.isPressed(mousePosition, true))
		{
			area.addNEntities(10000);
		}
		/*else if (testAButton.isPressed(mousePosition, true))
		{
			runTestA();
		}
		else if (testBButton.isPressed(mousePosition, true))
		{
			runTestB();
		}*/
		else
		{
			// Update selection end point
			selectionPointB.setX(mousePosition.x);
			selectionPointB.setY(mousePosition.y);

			// Determine top-left corner of drawn Rectangle
			selectionRectangle.x = std::min(selectionPointA.getX(), selectionPointB.getX());
			selectionRectangle.y = std::min(selectionPointA.getY(), selectionPointB.getY());

			// Determine width and height of drawn Rectangle
			selectionRectangle.width = abs(selectionPointA.getX() - selectionPointB.getX());
			selectionRectangle.height = abs(selectionPointA.getY() - selectionPointB.getY());

			squareSelection(PositionalCache::Bounds(selectionPointA, selectionPointB));

		}
	}
}

void DrawEntity(EngineEntity entity)
{
	Point2D entityPosition = entity.getPosition();
	EntityColor entityColor = entity.getColor();
	Color drawColor = WHITE;
	switch (entityColor) {
	case EntityColor::Red:
		drawColor = RED;
		break;
	case EntityColor::Blue:
		drawColor = BLUE;
		break;
	case EntityColor::Green:
		drawColor = GREEN;
		break;
	}
	DrawCircle(entityPosition.getX(), entityPosition.getY(), CIRCLERADIUS, drawColor);

}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	for (EntityHandle entityHandle : currentSelection)
	{
		EngineEntity* entity = entityHandle.getEntity();
		DrawCircle(entity->getPosition().getX(), entity->getPosition().getY(), CIRCLERADIUS + 2, WHITE);
	}

	std::deque<EngineEntity> entities = area.getEntities();
	for (const EngineEntity& entity : entities) {
		DrawEntity(entity);
	}

	DrawRectangleLinesEx(selectionRectangle, 1, LIGHTGRAY);

	DrawRectangle(WIDTH - 200, HEIGHT - 100, 180, 70, GRAY);
	DrawText("Color", WIDTH - 190, HEIGHT - 100, 30, BLACK);
	DrawText("Press [C] to clear", 170, 30, 20, DARKGRAY);
	DrawText("Add Entities", 30, 30, 20, WHITE);
	DrawText("10", 40, 93, 17, WHITE);
	DrawText("100", 80, 93, 17, WHITE);
	DrawText("1000", 120, 93, 17, WHITE);
	DrawText("10k", 160, 93, 17, WHITE);
	//DrawText("Test A", 30, static_cast<float>(HEIGHT) - 35, 17, WHITE);
	//DrawText("Test B", 130, static_cast<float>(HEIGHT) - 35, 17, WHITE);
	blueButton.Draw();
	greenButton.Draw();
	redButton.Draw();
	add10EntitiesButton.Draw();
	add100EntitiesButton.Draw();
	add1000EntitiesButton.Draw();
	add10KEntitiesButton.Draw();
	//testAButton.Draw();
	//testBButton.Draw();

	EndDrawing();
}

int main()
{
	InitWindow(WIDTH, HEIGHT, "Prototype 3");
	SetTargetFPS(60);

	std::srand(static_cast<unsigned>(std::time(0))); 

	area.startRandomMovements();

	timer.addTimer(SELECTION_TIMER, "Square Selection");

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	CloseWindow();

	return 0;
}