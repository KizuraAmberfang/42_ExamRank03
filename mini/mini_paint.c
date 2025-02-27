#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct	s_bg
{
	int		w;
	int		h;
	char	bgc;
}				t_bg;

typedef struct	s_draw
{
	char	type;
	float	x;
	float	y;
	float	rad;
	char	cc;
}				t_draw;

/*	[ UTILS ]	*/
//putstring
void	putstr(char *str)
{
	for (int i = 0; str[i]; i++)
		write(1, &str[i], 1);
}
//printMatrix
void	printMatrix(char **mtx)
{
	for (int i = 0; mtx[i]; i++)
	{
		putstr(mtx[i]);
		write(1, "\n", 1);
	}
}
//errore
int	errore(char *str)
{
	write(1, "Error: ", 7);
	putstr(str);
	write(1, "\n", 1);
	return (1);
}
//freeMatrix
int	freeMatrix(char **mtx, int ret)
{
	for (int i = 0; mtx[i]; i++)
		free(mtx[i]);
	free(mtx);
	return (ret);
}

/*	[ PROGRAM FUNCIONS ]	*/
//getBG
int	getBG(FILE *stream, t_bg *bg)
{
	int	ret = 0;
	ret = fscanf(stream, "%d %d %c", &bg->w, &bg->h, &bg->bgc);
	if (bg->w < 1 || bg->w > 300 || bg->h < 1 || bg->h > 300 || bg->bgc == 0 || bg->bgc == '\n')
		return (1);
	return (0);
}
//setBG
int	setBG(char **canv, t_bg bg)
{
	if (!canv)
		return (1);
	for (int i = 0; i <= bg.h; i++)
	{
		if (!(canv[i] = calloc(1, (bg.w + 1))))
			return (1);
		memset(canv[i], bg.bgc, bg.w);
		if (i == bg.h)
			canv[i] = 0;
	}
	return (0);
}
//isCircle
int	isCircle(int x, int y, t_draw draw)
{
	float	d = 0;

	d = sqrtf(((x - draw.x) * (x - draw.x) + (y - draw.y) * (y - draw.y)));
	if (d <= draw.rad)
	{
		if ((draw.rad - d) < 1.0 && (draw.rad - d) > 0.0)
			return (1);
		return (0);
	}
	return (-1);
}
//setCircle
void	setCircle(char **canv, t_draw draw, t_bg bg)
{
	int	io = 0;

	for (int y = 0; y < bg.h; y++)
	{
		for (int x = 0; x < bg.w; x++)
		{
			io = isCircle(x, y, draw);
			if (io == 1 || (io == 0 && draw.type == 'C'))
				canv[y][x] = draw.cc;
		}
	}
}
//draw
int	draw(FILE *stream, t_bg bg)
{
	char	**canv;
	t_draw	draw;
	int		ret = 0;

	canv = malloc(sizeof(char*) * (bg.h + 1));
	if (setBG(canv, bg) == 1)
		return (freeMatrix(canv, 1));
	draw.type = 0;
	while ((ret = fscanf(stream, "\n%c %f %f %f %c", &draw.type, &draw.x, &draw.y, &draw.rad, &draw.cc)) == 5)
	{
		if ((draw.type != 'C' && draw.type != 'c') || draw.rad == 0.0 || draw.cc == 0 || draw.cc == '\n')
			return (freeMatrix(canv, 1));
		setCircle(canv, draw, bg);
		draw.type = 0;
	}
	if (draw.type != 0 && ret != -1)
		return (freeMatrix(canv, 1));
	printMatrix(canv);
	return (freeMatrix(canv, 0));
}
//main
int	main(int ac, char **av)
{
	FILE	*stream;
	t_bg	bg;

	if (ac != 2)
		return (errore("argument"));
	if (!(stream = fopen(av[1], "r")))
		return (errore("Operation file corrupted"));
	if (getBG(stream, &bg) == 1)
		return (errore("Operation file corrupted"));
	if (draw(stream, bg) == 1)
		return (errore("Operation file corrupted"));
	fclose(stream);
	return (0);
}