#include <iostream>
#include <cmath>

int lon2tilex(double lon, int z)
{
    return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int lat2tiley(double lat, int z)
{
    double latrad = lat * M_PI / 180.0;
    return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z)));
}

/*--- tile col/row number to lon/lat ---*/
double tilex2lon(int x, int z)
{
    return x / (double)(1 << z) * 360.0 - 180.0;
}

double tiley2lat(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

#define EARTH_EQUATORIAL_RADIUS (6378.1370) // equatorial: 40,075 km
#define EARTH_POLAR_RADIUS      (6356.7523) // polar radius: 40,008 km
#define EARTH_MEAN_RADIUS       (6371.0088) // mean: 40,030 km

#define EARTH_RADIUS            (EARTH_MEAN_RADIUS)

/*--- ground resolution (m/pixel) ---*/
double ground_resolution_x(double lat, int z)
{
#if 0
    double circumference = cos(lat * M_PI / 180.0) * (2.0 * M_PI * EARTH_RADIUS * 1000.0);
    PR(">> circumference=%f, z=%d\n", circumference, z);
    PR(">> (1 << (z+8)=%x\n", (1 << (z + 8)));
    return (circumference / (1 << (z + 8)));
#else
    return ((cos(lat * M_PI / 180.0) * (2.0 * M_PI * EARTH_RADIUS * 1000.0)) / (1 << (z + 8)));
#endif
}

double ground_resolution_y(int z)
{
    return ((2.0 * M_PI * EARTH_RADIUS * 1000.0) / (1 << (z + 8)));
}

// ./tile <zoom> <lat> <lon>
int main(int argc, char** argv)
{
    if (argc != 4) return -1;

    int    zoom = strtol(argv[1], NULL, 10);
    double lat = strtod(argv[2], NULL);
    double lon = strtod(argv[3], NULL);

    int col = lon2tilex(lon, zoom);
    int row = lat2tiley(lat, zoom);

    double tile_lat = tiley2lat(row, zoom);
    double tile_lon = tilex2lon(col, zoom);

    double resol_x = ground_resolution_x(lat, zoom);
    double resol_y = ground_resolution_y(zoom);

    printf(" [z:%-2d, lat=%12.8f, lon=%12.8f] col=%d, row=%d \n",
            zoom, lat, lon, col, row);
    printf("        tile_lat=%12.8f, tile_lon=%12.8f, resol_x=%f, resol_y=%f\n",
            tile_lat, tile_lon, resol_x, resol_y);

    return 0;
}