#ifndef JALLEGRO_GRAPHICS_H
#define JALLEGRO_GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Color.h"
#include "Image.h"

namespace jallegro {
    class Graphics {

    public:

        void fillRect(int x, int y, int w, int h) {
            al_draw_filled_rectangle(x, y, x + w, y + h, color.toAllegro());
        }

        void fillCircle(int x, int y, int r) {
            al_draw_filled_circle(x, y, r, color.toAllegro());
        }

        void drawImage(Image* i, int x, int y) {
            if(i->getImage() != nullptr)
                al_draw_bitmap(i->getImage(), x, y, 0);
        }

        void drawImage(Image* i, int x, int y, int width, int height) {
            if(i->getImage() != nullptr)
                al_draw_scaled_bitmap(i->getImage(),
                                           0, 0, al_get_bitmap_width(i->getImage()), al_get_bitmap_height(i->getImage()),
                                           0, 0, width, height, 0);
        }

        void setColor(Color c) {
            color = c;
        }

    private:
        Color color;
    };
}

#endif