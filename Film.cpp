#include "Film.h"

#include <limits>


namespace xrt {
    Film::Film(const FilmCoordinate x_resolution, const FilmCoordinate y_resolution,
               const double z, const double extent) :
        x_resolution(x_resolution),
        y_resolution(y_resolution),
        z(z),
        extent(extent),
        pixels(x_resolution * y_resolution, 0)
    {
    }

    void Film::set(const FilmCoordinate x, const FilmCoordinate y, Intensity i) {
        pixels.at(indexOf(x, y)) = i;
    }


    void Film::dumpPGM(std::ostream& sink) const {
        sink << "P2\n";
        sink << x_resolution << ' ' << y_resolution << '\n';
        sink << static_cast<int>(std::numeric_limits<Intensity>::max()) << '\n';

        for (FilmCoordinate x = 0; x < x_resolution; ++x) {
            for (FilmCoordinate y = 0; y < y_resolution; ++y)
                sink << static_cast<int>(pixels.at(indexOf(x, y))) << ' ';
            sink << '\n'; 
        }

    }

    Point Film::centerOfPixel(const FilmCoordinate x, const FilmCoordinate y) const {
        return Point{ // cast to double beause may go negative
            ((double) x - (double) x_resolution / 2) * extent / x_resolution,
            ((double) y - (double) y_resolution / 2) * extent / y_resolution,
            z
        };
    }


    size_t Film::indexOf(const FilmCoordinate x, const FilmCoordinate y) const {
        return x + x_resolution * y;
    }
}