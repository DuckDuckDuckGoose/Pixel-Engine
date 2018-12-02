#include "PixGL.h"

PixGL void SetValues(float pw_, float ph_, float ww_, float wh_)
{
	pw = pw_;
	ph = ph_;
	ww = ww_;
	wh = wh_;
}

PixGL void CreateCoords(int pixW, int pixH, int scrW, int scrH)
{
	int winW = pixW * scrW;
	int winH = pixH * scrH;

	for (size_t i = 0; i < winW; i++)
	{
		for (size_t j = 0; j < winH; j++)
		{
			float x = i / (float)winW;
			x = x * 2 - 1;
			float y = j / (float)winH;
			y = y * 2 - 1;

			unitCoords.push_back(std::make_pair(x, y));
		}
	}

	points a;
	points b;

	for (size_t i = 0; i < scrW; i++)
	{
		for (size_t j = 0; j < scrH; j++)
		{
			float x = i / (float)scrW;
			x = x * 2 - 1;
			float y = j / (float)scrH;
			y = y * 2 - 1; 
			
			a.push_back(std::make_pair(x, y));

			float x2 = (i + 1) / (float)scrW;
			x2 = x2 * 2 - 1;
			float y2 = (j + 1) / (float)scrH;
			y2 = y2 * 2 - 1;

			b.push_back(std::make_pair(x2, y2));
		}
	}

	coords = std::make_pair(a, b);
}

PixGL void RenderUnitPixels(int width, int height, const Pixel* pixels)
{
	auto it = unitCoords.begin();

	glBegin(GL_POINTS);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			auto point = *it;
			advance(it, 1);

			float x = point.first;
			float y = point.second;

			Pixel p = pixels[j * width + i];

			glColor3ub(p.r, p.g, p.b);
			glVertex2f(x, -y);
		}
	}
	glEnd();
}

PixGL void RenderPixels(int width, int height, const Pixel* pixels)
{
	auto it = coords.first.begin();
	auto it2 = coords.second.begin();

	glBegin(GL_QUADS);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Pixel p = pixels[j * width + i];

			auto pair1 = *it;
			float x1 = pair1.first;
			float y1 = pair1.second;

			auto pair2 = *it2;
			float x2 = pair2.first;
			float y2 = pair2.second;

			std::advance(it, 1);
			std::advance(it2, 1);

			glColor3ub(p.r, p.g, p.b);

			glVertex2f(x1, -y1);
			glVertex2f(x1, -y2);
			glVertex2f(x2, -y2);
			glVertex2f(x2, -y1);
		}
	}
	glEnd();
}

PixGL void RenderText(int scrW, int scrH, int width, int height, const Pixel* pixels)
{
	auto it = unitCoords.begin();

	glBegin(GL_POINTS);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			auto point = *it;
			advance(it, 1);

			float x = point.first;
			float y = point.second;

			Pixel p = pixels[j * width + i];

			if (p.a == 0)
				continue;

			glColor3ub(p.r, p.g, p.b);
			glVertex2f(x, -y);
		}

		if (scrH > height)
			advance(it, scrH - height);
	}
	glEnd();
}

PixGL void DestroyCoords()
{
	coords.first.clear();
	coords.second.clear();
	unitCoords.clear();
}