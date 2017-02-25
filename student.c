/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * -
 */

#include "student.h"
#include "globals.h"

#include <time.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.5 bodu */
S_RGBA getPixel(int x, int y)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
		return frame_buffer[y*width + x];
	else
		return makeBlackColor();
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.5 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
		frame_buffer[y*width + x] = color;
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 1 bod. */
void grayScale()
{
	S_RGBA pix;
	int intensity;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			pix = getPixel(x, y);
			intensity = (int)0.299*pix.red + 0.587*pix.green + 0.114*pix.blue;
			intensity = ROUND(intensity);
			pix.red = pix.blue = pix.green = intensity;
			putPixel(x, y, pix);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution()
{
	double error;
	
	grayScale();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			// point
			S_RGBA pix = getPixel(x, y);
			S_RGBA oldpix = pix;

			if (pix.red > 127)
				pix = COLOR_WHITE;
			else
				pix = COLOR_BLACK;

			putPixel(x, y, pix);

			if (pix.red == 255)
				error = oldpix.red - 255;
			else
				error = oldpix.red;

			/*
			---------
			|*  | p1|
			|p3 | p2|
			---------
			*/


			error_distrib(error, x+1, y, 3.0/8);
			error_distrib(error, x + 1, y + 1, 1.0/4);
			error_distrib(error, x, y + 1, 3.0/8);
		
		}
	}
}

void error_distrib(double error, int x, int y, double coeficient) {

	S_RGBA pixel = getPixel(x , y);
	
	double tmp_color = ROUND(error*coeficient);

	if ((tmp_color + pixel.red) <= 255 && (tmp_color + pixel.red) >= 0) {
		pixel.red	+= tmp_color;
		pixel.green += tmp_color;
		pixel.blue	+= tmp_color;
		
	}
	else if ((tmp_color + pixel.red) > 255) {
		pixel.red = pixel.green = pixel.blue = 255;
	}
	else if ((tmp_color + pixel.red) < 0)
		pixel.red = pixel.green = pixel.blue = 0;
	
	putPixel(x, y, pixel);
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/