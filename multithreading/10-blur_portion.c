#include "multithreading.h"

/**
 * apply_kernel - apply convolutin kernel to img
 * @img: pointer to original image
 * @kernel: pointer to kernel
 * @px: x coordinate of pixel
 * @py: y coordinate of pixel
 * Return: pointer to new blurred pixel
 */
pixel_t apply_kernel(const img_t *img, const kernel_t *kernel,
size_t px, size_t py)
{
int half = kernel->size / 2;
float sum_r = 0, sum_g = 0, sum_b = 0, sum = 0;
int kx, ky, nx, ny;
size_t pixel_idx;
float kernel_value;
pixel_t result;
for (ky = -half; ky <= half; ky++)
{
for (kx = -half; kx <= half; kx++)
{
nx = px + kx; /* neighbouring pixel X-coordinate */
ny = py + ky; /* neighbouring pixel Y-coordinate */
if (nx >= 0 && nx < (int)img->w && ny >= 0 && ny < (int)img->h)
{
pixel_idx = (ny *img->w) + nx;
kernel_value = kernel->matrix[ky + half][kx + half];
sum_r += img->pixels[pixel_idx].r * kernel_value;
sum_g += img->pixels[pixel_idx].g * kernel_value;
sum_b += img->pixels[pixel_idx].b * kernel_value;
sum += kernel_value;
}
}
}
result.r = (uint8_t)(sum_r / sum);
result.g = (uint8_t)(sum_g / sum);
result.b = (uint8_t)(sum_b / sum);

return (result);
}

/**
 * blur_portion  - blurs a portion of an image using a Gaussian Blur
 * @portion: pointer to structure
 * Return: Nothing
 */
void blur_portion(blur_portion_t const *portion)
{
size_t x, y, pixel_idx;
if (!portion)
return;
for (y = portion->y; y < portion->y + portion->h; y++)
{
for (x = portion->x; x < portion->x + portion->w; x++)
{
pixel_idx = y * portion->img->w + x;
portion->img_blur->pixels[pixel_idx] = apply_kernel(portion->img,
portion->kernel, x, y);
}
}
}
