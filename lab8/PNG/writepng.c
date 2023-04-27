/*
 * Copyright 2002-2008 Guillaume Cottenceau, 2015 Aleksander Denisiuk
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>


#define OUT_FILE "initials.png"
#define WIDTH 400
#define HEIGHT 400
#define COLOR_TYPE PNG_COLOR_TYPE_RGB
#define BIT_DEPTH 8


void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void create_png_file()
{
	width = WIDTH;
	height = HEIGHT;
        bit_depth = BIT_DEPTH;
        color_type = COLOR_TYPE;

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(width*bit_depth*3);


}


void write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, width, height,
		     bit_depth, color_type, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
	for (y=0; y<height; y++)
		free(row_pointers[y]);
	free(row_pointers);

        fclose(fp);
}

void write_pixel(int x, int y, png_byte cr, png_byte cg, png_byte cb){
    png_byte* row = row_pointers[y];
    png_byte* ptr = &(row[x*3]);
    ptr[0] = cr;
    ptr[1] = cg;
    ptr[2] = cb;
}

void rasterize_line(int x1, int y1, int x2, int y2, png_byte cr, png_byte cg, png_byte cb){

    if(x2 > x1 && y2 >= y1 && y2 - y1 <= x2 - x1){
        int m = 2 * (y2 - y1);
        int b = 0;
        write_pixel(x1, y1, cr, cg, cb);
        int y = y1;
        int P = x2 - x1;
        for(int x = x1 + 1; x <= x2; x++){
            b += m;
            if(b > P){
                y++;
                b -= 2 * P;
            }
            write_pixel(x, y, cr, cg, cb);
        }
    } else if(x2 > x1 && -y2 >= -y1 && -y2 + y1 <= x2 - x1){
        int m = 2 * (-y2 + y1);
        int b = 0;
        write_pixel(x1, y1, cr, cg, cb);
        int y = y1;
        int P = x2 - x1;
        for(int x = x1 + 1; x <= x2; x++){
            b += m;
            if(b > P){
                y--;
                b -= 2 * P;
            }
            write_pixel(x, y, cr, cg, cb);
        }
    }
    else if(y2 > y1 && x2 >= x1 && x2 - x1 <= y2 - y1){
        int m = 2 * (x2 - x1);
        int b = 0;
        write_pixel(x1, y1, cr, cg, cb);
        int x = x1;
        int P = y2 - y1;
        for(int y = y1 + 1; y <= y2; y++){
            b += m;
            if(b > P){
                x++;
                b -= 2 * P;
            }
            write_pixel(x, y, cr, cg, cb);
        }
    } else if(-y2 > -y1 && x2 >= x1 && x2 - x1 <= -y2 + y1){
        int m = 2 * (x2 - x1);
        int b = 0;
        write_pixel(x1, y1, cr, cg, cb);
        int x = x1;
        int P = -y2 + y1;
        for(int y = y1; -y <= -y2; y--){
            b += m;
            if(b > P){
                x++;
                b -= 2 * P;
            }
            write_pixel(x, y, cr, cg, cb);
        }
    }
}

void rasterize_circle(int x, int y, int r, png_byte cr, png_byte cg, png_byte cb){
    int i = 0;
    int j = r;
    int f = 5 - 4 * r;
    write_pixel(i, j, cr, cg, cb);
    while(i < j){
        if(f > 0){
            f += 8 * i + 8 * j + 20;
            j--;
        } else{
            f += 8 * i + 12;
        }
        i++;
        write_pixel(i, j, cr, cg, cb);
    }
}

void process_file(void)
{
	for (y=0; y<height; y++) {
		png_byte* row = row_pointers[y];
		for (x=0; x<width; x++) {
			png_byte* ptr = &(row[x*3]);
			ptr[0] = ptr[1] = ptr[2] = 122;
		}
	}

    // G
    rasterize_line(170, 160, 180, 170, 0, 0, 0);
    rasterize_line(130, 160, 170, 160, 0, 0, 0);
    rasterize_line(120, 170, 130, 160, 0, 0, 0);
    rasterize_line(120, 230, 120, 170, 0, 0, 0);
    rasterize_line(120, 230, 120, 170, 0, 0, 0);
    rasterize_line(120, 230, 130, 240, 0, 0, 0);
    rasterize_line(130, 240, 170, 240, 0, 0, 0);
    rasterize_line(170, 240, 180, 230, 0, 0, 0);
    rasterize_line(180, 230, 180, 200, 0, 0, 0);
    rasterize_line(140, 200, 180, 200, 0, 0, 0);
    rasterize_line(140, 200, 140, 210, 0, 0, 0);
    rasterize_line(140, 210, 170, 210, 0, 0, 0);
    rasterize_line(170, 210, 170, 230, 0, 0, 0);
    rasterize_line(130, 230, 170, 230, 0, 0, 0);
    rasterize_line(130, 170, 130, 230, 0, 0, 0);
    rasterize_line(130, 170, 180, 170, 0, 0, 0);

    // S
    rasterize_line(270, 160, 280, 170, 0, 0, 0);
    rasterize_line(230, 160, 270, 160, 0, 0, 0);
    rasterize_line(220, 170, 230, 160, 0, 0, 0);
    rasterize_line(220, 170, 220, 195, 0, 0, 0);
    rasterize_line(220, 195, 230, 205, 0, 0, 0);
    rasterize_line(230, 205, 270, 205, 0, 0, 0);
    rasterize_line(270, 205, 270, 230, 0, 0, 0);
    rasterize_line(220, 230, 270, 230, 0, 0, 0);
    rasterize_line(220, 230, 230, 240, 0, 0, 0);
    rasterize_line(230, 240, 270, 240, 0, 0, 0);
    rasterize_line(270, 240, 280, 230, 0, 0, 0);
    rasterize_line(280, 230, 280, 205, 0, 0, 0);
    rasterize_line(270, 195, 280, 205, 0, 0, 0);
    rasterize_line(230, 195, 270, 195, 0, 0, 0);
    rasterize_line(230, 195, 230, 170, 0, 0, 0);
    rasterize_line(230, 170, 280, 170, 0, 0, 0);
}


int main(int argc, char **argv)
{
	create_png_file();
	process_file();
	write_png_file(OUT_FILE);

    return 0;
}
