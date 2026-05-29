#include <gl_playground/game.h>
#include <stddef.h>

int main()
{
    Game *game = game_create();

    if(!game)
        return 1;

    game_run(game);

    game_destroy(game);
    game = NULL;

    return 0;
}
