#include "cscores.h"
namespace HGS
{
Scores::~Scores()
{
    SDL_Log("~Scores\n");
}

void Scores::updateScores()
{
    std::stringstream parser;
    parser << left;
    parser << " : ";
    parser << right;
    setText(parser.str());
}
void Scores::reset()
{
    left = 0;
    right = 0;
}
void Scores::incLeft()
{
    ++left;
}
void Scores::setLeft(int l)
{
    left = l;
}
void Scores::incRight()
{
    ++right;
}
void Scores::setRight(int r)
{
    right = r;
}
int Scores::getLeft() const
{
    return left;
}
int Scores::getRight() const
{
    return right;
}
Scores* ScoresFactory(const SDL_FRect& g, int l, int r, TTF_Font* font, const SDL_Color& bg, const SDL_Color& fc)
{
    Scores* scores{new Scores};
    scores->setPos({g.x, g.y});
    scores->setWH({g.w, g.h});
    scores->setText("");
    scores->setFont(font);
    scores->setBG(bg);
    scores->setFC(fc);
    scores->setLeft(l);
    scores->setRight(r);
    return scores;
}
} // namespace HGS