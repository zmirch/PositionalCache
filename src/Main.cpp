#include "Area2D.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <algorithm> // For std::min

#include <raylib.h>

#include "Bounds.h"
#include "Button.h"

namespace Engine
{

int WIDTH = 1280, HEIGHT = 800, CIRCLERADIUS = 4;

Area2D area(Point2D(WIDTH, HEIGHT));

std::vector<EntityHandle<EngineEntity>> selectedEntities;
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

const int SELECTION_TIMER{ 0 };

void ColorSelection(EntityColor color)
{
	for (auto& entity : selectedEntities)
	{
		if (entity->hasEntity())
		{
			entity->getEntity().setColor(color);
		}
	}
}

void squareSelection(PositionalCache::Bounds boundingBox) {
	selectedEntities.clear();
	area.selectArea(boundingBox, [&](EntityView<EngineEntity>& safeView) {
		selectedEntities.push_back(safeView.getHandle());
	});
}

void Update()
{
	if (area.isTesting) {  // If benchmark is running, skip interactive actions
		return;
	}

	if (IsKeyPressed(KEY_C))
	{
		area.clear();
		selectedEntities.clear();
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

void DrawEntity(const EngineEntity& entity)
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

	for (auto& entity : selectedEntities)
	{
		if(entity->hasEntity())
		{
			EngineEntity& selectedEntity = entity->getEntity();
			DrawCircle(selectedEntity.getPosition().getX(), selectedEntity.getPosition().getY(), CIRCLERADIUS + 2, WHITE);
		}
	}

	area.getAllEntities([&](EntityView<EngineEntity>& safeView) {
		DrawEntity(safeView.getEntity());
	});

	DrawRectangleLinesEx(selectionRectangle, 1, LIGHTGRAY);

	DrawRectangle(WIDTH - 200, HEIGHT - 100, 180, 70, GRAY);
	DrawText("Color", WIDTH - 190, HEIGHT - 100, 30, BLACK);
	DrawText("Press [C] to clear", 170, 30, 20, DARKGRAY);
	DrawText("Add Entities", 30, 30, 20, WHITE);
	DrawText("10", 40, 93, 17, WHITE);
	DrawText("100", 80, 93, 17, WHITE);
	DrawText("1000", 120, 93, 17, WHITE);
	DrawText("10k", 160, 93, 17, WHITE);
	blueButton.Draw();
	greenButton.Draw();
	redButton.Draw();
	add10EntitiesButton.Draw();
	add100EntitiesButton.Draw();
	add1000EntitiesButton.Draw();
	add10KEntitiesButton.Draw();

	EndDrawing();
}
}

using namespace Engine;

int main()
{
	InitWindow(WIDTH, HEIGHT, "Prototype 3");
	SetTargetFPS(60);

	std::srand(static_cast<unsigned>(std::time(0)));

	area.startRandomMovements();

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	CloseWindow();

	return 0;
}