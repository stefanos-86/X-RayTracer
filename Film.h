#ifndef FILM_H
#define FILM_H

#include <cstdint>
#include <ostream>
#include <vector>

#include "Vector3.h"

/** Stand in for the film that receives the x-rays.
 * 
 * Basically an array of grayscale pixels.
*/
namespace xrt {
    using Intensity = uint8_t;
    using FilmCoordinate = size_t;

    class Film {
        public:
            /* Assume that the film is parallel to the XY plane (vertical) at depth z
               along the Z axis and it is square, with the side extent units long. */
            Film(const FilmCoordinate x_resolution, const FilmCoordinate y_resolution,
                 const double z, const double extent);

            void set(const FilmCoordinate x, const FilmCoordinate y, Intensity i);

            /** Writes the data as the content of a PGM file. 
             *
             *  Does not write the file directly to allow testing/decouple from the saving itself.
             *  Format as per specifications at https://en.wikipedia.org/wiki/Netpbm#File_formats
             *  Uses the optional spacing for ease of debugging, sacrificing run time and file size.
            */
            void dumpPGM(std::ostream& sink) const;

            Point centerOfPixel(const FilmCoordinate x, const FilmCoordinate y) const;

            const FilmCoordinate x_resolution;
            const FilmCoordinate y_resolution;

        private:
            const double z;
            const double extent;

            /** Linearized 2D matrix, nothing fancy. */
            std::vector<Intensity> pixels;

            size_t indexOf(const FilmCoordinate x, const FilmCoordinate y) const;
    };
}

#endif