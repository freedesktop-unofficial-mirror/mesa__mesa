/* -*- mode: C; c-file-style: "k&r"; tab-width 4; indent-tabs-mode: t; -*- */

/*
 * Copyright (C) 2014 Rob Clark <robclark@freedesktop.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Rob Clark <robclark@freedesktop.org>
 */

#include "pipe/p_defines.h"
#include "util/u_format.h"

#include "fd4_format.h"


/* Specifies the table of all the formats and their features. Also supplies
 * the helpers that look up various data in those tables.
 */

struct fd4_format {
	enum a4xx_vtx_fmt vtx;
	enum a4xx_tex_fmt tex;
	enum a4xx_color_fmt rb;
	enum a3xx_color_swap swap;
	boolean present;
};

#define RB4_NONE ~0

/* vertex + texture */
#define VT(pipe, fmt, rbfmt, swapfmt) \
	[PIPE_FORMAT_ ## pipe] = { \
		.present = 1, \
		.vtx = VFMT4_ ## fmt, \
		.tex = TFMT4_ ## fmt, \
		.rb = RB4_ ## rbfmt, \
		.swap = swapfmt \
	}

/* texture-only */
#define _T(pipe, fmt, rbfmt, swapfmt) \
	[PIPE_FORMAT_ ## pipe] = { \
		.present = 1, \
		.vtx = ~0, \
		.tex = TFMT4_ ## fmt, \
		.rb = RB4_ ## rbfmt, \
		.swap = swapfmt \
	}

/* vertex-only */
#define V_(pipe, fmt, rbfmt, swapfmt) \
	[PIPE_FORMAT_ ## pipe] = { \
		.present = 1, \
		.vtx = VFMT4_ ## fmt, \
		.tex = ~0, \
		.rb = RB4_ ## rbfmt, \
		.swap = swapfmt \
	}

static struct fd4_format formats[PIPE_FORMAT_COUNT] = {
	/* 8-bit */
	VT(R8_UNORM,   8_UNORM, NONE,     WZYX),
	V_(R8_SNORM,   8_SNORM, NONE,     WZYX),
	V_(R8_UINT,    8_UINT,  NONE,     WZYX),
	V_(R8_SINT,    8_SINT,  NONE,     WZYX),
	V_(R8_USCALED, 8_UINT,  NONE,     WZYX),
	V_(R8_SSCALED, 8_UINT,  NONE,     WZYX),

	_T(A8_UNORM,   8_UNORM, A8_UNORM, WZYX),
	_T(L8_UNORM,   8_UNORM, NONE,     WZYX),
	_T(I8_UNORM,   8_UNORM, NONE,     WZYX),

	/* 16-bit */
	V_(R16_UNORM,   16_UNORM, NONE,     WZYX),
	V_(R16_SNORM,   16_SNORM, NONE,     WZYX),
	V_(R16_UINT,    16_UINT,  NONE,     WZYX),
	V_(R16_SINT,    16_SINT,  NONE,     WZYX),
	V_(R16_USCALED, 16_UINT,  NONE,     WZYX),
	V_(R16_SSCALED, 16_UINT,  NONE,     WZYX),
	VT(R16_FLOAT,   16_FLOAT, NONE,     WZYX),

	VT(R8G8_UNORM,   8_8_UNORM, NONE,       WZYX),
	V_(R8G8_SNORM,   8_8_SNORM, NONE,       WZYX),
	V_(R8G8_UINT,    8_8_UINT,  NONE,       WZYX),
	V_(R8G8_SINT,    8_8_SINT,  NONE,       WZYX),
	V_(R8G8_USCALED, 8_8_UINT,  NONE,       WZYX),
	V_(R8G8_SSCALED, 8_8_SINT,  NONE,       WZYX),

	/*_T(B5G6R5_UNORM,   5_6_5_UNORM,   R5G6B5_UNORM,   WXYZ),*/
	_T(B5G5R5A1_UNORM, 5_5_5_1_UNORM, NONE, WXYZ),
	_T(B5G5R5X1_UNORM, 5_5_5_1_UNORM, NONE, WXYZ),
	_T(B4G4R4A4_UNORM, 4_4_4_4_UNORM, NONE, WXYZ),

	/* 24-bit */
	V_(R8G8B8_UNORM,   8_8_8_UNORM, NONE, WZYX),
	V_(R8G8B8_SNORM,   8_8_8_SNORM, NONE, WZYX),
	V_(R8G8B8_UINT,    8_8_8_UINT,  NONE, WZYX),
	V_(R8G8B8_SINT,    8_8_8_SINT,  NONE, WZYX),
	V_(R8G8B8_USCALED, 8_8_8_UINT,  NONE, WZYX),
	V_(R8G8B8_SSCALED, 8_8_8_SINT,  NONE, WZYX),

	/* 32-bit */
	VT(R32_FLOAT,   32_FLOAT, NONE,     WZYX),
	V_(R32_FIXED,   32_FIXED, NONE,     WZYX),

	V_(R16G16_UNORM,   16_16_UNORM, NONE,        WZYX),
	V_(R16G16_SNORM,   16_16_SNORM, NONE,        WZYX),
	V_(R16G16_UINT,    16_16_UINT,  NONE,        WZYX),
	V_(R16G16_SINT,    16_16_SINT,  NONE,        WZYX),
	V_(R16G16_USCALED, 16_16_UINT,  NONE,        WZYX),
	V_(R16G16_SSCALED, 16_16_SINT,  NONE,        WZYX),
	VT(R16G16_FLOAT,   16_16_FLOAT, NONE,        WZYX),

	VT(R8G8B8A8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, WZYX),
	_T(R8G8B8X8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, WZYX),
	_T(R8G8B8A8_SRGB,    8_8_8_8_UNORM, NONE,           WZYX),
	_T(R8G8B8X8_SRGB,    8_8_8_8_UNORM, NONE,           WZYX),
	V_(R8G8B8A8_SNORM,   8_8_8_8_SNORM, NONE,           WZYX),
	V_(R8G8B8A8_UINT,    8_8_8_8_UINT,  NONE,           WZYX),
	V_(R8G8B8A8_SINT,    8_8_8_8_SINT,  NONE,           WZYX),
	V_(R8G8B8A8_USCALED, 8_8_8_8_UINT,  NONE,           WZYX),
	V_(R8G8B8A8_SSCALED, 8_8_8_8_SINT,  NONE,           WZYX),

	VT(B8G8R8A8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, WXYZ),
	_T(B8G8R8X8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, WXYZ),
	VT(B8G8R8A8_SRGB,    8_8_8_8_UNORM, NONE,           WXYZ),
	_T(B8G8R8X8_SRGB,    8_8_8_8_UNORM, NONE,           WXYZ),

	VT(A8B8G8R8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, XYZW),
	_T(X8B8G8R8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, XYZW),
	_T(A8B8G8R8_SRGB,    8_8_8_8_UNORM, NONE,           XYZW),
	_T(X8B8G8R8_SRGB,    8_8_8_8_UNORM, NONE,           XYZW),

	VT(A8R8G8B8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, ZYXW),
	_T(X8R8G8B8_UNORM,   8_8_8_8_UNORM, R8G8B8A8_UNORM, ZYXW),
	_T(A8R8G8B8_SRGB,    8_8_8_8_UNORM, NONE,           ZYXW),
	_T(X8R8G8B8_SRGB,    8_8_8_8_UNORM, NONE,           ZYXW),

	V_(R10G10B10A2_UNORM,   10_10_10_2_UNORM, NONE/*R10G10B10A2_UNORM*/, WZYX),
	V_(R10G10B10A2_SNORM,   10_10_10_2_SNORM, NONE,              WZYX),
	V_(R10G10B10A2_UINT,    10_10_10_2_UINT,  NONE,              WZYX),
	V_(R10G10B10A2_USCALED, 10_10_10_2_UINT,  NONE,              WZYX),
	V_(R10G10B10A2_SSCALED, 10_10_10_2_SINT,  NONE,              WZYX),

	_T(Z24X8_UNORM,       X8Z24_UNORM, R8G8B8A8_UNORM, WZYX),
	_T(Z24_UNORM_S8_UINT, X8Z24_UNORM, R8G8B8A8_UNORM, WZYX),
	/*_T(Z32_FLOAT,         Z32_FLOAT,   R8G8B8A8_UNORM, WZYX),*/

	/* 48-bit */
	V_(R16G16B16_UNORM,   16_16_16_UNORM, NONE, WZYX),
	V_(R16G16B16_SNORM,   16_16_16_SNORM, NONE, WZYX),
	V_(R16G16B16_UINT,    16_16_16_UINT,  NONE, WZYX),
	V_(R16G16B16_SINT,    16_16_16_SINT,  NONE, WZYX),
	V_(R16G16B16_USCALED, 16_16_16_UINT,  NONE, WZYX),
	V_(R16G16B16_SSCALED, 16_16_16_SINT,  NONE, WZYX),
	V_(R16G16B16_FLOAT,   16_16_16_FLOAT, NONE, WZYX),

	/* 64-bit */
	V_(R16G16B16A16_UNORM,   16_16_16_16_UNORM, NONE,               WZYX),
	V_(R16G16B16A16_SNORM,   16_16_16_16_SNORM, NONE,               WZYX),
	V_(R16G16B16A16_UINT,    16_16_16_16_UINT,  NONE,               WZYX),
	V_(R16G16B16A16_SINT,    16_16_16_16_SINT,  NONE,               WZYX),
	V_(R16G16B16A16_USCALED, 16_16_16_16_UINT,  NONE,               WZYX),
	V_(R16G16B16A16_SSCALED, 16_16_16_16_SINT,  NONE,               WZYX),
	VT(R16G16B16A16_FLOAT,   16_16_16_16_FLOAT, NONE,               WZYX),
	_T(R16G16B16X16_FLOAT,   16_16_16_16_FLOAT, NONE,               WZYX),

	VT(R32G32_FLOAT,   32_32_FLOAT, NONE,        WZYX),
	V_(R32G32_FIXED,   32_32_FIXED, NONE,        WZYX),

	/* 96-bit */
	V_(R32G32B32_FLOAT,   32_32_32_FLOAT, NONE, WZYX),
	V_(R32G32B32_FIXED,   32_32_32_FIXED, NONE, WZYX),

	/* 128-bit */
	VT(R32G32B32A32_FLOAT,   32_32_32_32_FLOAT, NONE,               WZYX),
	_T(R32G32B32X32_FLOAT,   32_32_32_32_FLOAT, NONE,               WZYX),
	V_(R32G32B32A32_FIXED,   32_32_32_32_FIXED, NONE,               WZYX),
};

/* convert pipe format to vertex buffer format: */
enum a4xx_vtx_fmt
fd4_pipe2vtx(enum pipe_format format)
{
	if (!formats[format].present)
		return ~0;
	return formats[format].vtx;
}

/* convert pipe format to texture sampler format: */
enum a4xx_tex_fmt
fd4_pipe2tex(enum pipe_format format)
{
	if (!formats[format].present)
		return ~0;
	return formats[format].tex;
}

/* convert pipe format to MRT / copydest format used for render-target: */
enum a4xx_color_fmt
fd4_pipe2color(enum pipe_format format)
{
	if (!formats[format].present)
		return ~0;
	return formats[format].rb;
}

enum a3xx_color_swap
fd4_pipe2swap(enum pipe_format format)
{
	if (!formats[format].present)
		return WZYX;
	return formats[format].swap;
}

enum a4xx_tex_fetchsize
fd4_pipe2fetchsize(enum pipe_format format)
{
	switch (util_format_get_blocksizebits(format)) {
	case 8:   return TFETCH4_1_BYTE;
	case 16:  return TFETCH4_2_BYTE;
	case 32:  return TFETCH4_4_BYTE;
	case 64:  return TFETCH4_8_BYTE;
	case 128: return TFETCH4_16_BYTE;
	default:
		debug_printf("Unknown block size for format %s: %d\n",
				util_format_name(format),
				util_format_get_blocksizebits(format));
		return TFETCH4_1_BYTE;
	}
}

/* we need to special case a bit the depth/stencil restore, because we are
 * using the texture sampler to blit into the depth/stencil buffer, *not*
 * into a color buffer.  Otherwise fd4_tex_swiz() will do the wrong thing,
 * as it is assuming that you are sampling into normal render target..
 *
 * TODO looks like we can probably share w/ a3xx..
 */
enum pipe_format
fd4_gmem_restore_format(enum pipe_format format)
{
	switch (format) {
	case PIPE_FORMAT_Z24X8_UNORM:
	case PIPE_FORMAT_Z24_UNORM_S8_UINT:
		return PIPE_FORMAT_R8G8B8A8_UNORM;
	case PIPE_FORMAT_Z16_UNORM:
		return PIPE_FORMAT_R8G8_UNORM;
	default:
		return format;
	}
}

enum a4xx_depth_format
fd4_pipe2depth(enum pipe_format format)
{
	switch (format) {
	case PIPE_FORMAT_Z16_UNORM:
		return DEPTH4_16;
	case PIPE_FORMAT_Z24X8_UNORM:
	case PIPE_FORMAT_Z24_UNORM_S8_UINT:
	case PIPE_FORMAT_X8Z24_UNORM:
	case PIPE_FORMAT_S8_UINT_Z24_UNORM:
		return DEPTH4_24_8;
	default:
		return ~0;
	}
}

static inline enum a4xx_tex_swiz
tex_swiz(unsigned swiz)
{
	switch (swiz) {
	default:
	case PIPE_SWIZZLE_RED:   return A4XX_TEX_X;
	case PIPE_SWIZZLE_GREEN: return A4XX_TEX_Y;
	case PIPE_SWIZZLE_BLUE:  return A4XX_TEX_Z;
	case PIPE_SWIZZLE_ALPHA: return A4XX_TEX_W;
	case PIPE_SWIZZLE_ZERO:  return A4XX_TEX_ZERO;
	case PIPE_SWIZZLE_ONE:   return A4XX_TEX_ONE;
	}
}

uint32_t
fd4_tex_swiz(enum pipe_format format, unsigned swizzle_r, unsigned swizzle_g,
		unsigned swizzle_b, unsigned swizzle_a)
{
	const struct util_format_description *desc =
			util_format_description(format);
	unsigned char swiz[4] = {
			swizzle_r, swizzle_g, swizzle_b, swizzle_a,
	}, rswiz[4];

	util_format_compose_swizzles(desc->swizzle, swiz, rswiz);

	return A4XX_TEX_CONST_0_SWIZ_X(tex_swiz(rswiz[0])) |
			A4XX_TEX_CONST_0_SWIZ_Y(tex_swiz(rswiz[1])) |
			A4XX_TEX_CONST_0_SWIZ_Z(tex_swiz(rswiz[2])) |
			A4XX_TEX_CONST_0_SWIZ_W(tex_swiz(rswiz[3]));
}
