#ifndef FILM_H
#define FILM_H

#include <cstdint>
#include <ostream>
#include <vector>

#include "Vector3.h"

/** Stand in for the film that receives the x-rays.
 * 
 * Basically an array of grayscale pixels, but with coordinates that tells where
 * it is in space, so that it is possible to direct rays at it.
*/
namespace xrt {
    using Intensity = uint8_t;      // Matches the PGM format.
    using FilmCoordinate = size_t;  // Index in a matrix.

    class Film {
        public:
            /* Assume that the film is parallel to the XY plane (vertical) at depth z
               along the Z axis and it is square, with the side extent units long. 
               
               This limits the scene setup somewhat, but it is enough to get the job done
               and to experiment with the distance between the screen and the target. 
               The target can be positioned directly in the 3D editor of choice at matching
               coordinates. */
            Film(const FilmCoordinate x_resolution, const FilmCoordinate y_resolution,
                 const double z, const double extent);

            /** Send light to the pixel, setting the intensity. 
             * 
             * 0 means black.
            */
            void expose(const FilmCoordinate x, const FilmCoordinate y, Intensity i);

            /** Writes the data as the content of a PGM file. 
             *
             *  Does not write the file directly to allow testing/decouple from the saving itself.
             *  Format as per specifications at https://en.wikipedia.org/wiki/Netpbm#File_formats
             *  Uses the optional spacing for ease of debugging, sacrificing run time and file size.
            */
            void dumpPGM(std::ostream& sink) const;

            /** Gives the reference position of the pixel, in space.
             *
             *  It is not the pixel center, it does not account for the dimensions of the 
             *  pixels itself. In practice it does not matter, as long as the screen is close
             *  enough to the target it is easy to obtain images without requiring sub-pixel-perfect
             *  accuracy.
             */
            Point positionsOfPixel(const FilmCoordinate x, const FilmCoordinate y) const;

            /** Resolution along the x axis.
             *  Public beacuse useful to loop over each pixel in the film.
            */
            const FilmCoordinate x_resolution;

            /** @see x_resolution, on the y axis. */
            const FilmCoordinate y_resolution;

        private:
            const double z;
            const double extent;

            /** Linearized 2D matrix, nothing fancy. */
            std::vector<Intensity> pixels;

            /** Shorthand to find the pixel in the pixel array.*/
            size_t indexOf(const FilmCoordinate x, const FilmCoordinate y) const;
    };
}

#endif