#ifndef HGS_SCORES
#define HGS_SCORES
#include <sstream>
#include "clabel.h"
namespace HGS
{
class Scores : public Label
{
public:
    Scores() = default;
    ~Scores();
    void updateScores();
    void reset();
    void incLeft();
    void setLeft(int l);
    int getLeft() const;
    void incRight();
    void setRight(int r);
    int getRight() const;

private:
    int left{0};
    int right{0};
};
Scores* ScoresFactory(const SDL_FRect& g, int l, int r, TTF_Font* font, const SDL_Color& bg, const SDL_Color& fc);
} // namespace HGS
#endif