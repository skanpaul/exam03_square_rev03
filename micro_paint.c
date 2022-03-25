#define NO_ERROR 0
#define ERROR 1
#define TXT_ERR_ARG "Error: argument\n"
#define TXT_ERR_FILE "Error: Operation file corrupted\n"
/* *********************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

/* *********************************************************************************** */
typedef struct s_zone
{
	int width;
	int height;
	char back;
	int total;
} t_zone;

typedef struct s_square
{
	char type;
	float orig_x;
	float orig_y;
	float width;
	float height;
	char drawn_char;
} t_square;

/* *********************************************************************************** */
int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;

	return i;
}

/* *********************************************************************************** */
void clean_all(char **map, FILE **ptr_file)
{
	if (*map)
		free(*map);
	*map = NULL;

	if (*ptr_file)
		if ((fclose(*ptr_file)) == 0)
			*ptr_file = NULL;
}

/* *********************************************************************************** */
char *get_zone_information(FILE *ptr_file, t_zone *z)
{
	char *map;
	int ret;

	map = NULL;

	ret = fscanf(ptr_file, "%d %d %c\n", &z->width, &z->height, &z->back);

	if (ret != 3)
		return NULL;

	if (z->height <= 0 || z->height > 300 || z->width <= 0 || z->width > 300)
		return NULL;

	z->total = z->width * z->height;

	if ((map = (char *)malloc(z->total * sizeof(char))) == NULL)
		return NULL;

	memset(map, z->back, z->total);

	return map;
}

/* *********************************************************************************** */
bool is_in_rectangle(int x_int, int y_int, t_square *s)
{
	float x;
	float y;
	float end_x;
	float end_y;

	x = (float)x_int;
	y = (float)y_int;

	end_x = s->orig_x + s->width;
	end_y = s->orig_y + s->height;

	if (s->orig_x <= x && x <= end_x && s->orig_y <= y && y <= end_y)
		return true;

	return false;
}
/* *********************************************************************************** */
bool is_on_edge(int x_int, int y_int, t_square *s)
{
	float x;
	float y;
	float end_x;
	float end_y;

	x = (float)x_int;
	y = (float)y_int;

	end_x = s->orig_x + s->width;
	end_y = s->orig_y + s->height;

	if (x - s->orig_x < 1 || end_x - x < 1)
		return true;

	if (y - s->orig_y < 1 || end_y - y < 1)
		return true;

	return false;
}

/* *********************************************************************************** */
void draw_square(char *map, t_zone *z, t_square *s)
{
	int x;
	int y;

	y = 0;
	while (y < z->height)
	{
		x = 0;
		while (x < z->width)
		{
			if (is_in_rectangle(x, y, s))
			{
				if ((s->type == 'r' && is_on_edge(x, y, s)) || (s->type == 'R'))
					map[x + y * z->width] = s->drawn_char;					
			}
			x++;
		}
		y++;
	}
}

/* *********************************************************************************** */
/* *********************************************************************************** */
char *do_map(char *map, t_zone *z, t_square *s, FILE *ptr_file)
{
	int ret;

	while (1)
	{
		ret = fscanf(ptr_file, "%c %f %f %f %f %c\n", &s->type, &s->orig_x, &s->orig_y, &s->width, &s->height, &s->drawn_char);

		if (ret == EOF)
			break;

		if (ret != 6)
			return NULL;

		if (s->width <= 0 || s->width <= 0)
			return NULL;

		if ((s->type != 'r') && (s->type != 'R'))
			return NULL;

		draw_square(map, z, s);
	}

	return map;
}

/* *********************************************************************************** */
void print_map(char *map, t_zone *z)
{
	int y;

	y = 0;
	while (y < z->height)
	{
		write(1, &map[y * z->width], z->width);
		write(1, "\n", 1);		
		y++;
	}
}

/* *********************************************************************************** */
int main(int argc, char **argv)
{
	FILE *ptr_file;
	char *map;
	t_zone z;
	t_square s;

	ptr_file = NULL;
	map = NULL;

	if (argc != 2)
	{
		write(1, TXT_ERR_ARG, ft_strlen(TXT_ERR_ARG));
		goto is_error;
	}

	if ((ptr_file = fopen(argv[1], "r")) == NULL)
	{
		write(1, TXT_ERR_FILE, ft_strlen(TXT_ERR_FILE));
		goto is_error;
	}

	if ((map = get_zone_information(ptr_file, &z)) == NULL)
		goto is_error;

	if ((map = do_map(map, &z, &s, ptr_file)) == NULL)
		goto is_error;

	print_map(map, &z);

	clean_all(&map, &ptr_file);
	clean_all(&map, &ptr_file);
	clean_all(&map, &ptr_file);
	return NO_ERROR;

is_error:
	clean_all(&map, &ptr_file);
	clean_all(&map, &ptr_file);
	clean_all(&map, &ptr_file);
	return ERROR;
}

/* *********************************************************************************** */