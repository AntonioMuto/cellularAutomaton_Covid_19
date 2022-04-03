#ifndef ALLEGRO_IMAGE_H
#define ALLEGRO_IMAGE_H

#include <iostream>
using namespace std;
namespace jallegro {
    class Image {
        public:
            Image() {
                loaded = false;
                image = nullptr;
            }

            void load(const char* filename) {
                image = al_load_bitmap(filename);
                loaded = true;
            }

            ~Image() {
                if(loaded and image != nullptr) {
                    al_destroy_bitmap(image);
                }
            }

            ALLEGRO_BITMAP* getImage() {
                return image;
            }

        private:
            Image(ALLEGRO_BITMAP* img) {
                image = img;
                loaded = true;
            }

            ALLEGRO_BITMAP* image;
            bool loaded;
            Image(const Image&);
            Image& operator=(const Image&);
    };
}
#endif
