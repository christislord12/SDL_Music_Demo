#include <SDL.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "thing.h"
#include <iostream>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1440;

struct context
{
	SDL_Renderer *renderer;
	SDL_Rect dest;
	SDL_Rect purpleSquare;
	int r;
	int g;
	int b;
	SDL_Rect renderLocation = {0, 0, 1920, 1440};
	SDL_Rect clipRect = {0, 0, 1920, 1440};
	SDL_Texture *titleScreen;
};

void handleEvents(context *ctx, SDL_Event *e)
{
	while (SDL_PollEvent(e))
	{
	}
}

void mainloop(void *arg)
{
	struct context *ctx = static_cast<context *>(arg);
	SDL_Event e;
	handleEvents(ctx, &e);
	SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
	SDL_RenderClear(ctx->renderer);
	SDL_RenderCopy(ctx->renderer, ctx->titleScreen, &ctx->clipRect, &ctx->renderLocation);
	SDL_SetRenderDrawColor(ctx->renderer, ctx->r, ctx->g, ctx->b, 255);
	SDL_RenderPresent(ctx->renderer);
}

int main(int argc, char *args[])
{
	SDL_Window *window = NULL;
	struct context ctx;
	if (Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &ctx.renderer);
	SDL_SetRenderDrawColor(ctx.renderer, 255, 0, 0, 255);
	Thing *thing = new Thing(34);
	thing->printNumber();
	Mix_Chunk *m_theme = Mix_LoadWAV("assets/audio/LetOurVoicesRiseLikeIncense.ogg");
	int canPlay = Mix_PlayChannel(-1, m_theme, 1);
	SDL_Surface *spriteSurface = IMG_Load("assets/img/title.jpg");
	ctx.titleScreen = SDL_CreateTextureFromSurface(ctx.renderer, spriteSurface);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(mainloop, &ctx, -1, 1);
#else
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		handleEvents(&ctx, &e);
		mainloop(&ctx);
	}
#endif
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();
	Mix_Quit();
	return 0;
}
