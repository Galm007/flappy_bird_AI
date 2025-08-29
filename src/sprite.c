#include "sprite.h"
#include "constants.h"
#include "game_state.h"

#include <raymath.h>

// draws a texture with the center of the texture as the origin
void draw_sprite(Texture2D* tex, Vector2 pos, float rot, bool fliph) {
	Vector2 size = { tex->width, tex->height };
	Rectangle src_rect = (Rectangle) { 0.0f, 0.0f, tex->width, tex->height };
	if (fliph)
		src_rect.width = -src_rect.width;
	
	DrawTexturePro(
		*tex,
		src_rect,
		(Rectangle) { pos.x, pos.y, size.x, size.y },
		Vector2Scale(size, 0.5f),
		rot,
		WHITE);
}

/*
 * This function shifts a rectangle to the top left to perfectly surround
 * the whole texture assuming that the origin of the texture is in the middle.
 */
Rectangle get_collider(Vector2 pos, Texture2D* tex) {
	return (Rectangle) {
		.x = pos.x - tex->width / 2.0f,
		.y = pos.y - tex->height / 2.0f,
		.width = tex->width,
		.height = tex->height,
	};
}

void render_score(Texture2D numbertex[10]) {
	int count = 0;
	int ones = 0; // keep track of ones because they have different width
	unsigned int digits[12];

	unsigned int s = score;
	while (s > 0) {
		unsigned int digit = s % 10;
		digits[count++] = digit;
		s -= digit;
		s /= 10;

		if (digit == 1)
			ones++;
	}

	if (score == 0) {
		count = 1;
		digits[0] = 0;
	}
	
	int width_diff = numbertex[0].width - numbertex[1].width;
	int score_width = numbertex[0].width * count - ones * width_diff;
	int x = (SCREEN_WIDTH - score_width) / 2.0f;

	for (int j = count - 1; j >= 0; j--)
	{
		DrawTexture(numbertex[digits[j]], x, SCREEN_HEIGHT / 8, WHITE);
		x += numbertex[digits[j]].width;
	}
}
