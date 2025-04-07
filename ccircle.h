#ifndef HGS_CIRCLE
#define HGS_CIRCLE
#include "cwidget.h"
namespace HGS
{
    
    class Circle : public Widget
    {
        public:
            Circle() = default;
            explicit Circle(const float x, const float y, const float r);
            explicit Circle(const float x, const float y, const float r, const SDL_Color& c);
            ~Circle() = default;

            bool isContains(float mx, float my) const override;
            void render(SDL_Renderer* r) const override;

            float getRadius() const;
            void setRadius(const float r);

        private:
            float radius {};
            SDL_Color bg{0xff, 0xff, 0x00, 0xff};
    };
}
#endif