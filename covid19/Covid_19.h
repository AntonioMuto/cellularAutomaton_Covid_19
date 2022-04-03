#ifndef JALLEGRO_MYPANEL_H
#define JALLEGRO_MYPANEL_H

#include "jallegro/Panel.h"
#include "jallegro/Image.h"
#include "persona.h"
using namespace jallegro;

class Covid_19 : public Panel {

    public:
        Covid_19(Persona** world, int n, int m) {
            this->world = world;
            this->n = n;
            this->m = m;
        }

    protected:
        void paintComponent(Graphics g) {
            Panel::paintComponent(g);
            int blockSize = 5;
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < m; j++) {
                    if (world[i][j].getStato() == 0)
                        g.setColor(Color::white());
                    if (world[i][j].getStato() == 2)
		        g.setColor(Color::green());   
		    if (world[i][j].getStato() == 1)
                        g.setColor(Color::red());
		    if (world[i][j].getStato() == 3)
                        g.setColor(Color::yellow());
 	            if (world[i][j].getStato() == 4)
                        g.setColor(Color::black());	
                    g.fillRect(10+i*blockSize, 10+ j*blockSize, blockSize, blockSize);
                }
            }
        }

    private:
        Persona** world;
        int n;
        int m;
};

#endif
