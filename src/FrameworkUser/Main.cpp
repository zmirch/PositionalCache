#include "World.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <algorithm> // For std::min
#include <optional>

#include <raylib.h>
#include <variant>

#include "../Framework/Bounds.h"
#include "Button.h"

namespace FrameworkUser
{

float WIDTH = 1000, HEIGHT = 1000, CIRCLERADIUS = 1, BUTTONSIZE = 17;
bool randomMovements = true, showBoundariesToggle = false;

World world(Point2D(WIDTH, HEIGHT));

std::vector<EntityHandle<WorldEntity>> selectedEntities;
Point2D selectionPointA;
Point2D selectionPointB;
Rectangle selectionRectangle;

Button blueButton = Button(Vector2{ static_cast<float>(WIDTH) - 180, static_cast<float>(HEIGHT) - 60 }, BUTTONSIZE, BUTTONSIZE, BLUE);
Button greenButton = Button(Vector2{ static_cast<float>(WIDTH) - 150, static_cast<float>(HEIGHT) - 60 }, BUTTONSIZE, BUTTONSIZE, GREEN);
Button redButton = Button(Vector2{ static_cast<float>(WIDTH) - 120, static_cast<float>(HEIGHT) - 60 }, BUTTONSIZE, BUTTONSIZE, RED);
Button worldVectorButton = Button(Vector2{ static_cast<float>(WIDTH) - 240, static_cast<float>(30) }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button worldBasicButton = Button(Vector2{ static_cast<float>(WIDTH) - 180, static_cast<float>(30) }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button worldQTreeButton = Button(Vector2{ static_cast<float>(WIDTH) - 120, static_cast<float>(30) }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button worldGridButton = Button(Vector2{ static_cast<float>(WIDTH) - 60, static_cast<float>(30) }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button randomMovementToggleButton = Button(Vector2{ static_cast<float>(460), 10 }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button shuffleEntityPositionsButton = Button(Vector2{ static_cast<float>(560), 10 }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button clearButton = Button(Vector2{ 250, 10 }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
Button showBoundsButton = Button(Vector2{ static_cast<float>(710), 10 }, BUTTONSIZE, BUTTONSIZE, LIGHTGRAY);
const char* currentAlgorithmText = nullptr;

struct AddEntityButton {
	Button button;
	const char* label;
	int count;
};

std::vector<AddEntityButton> addEntityButtons = {
	{ Button(BUTTONSIZE, BUTTONSIZE), "100", 100 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "1k", 1000 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "10k", 10000 }
};

std::vector<AddEntityButton> addEntityLineButtons = {
	{ Button(BUTTONSIZE, BUTTONSIZE), "100", 100 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "1k", 1000 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "10k", 10000 }
};

std::vector<AddEntityButton> addEntityClusterButtons = {
	{ Button(BUTTONSIZE, BUTTONSIZE), "100", 100 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "1k", 1000 },
	{ Button(BUTTONSIZE, BUTTONSIZE), "10k", 10000 }
};

void ClearEntities()
{
	selectedEntities.clear();
	world.clear();
}

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
	world.selectArea(boundingBox, [&](EntityView<WorldEntity>& safeView)
	{
		selectedEntities.push_back(safeView.getHandle());
	});
}

void Update()
{
	if (world.isTesting)
		return;

	if (IsKeyPressed(KEY_C))
	{
		ClearEntities();
		selectedEntities.clear();
	}

	if (IsMouseButtonPressed(0))
	{
		Vector2 mousePosition = GetMousePosition();

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
		else if (worldVectorButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Deque)
			{
				ClearEntities();
				world.setCacheType(CacheType::Deque);
				currentAlgorithmText = "Deque";
				if (randomMovements)
					world.startRandomMovements();
			}
		}
		else if (worldBasicButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Basic)
			{
				ClearEntities();
				world.setCacheType(CacheType::Basic);
				currentAlgorithmText = "Map";
				if (randomMovements)
					world.startRandomMovements();
			}
		}
		else if (worldQTreeButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Quadtree)
			{
				ClearEntities();
				world.setCacheType(CacheType::Quadtree);
				currentAlgorithmText = "Quadtree";
				if (randomMovements)
					world.startRandomMovements();
			}
		}
		else if (worldGridButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Grid)
			{
				ClearEntities();
				world.setCacheType(CacheType::Grid);
				currentAlgorithmText = "Grid";
				if (randomMovements)
					world.startRandomMovements();
			}
		}
		else if (randomMovementToggleButton.isPressed(mousePosition, true))
		{
			randomMovements ? world.stopRandomMovements() : world.startRandomMovements();
			randomMovements = !randomMovements;
		}
		else if (shuffleEntityPositionsButton.isPressed(mousePosition, true))
		{
			world.shuffleEntityPositions();
		}
		else if (clearButton.isPressed(mousePosition, true))
		{
			world.clear();
		}
		else if (showBoundsButton.isPressed(mousePosition, true))
		{
			showBoundariesToggle = !showBoundariesToggle;
		}
		else
		{
			bool handled = false;
			for (auto& b : addEntityButtons) {
				if (b.button.isPressed(mousePosition, true)) {
					world.addNEntities(b.count);
					handled = true;
					break;
				}
			}
			if (!handled)
				for (auto& b : addEntityLineButtons) {
					if (b.button.isPressed(mousePosition, true)) {
						world.addNEntitiesAlongLine(b.count);
						handled = true;
						break;
					}
				}

			if (!handled)
				for (auto& b : addEntityClusterButtons)
				{
					if (b.button.isPressed(mousePosition, true))
					{
						world.addNEntitiesCluster(b.count);
						handled = true;
						break;
					}
				}

			if (!handled) {
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
}

void DrawEntity(const EntityView<WorldEntity>& safeView)
{
	Point2D entityPosition = safeView.getEntity().getPosition();
	EntityColor entityColor = safeView.getEntity().getEntity().getColor(); // TODO second getEntity rename
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

	ClearBackground(BLACK);

	if (showBoundariesToggle)
	{
		world.forEachNodeBounds([](const Bounds& b)
		{
			float x = b.getPointA().getX();
			float y = b.getPointA().getY();
			float boundWidth = b.getPointB().getX() - b.getPointA().getX();
			float boundHeight = b.getPointB().getY() - b.getPointA().getY();

			Rectangle newRect;
			newRect.x = x;
			newRect.y = y;
			newRect.width = boundWidth;
			newRect.height = boundHeight;
			DrawRectangleLinesEx(newRect, 0.5, LIGHTGRAY);
		});
	}

	for (auto& entity : selectedEntities)
	{
		if(entity->hasEntity())
		{
			DrawCircle(entity->getPosition().getX(), entity->getPosition().getY(), CIRCLERADIUS + 2, WHITE);
		}
	}

	world.getAllEntities([&](EntityView<WorldEntity>& safeView)
	{
		DrawEntity(safeView);
	});

	DrawRectangleLinesEx(selectionRectangle, 1, LIGHTGRAY);

	DrawRectangle(WIDTH - 200, HEIGHT - 100, 180, 70, GRAY);
	DrawText("Color", WIDTH - 190, HEIGHT - 100, 30, BLACK);
	DrawText("Add Entities", 20, 10, 20, WHITE);
	DrawText("Deque", static_cast<float>(WIDTH) - 240, 53, 17, WHITE);
	DrawText("Map", static_cast<float>(WIDTH) - 180, 53, 17, WHITE);
	DrawText("QTree", static_cast<float>(WIDTH) - 130, 53, 17, WHITE);
	DrawText("Grid", static_cast<float>(WIDTH) - 60, 53, 17, WHITE);
	DrawText("Cache Algorithms", static_cast<float>(WIDTH) - 200, 5, 17, WHITE);
	DrawText(TextFormat("Current: %s", currentAlgorithmText), WIDTH - 200, 70, 17, YELLOW);
	DrawText("Random Movements:", 300, 10, 17, WHITE);
	DrawText("Shuffle:", 500, 10, 15, WHITE);
	DrawText("Clear:", 200, 10, 17, WHITE);
	DrawText("Show Bounds:", 600, 10, 17, WHITE);

	DrawText("Random Spread", 20, addEntityButtons[0].button.getPosition().y - 10, 10, WHITE);
	for (auto& b : addEntityButtons) {
		b.button.Draw();
		DrawText(b.label, b.button.getPosition().x, b.button.getPosition().y + 23, 14, WHITE);
	}

	DrawText("Line", 20, addEntityLineButtons[0].button.getPosition().y - 10, 10, WHITE);
	for (auto& b : addEntityLineButtons) {
		b.button.Draw();
		DrawText(b.label, b.button.getPosition().x, b.button.getPosition().y + 23, 14, WHITE);
	}

	DrawText("Cluster", 20, addEntityClusterButtons[0].button.getPosition().y - 10, 10, WHITE);
	for (auto& b : addEntityClusterButtons) {
		b.button.Draw();
		DrawText(b.label, b.button.getPosition().x, b.button.getPosition().y + 23, 14, WHITE);
	}

	blueButton.Draw();
	greenButton.Draw();
	redButton.Draw();
	worldVectorButton.Draw();
	worldBasicButton.Draw();
	worldQTreeButton.Draw();
	worldGridButton.Draw();
	randomMovementToggleButton.Draw();
	shuffleEntityPositionsButton.Draw();
	clearButton.Draw();
	showBoundsButton.Draw();
}
}

using namespace FrameworkUser;

int main()
{
	InitWindow(WIDTH, HEIGHT, "Positional Cache Demo");
	SetTargetFPS(60);

	std::srand(static_cast<unsigned>(std::time(0)));
	switch (world.getCurrentCacheType())
	{
		case FrameworkUser::CacheType::Deque:
			currentAlgorithmText = "Deque";
			break;
		case FrameworkUser::CacheType::Basic:
			currentAlgorithmText = "Map";
			break;
		case FrameworkUser::CacheType::Quadtree:
			currentAlgorithmText = "Qtree";
			break;
		case FrameworkUser::CacheType::Grid:
			currentAlgorithmText = "Grid";
			break;
		default:
			currentAlgorithmText = "Unknown";
			break;
	}

	world.startRandomMovements();
	for (size_t i = 0; i < addEntityButtons.size(); ++i) {
		addEntityButtons[i].button.setPosition(Vector2{ 20 + static_cast<float>(i) * 30, 50 });
	}

	for (size_t i = 0; i < addEntityLineButtons.size(); ++i) {
		addEntityLineButtons[i].button.setPosition(Vector2{ 20 + static_cast<float>(i) * 30, 120 });
	}

	for (size_t i = 0; i < addEntityClusterButtons.size(); ++i) {
		addEntityClusterButtons[i].button.setPosition(Vector2{ 20 + static_cast<float>(i) * 30, 190 });
	}

	int ct = 0;
	while (!WindowShouldClose())
	{
		Update();

		BeginDrawing();
		if (ct == 5)
		{
			Draw();
			ct = 0;
		}
		EndDrawing();
		ct++;
	}

	CloseWindow();

	return 0;
}