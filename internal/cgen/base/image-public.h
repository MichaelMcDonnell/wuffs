// After editing this file, run "go generate" in the parent directory.

// Copyright 2017 The Wuffs Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// ---------------- Images

// wuffs_base__color_u32_argb_premul is an 8 bit per channel premultiplied
// Alpha, Red, Green, Blue color, as a uint32_t value. It is in word order, not
// byte order: its value is always 0xAARRGGBB, regardless of endianness.
typedef uint32_t wuffs_base__color_u32_argb_premul;

// --------

typedef uint8_t wuffs_base__pixel_blend;

// wuffs_base__pixel_blend encodes how to blend source and destination pixels,
// accounting for transparency. It encompasses the Porter-Duff compositing
// operators as well as the other blending modes defined by PDF.
//
// TODO: implement the other modes.
#define WUFFS_BASE__PIXEL_BLEND__SRC ((wuffs_base__pixel_blend)0)

// --------

// wuffs_base__pixel_format encodes the format of the bytes that constitute an
// image frame's pixel data.
//
// See https://github.com/google/wuffs/blob/master/doc/note/pixel-formats.md
//
// Do not manipulate its bits directly; they are private implementation
// details. Use methods such as wuffs_base__pixel_format__num_planes instead.
typedef struct {
  uint32_t repr;

#ifdef __cplusplus
  inline bool is_valid() const;
  inline uint32_t bits_per_pixel() const;
  inline bool is_indexed() const;
  inline bool is_interleaved() const;
  inline bool is_planar() const;
  inline uint32_t num_planes() const;
#endif  // __cplusplus

} wuffs_base__pixel_format;

static inline wuffs_base__pixel_format  //
wuffs_base__make_pixel_format(uint32_t repr) {
  wuffs_base__pixel_format f;
  f.repr = repr;
  return f;
}

  // Common 8-bit-depth pixel formats. This list is not exhaustive; not all
  // valid wuffs_base__pixel_format values are present.

#define WUFFS_BASE__PIXEL_FORMAT__INVALID 0x00000000

#define WUFFS_BASE__PIXEL_FORMAT__A 0x02000008

#define WUFFS_BASE__PIXEL_FORMAT__Y 0x10000008
#define WUFFS_BASE__PIXEL_FORMAT__YA_NONPREMUL 0x15000008
#define WUFFS_BASE__PIXEL_FORMAT__YA_PREMUL 0x16000008

#define WUFFS_BASE__PIXEL_FORMAT__YCBCR 0x20020888
#define WUFFS_BASE__PIXEL_FORMAT__YCBCRK 0x21038888
#define WUFFS_BASE__PIXEL_FORMAT__YCBCRA_NONPREMUL 0x25038888

#define WUFFS_BASE__PIXEL_FORMAT__YCOCG 0x30020888
#define WUFFS_BASE__PIXEL_FORMAT__YCOCGK 0x31038888
#define WUFFS_BASE__PIXEL_FORMAT__YCOCGA_NONPREMUL 0x35038888

#define WUFFS_BASE__PIXEL_FORMAT__INDEXED__BGRA_NONPREMUL 0x45040008
#define WUFFS_BASE__PIXEL_FORMAT__INDEXED__BGRA_PREMUL 0x46040008
#define WUFFS_BASE__PIXEL_FORMAT__INDEXED__BGRA_BINARY 0x47040008

#define WUFFS_BASE__PIXEL_FORMAT__BGR 0x40000888
#define WUFFS_BASE__PIXEL_FORMAT__BGRX 0x41008888
#define WUFFS_BASE__PIXEL_FORMAT__BGRA_NONPREMUL 0x45008888
#define WUFFS_BASE__PIXEL_FORMAT__BGRA_PREMUL 0x46008888
#define WUFFS_BASE__PIXEL_FORMAT__BGRA_BINARY 0x47008888

#define WUFFS_BASE__PIXEL_FORMAT__RGB 0x50000888
#define WUFFS_BASE__PIXEL_FORMAT__RGBX 0x51008888
#define WUFFS_BASE__PIXEL_FORMAT__RGBA_NONPREMUL 0x55008888
#define WUFFS_BASE__PIXEL_FORMAT__RGBA_PREMUL 0x56008888
#define WUFFS_BASE__PIXEL_FORMAT__RGBA_BINARY 0x57008888

#define WUFFS_BASE__PIXEL_FORMAT__CMY 0x60020888
#define WUFFS_BASE__PIXEL_FORMAT__CMYK 0x61038888

extern const uint32_t wuffs_base__pixel_format__bits_per_channel[16];

static inline bool  //
wuffs_base__pixel_format__is_valid(const wuffs_base__pixel_format* f) {
  return f->repr != 0;
}

// wuffs_base__pixel_format__bits_per_pixel returns the number of bits per
// pixel for interleaved pixel formats, and returns 0 for planar pixel formats.
static inline uint32_t  //
wuffs_base__pixel_format__bits_per_pixel(const wuffs_base__pixel_format* f) {
  if (((f->repr >> 16) & 0x03) != 0) {
    return 0;
  }
  return wuffs_base__pixel_format__bits_per_channel[0x0F & (f->repr >> 0)] +
         wuffs_base__pixel_format__bits_per_channel[0x0F & (f->repr >> 4)] +
         wuffs_base__pixel_format__bits_per_channel[0x0F & (f->repr >> 8)] +
         wuffs_base__pixel_format__bits_per_channel[0x0F & (f->repr >> 12)];
}

static inline bool  //
wuffs_base__pixel_format__is_indexed(const wuffs_base__pixel_format* f) {
  return (f->repr >> 18) & 0x01;
}

static inline bool  //
wuffs_base__pixel_format__is_interleaved(const wuffs_base__pixel_format* f) {
  return ((f->repr >> 16) & 0x03) == 0;
}

static inline bool  //
wuffs_base__pixel_format__is_planar(const wuffs_base__pixel_format* f) {
  return ((f->repr >> 16) & 0x03) != 0;
}

static inline uint32_t  //
wuffs_base__pixel_format__num_planes(const wuffs_base__pixel_format* f) {
  return ((f->repr >> 16) & 0x03) + 1;
}

#define WUFFS_BASE__PIXEL_FORMAT__NUM_PLANES_MAX 4

#define WUFFS_BASE__PIXEL_FORMAT__INDEXED__INDEX_PLANE 0
#define WUFFS_BASE__PIXEL_FORMAT__INDEXED__COLOR_PLANE 3

#ifdef __cplusplus

inline bool  //
wuffs_base__pixel_format::is_valid() const {
  return wuffs_base__pixel_format__is_valid(this);
}

inline uint32_t  //
wuffs_base__pixel_format::bits_per_pixel() const {
  return wuffs_base__pixel_format__bits_per_pixel(this);
}

inline bool  //
wuffs_base__pixel_format::is_indexed() const {
  return wuffs_base__pixel_format__is_indexed(this);
}

inline bool  //
wuffs_base__pixel_format::is_interleaved() const {
  return wuffs_base__pixel_format__is_interleaved(this);
}

inline bool  //
wuffs_base__pixel_format::is_planar() const {
  return wuffs_base__pixel_format__is_planar(this);
}

inline uint32_t  //
wuffs_base__pixel_format::num_planes() const {
  return wuffs_base__pixel_format__num_planes(this);
}

#endif  // __cplusplus

// --------

// wuffs_base__pixel_subsampling encodes whether sample values cover one pixel
// or cover multiple pixels.
//
// See https://github.com/google/wuffs/blob/master/doc/note/pixel-subsampling.md
//
// Do not manipulate its bits directly; they are private implementation
// details. Use methods such as wuffs_base__pixel_subsampling__bias_x instead.
typedef struct {
  uint32_t repr;

#ifdef __cplusplus
  inline uint32_t bias_x(uint32_t plane) const;
  inline uint32_t denominator_x(uint32_t plane) const;
  inline uint32_t bias_y(uint32_t plane) const;
  inline uint32_t denominator_y(uint32_t plane) const;
#endif  // __cplusplus

} wuffs_base__pixel_subsampling;

static inline wuffs_base__pixel_subsampling  //
wuffs_base__make_pixel_subsampling(uint32_t repr) {
  wuffs_base__pixel_subsampling s;
  s.repr = repr;
  return s;
}

#define WUFFS_BASE__PIXEL_SUBSAMPLING__NONE 0x00000000

#define WUFFS_BASE__PIXEL_SUBSAMPLING__444 0x000000
#define WUFFS_BASE__PIXEL_SUBSAMPLING__440 0x010100
#define WUFFS_BASE__PIXEL_SUBSAMPLING__422 0x101000
#define WUFFS_BASE__PIXEL_SUBSAMPLING__420 0x111100
#define WUFFS_BASE__PIXEL_SUBSAMPLING__411 0x303000
#define WUFFS_BASE__PIXEL_SUBSAMPLING__410 0x313100

static inline uint32_t  //
wuffs_base__pixel_subsampling__bias_x(const wuffs_base__pixel_subsampling* s,
                                      uint32_t plane) {
  uint32_t shift = ((plane & 0x03) * 8) + 6;
  return (s->repr >> shift) & 0x03;
}

static inline uint32_t  //
wuffs_base__pixel_subsampling__denominator_x(
    const wuffs_base__pixel_subsampling* s,
    uint32_t plane) {
  uint32_t shift = ((plane & 0x03) * 8) + 4;
  return ((s->repr >> shift) & 0x03) + 1;
}

static inline uint32_t  //
wuffs_base__pixel_subsampling__bias_y(const wuffs_base__pixel_subsampling* s,
                                      uint32_t plane) {
  uint32_t shift = ((plane & 0x03) * 8) + 2;
  return (s->repr >> shift) & 0x03;
}

static inline uint32_t  //
wuffs_base__pixel_subsampling__denominator_y(
    const wuffs_base__pixel_subsampling* s,
    uint32_t plane) {
  uint32_t shift = ((plane & 0x03) * 8) + 0;
  return ((s->repr >> shift) & 0x03) + 1;
}

#ifdef __cplusplus

inline uint32_t  //
wuffs_base__pixel_subsampling::bias_x(uint32_t plane) const {
  return wuffs_base__pixel_subsampling__bias_x(this, plane);
}

inline uint32_t  //
wuffs_base__pixel_subsampling::denominator_x(uint32_t plane) const {
  return wuffs_base__pixel_subsampling__denominator_x(this, plane);
}

inline uint32_t  //
wuffs_base__pixel_subsampling::bias_y(uint32_t plane) const {
  return wuffs_base__pixel_subsampling__bias_y(this, plane);
}

inline uint32_t  //
wuffs_base__pixel_subsampling::denominator_y(uint32_t plane) const {
  return wuffs_base__pixel_subsampling__denominator_y(this, plane);
}

#endif  // __cplusplus

// --------

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    wuffs_base__pixel_format pixfmt;
    wuffs_base__pixel_subsampling pixsub;
    uint32_t width;
    uint32_t height;
  } private_impl;

#ifdef __cplusplus
  inline void set(uint32_t pixfmt_repr,
                  uint32_t pixsub_repr,
                  uint32_t width,
                  uint32_t height);
  inline void invalidate();
  inline bool is_valid() const;
  inline wuffs_base__pixel_format pixel_format() const;
  inline wuffs_base__pixel_subsampling pixel_subsampling() const;
  inline wuffs_base__rect_ie_u32 bounds() const;
  inline uint32_t width() const;
  inline uint32_t height() const;
  inline uint64_t pixbuf_len() const;
#endif  // __cplusplus

} wuffs_base__pixel_config;

static inline wuffs_base__pixel_config  //
wuffs_base__null_pixel_config() {
  wuffs_base__pixel_config ret;
  ret.private_impl.pixfmt.repr = 0;
  ret.private_impl.pixsub.repr = 0;
  ret.private_impl.width = 0;
  ret.private_impl.height = 0;
  return ret;
}

// TODO: Should this function return bool? An error type?
static inline void  //
wuffs_base__pixel_config__set(wuffs_base__pixel_config* c,
                              uint32_t pixfmt_repr,
                              uint32_t pixsub_repr,
                              uint32_t width,
                              uint32_t height) {
  if (!c) {
    return;
  }
  if (pixfmt_repr) {
    uint64_t wh = ((uint64_t)width) * ((uint64_t)height);
    // TODO: handle things other than 1 byte per pixel.
    if (wh <= ((uint64_t)SIZE_MAX)) {
      c->private_impl.pixfmt.repr = pixfmt_repr;
      c->private_impl.pixsub.repr = pixsub_repr;
      c->private_impl.width = width;
      c->private_impl.height = height;
      return;
    }
  }

  c->private_impl.pixfmt.repr = 0;
  c->private_impl.pixsub.repr = 0;
  c->private_impl.width = 0;
  c->private_impl.height = 0;
}

static inline void  //
wuffs_base__pixel_config__invalidate(wuffs_base__pixel_config* c) {
  if (c) {
    c->private_impl.pixfmt.repr = 0;
    c->private_impl.pixsub.repr = 0;
    c->private_impl.width = 0;
    c->private_impl.height = 0;
  }
}

static inline bool  //
wuffs_base__pixel_config__is_valid(const wuffs_base__pixel_config* c) {
  return c && c->private_impl.pixfmt.repr;
}

static inline wuffs_base__pixel_format  //
wuffs_base__pixel_config__pixel_format(const wuffs_base__pixel_config* c) {
  return c ? c->private_impl.pixfmt : wuffs_base__make_pixel_format(0);
}

static inline wuffs_base__pixel_subsampling  //
wuffs_base__pixel_config__pixel_subsampling(const wuffs_base__pixel_config* c) {
  return c ? c->private_impl.pixsub : wuffs_base__make_pixel_subsampling(0);
}

static inline wuffs_base__rect_ie_u32  //
wuffs_base__pixel_config__bounds(const wuffs_base__pixel_config* c) {
  if (c) {
    wuffs_base__rect_ie_u32 ret;
    ret.min_incl_x = 0;
    ret.min_incl_y = 0;
    ret.max_excl_x = c->private_impl.width;
    ret.max_excl_y = c->private_impl.height;
    return ret;
  }

  wuffs_base__rect_ie_u32 ret;
  ret.min_incl_x = 0;
  ret.min_incl_y = 0;
  ret.max_excl_x = 0;
  ret.max_excl_y = 0;
  return ret;
}

static inline uint32_t  //
wuffs_base__pixel_config__width(const wuffs_base__pixel_config* c) {
  return c ? c->private_impl.width : 0;
}

static inline uint32_t  //
wuffs_base__pixel_config__height(const wuffs_base__pixel_config* c) {
  return c ? c->private_impl.height : 0;
}

// TODO: this is the right API for planar (not interleaved) pixbufs? Should it
// allow decoding into a color model different from the format's intrinsic one?
// For example, decoding a JPEG image straight to RGBA instead of to YCbCr?
static inline uint64_t  //
wuffs_base__pixel_config__pixbuf_len(const wuffs_base__pixel_config* c) {
  if (!c) {
    return 0;
  }
  if (wuffs_base__pixel_format__is_planar(&c->private_impl.pixfmt)) {
    // TODO: support planar pixel formats, concious of pixel subsampling.
    return 0;
  }
  uint32_t bits_per_pixel =
      wuffs_base__pixel_format__bits_per_pixel(&c->private_impl.pixfmt);
  if ((bits_per_pixel == 0) || ((bits_per_pixel % 8) != 0)) {
    // TODO: support fraction-of-byte pixels, e.g. 1 bit per pixel?
    return 0;
  }
  uint64_t bytes_per_pixel = bits_per_pixel / 8;

  uint64_t n =
      ((uint64_t)c->private_impl.width) * ((uint64_t)c->private_impl.height);
  if (n > (UINT64_MAX / bytes_per_pixel)) {
    return 0;
  }
  n *= bytes_per_pixel;

  if (wuffs_base__pixel_format__is_indexed(&c->private_impl.pixfmt)) {
    if (n > (UINT64_MAX - 1024)) {
      return 0;
    }
    n += 1024;
  }

  return n;
}

#ifdef __cplusplus

inline void  //
wuffs_base__pixel_config::set(uint32_t pixfmt_repr,
                              uint32_t pixsub_repr,
                              uint32_t width,
                              uint32_t height) {
  wuffs_base__pixel_config__set(this, pixfmt_repr, pixsub_repr, width, height);
}

inline void  //
wuffs_base__pixel_config::invalidate() {
  wuffs_base__pixel_config__invalidate(this);
}

inline bool  //
wuffs_base__pixel_config::is_valid() const {
  return wuffs_base__pixel_config__is_valid(this);
}

inline wuffs_base__pixel_format  //
wuffs_base__pixel_config::pixel_format() const {
  return wuffs_base__pixel_config__pixel_format(this);
}

inline wuffs_base__pixel_subsampling  //
wuffs_base__pixel_config::pixel_subsampling() const {
  return wuffs_base__pixel_config__pixel_subsampling(this);
}

inline wuffs_base__rect_ie_u32  //
wuffs_base__pixel_config::bounds() const {
  return wuffs_base__pixel_config__bounds(this);
}

inline uint32_t  //
wuffs_base__pixel_config::width() const {
  return wuffs_base__pixel_config__width(this);
}

inline uint32_t  //
wuffs_base__pixel_config::height() const {
  return wuffs_base__pixel_config__height(this);
}

inline uint64_t  //
wuffs_base__pixel_config::pixbuf_len() const {
  return wuffs_base__pixel_config__pixbuf_len(this);
}

#endif  // __cplusplus

// --------

typedef struct {
  wuffs_base__pixel_config pixcfg;

  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    uint64_t first_frame_io_position;
    bool first_frame_is_opaque;
  } private_impl;

#ifdef __cplusplus
  inline void set(uint32_t pixfmt_repr,
                  uint32_t pixsub_repr,
                  uint32_t width,
                  uint32_t height,
                  uint64_t first_frame_io_position,
                  bool first_frame_is_opaque);
  inline void invalidate();
  inline bool is_valid() const;
  inline uint64_t first_frame_io_position() const;
  inline bool first_frame_is_opaque() const;
#endif  // __cplusplus

} wuffs_base__image_config;

static inline wuffs_base__image_config  //
wuffs_base__null_image_config() {
  wuffs_base__image_config ret;
  ret.pixcfg = wuffs_base__null_pixel_config();
  ret.private_impl.first_frame_io_position = 0;
  ret.private_impl.first_frame_is_opaque = false;
  return ret;
}

// TODO: Should this function return bool? An error type?
static inline void  //
wuffs_base__image_config__set(wuffs_base__image_config* c,
                              uint32_t pixfmt_repr,
                              uint32_t pixsub_repr,
                              uint32_t width,
                              uint32_t height,
                              uint64_t first_frame_io_position,
                              bool first_frame_is_opaque) {
  if (!c) {
    return;
  }
  if (pixfmt_repr) {
    c->pixcfg.private_impl.pixfmt.repr = pixfmt_repr;
    c->pixcfg.private_impl.pixsub.repr = pixsub_repr;
    c->pixcfg.private_impl.width = width;
    c->pixcfg.private_impl.height = height;
    c->private_impl.first_frame_io_position = first_frame_io_position;
    c->private_impl.first_frame_is_opaque = first_frame_is_opaque;
    return;
  }

  c->pixcfg.private_impl.pixfmt.repr = 0;
  c->pixcfg.private_impl.pixsub.repr = 0;
  c->pixcfg.private_impl.width = 0;
  c->pixcfg.private_impl.height = 0;
  c->private_impl.first_frame_io_position = 0;
  c->private_impl.first_frame_is_opaque = 0;
}

static inline void  //
wuffs_base__image_config__invalidate(wuffs_base__image_config* c) {
  if (c) {
    c->pixcfg.private_impl.pixfmt.repr = 0;
    c->pixcfg.private_impl.pixsub.repr = 0;
    c->pixcfg.private_impl.width = 0;
    c->pixcfg.private_impl.height = 0;
    c->private_impl.first_frame_io_position = 0;
    c->private_impl.first_frame_is_opaque = 0;
  }
}

static inline bool  //
wuffs_base__image_config__is_valid(const wuffs_base__image_config* c) {
  return c && wuffs_base__pixel_config__is_valid(&(c->pixcfg));
}

static inline uint64_t  //
wuffs_base__image_config__first_frame_io_position(
    const wuffs_base__image_config* c) {
  return c ? c->private_impl.first_frame_io_position : 0;
}

static inline bool  //
wuffs_base__image_config__first_frame_is_opaque(
    const wuffs_base__image_config* c) {
  return c ? c->private_impl.first_frame_is_opaque : false;
}

#ifdef __cplusplus

inline void  //
wuffs_base__image_config::set(uint32_t pixfmt_repr,
                              uint32_t pixsub_repr,
                              uint32_t width,
                              uint32_t height,
                              uint64_t first_frame_io_position,
                              bool first_frame_is_opaque) {
  wuffs_base__image_config__set(this, pixfmt_repr, pixsub_repr, width, height,
                                first_frame_io_position, first_frame_is_opaque);
}

inline void  //
wuffs_base__image_config::invalidate() {
  wuffs_base__image_config__invalidate(this);
}

inline bool  //
wuffs_base__image_config::is_valid() const {
  return wuffs_base__image_config__is_valid(this);
}

inline uint64_t  //
wuffs_base__image_config::first_frame_io_position() const {
  return wuffs_base__image_config__first_frame_io_position(this);
}

inline bool  //
wuffs_base__image_config::first_frame_is_opaque() const {
  return wuffs_base__image_config__first_frame_is_opaque(this);
}

#endif  // __cplusplus

// --------

// Deprecated: use wuffs_base__pixel_blend instead.
//
// wuffs_base__animation_blend encodes, for an animated image, how to blend the
// transparent pixels of this frame with the existing canvas. In Porter-Duff
// compositing operator terminology:
//  - 0 means the frame may be transparent, and should be blended "src over
//    dst", also known as just "over".
//  - 1 means the frame may be transparent, and should be blended "src".
//  - 2 means the frame is completely opaque, so that "src over dst" and "src"
//    are equivalent.
//
// These semantics are conservative. It is valid for a completely opaque frame
// to have a blend value other than 2.
typedef uint8_t wuffs_base__animation_blend;

#define WUFFS_BASE__ANIMATION_BLEND__SRC_OVER_DST \
  ((wuffs_base__animation_blend)0)
#define WUFFS_BASE__ANIMATION_BLEND__SRC ((wuffs_base__animation_blend)1)
#define WUFFS_BASE__ANIMATION_BLEND__OPAQUE ((wuffs_base__animation_blend)2)

// --------

// wuffs_base__animation_disposal encodes, for an animated image, how to
// dispose of a frame after displaying it:
//  - None means to draw the next frame on top of this one.
//  - Restore Background means to clear the frame's dirty rectangle to "the
//    background color" (in practice, this means transparent black) before
//    drawing the next frame.
//  - Restore Previous means to undo the current frame, so that the next frame
//    is drawn on top of the previous one.
typedef uint8_t wuffs_base__animation_disposal;

#define WUFFS_BASE__ANIMATION_DISPOSAL__NONE ((wuffs_base__animation_disposal)0)
#define WUFFS_BASE__ANIMATION_DISPOSAL__RESTORE_BACKGROUND \
  ((wuffs_base__animation_disposal)1)
#define WUFFS_BASE__ANIMATION_DISPOSAL__RESTORE_PREVIOUS \
  ((wuffs_base__animation_disposal)2)

// --------

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    wuffs_base__rect_ie_u32 bounds;
    wuffs_base__flicks duration;
    uint64_t index;
    uint64_t io_position;
    wuffs_base__animation_disposal disposal;
    bool opaque_within_bounds;
    bool overwrite_instead_of_blend;
    wuffs_base__color_u32_argb_premul background_color;
  } private_impl;

#ifdef __cplusplus
  inline void update(wuffs_base__rect_ie_u32 bounds,
                     wuffs_base__flicks duration,
                     uint64_t index,
                     uint64_t io_position,
                     wuffs_base__animation_disposal disposal,
                     bool opaque_within_bounds,
                     bool overwrite_instead_of_blend,
                     wuffs_base__color_u32_argb_premul background_color);
  inline wuffs_base__rect_ie_u32 bounds() const;
  inline uint32_t width() const;
  inline uint32_t height() const;
  inline wuffs_base__flicks duration() const;
  inline uint64_t index() const;
  inline uint64_t io_position() const;
  inline wuffs_base__animation_disposal disposal() const;
  inline bool opaque_within_bounds() const;
  inline bool overwrite_instead_of_blend() const;
  inline wuffs_base__color_u32_argb_premul background_color() const;
#endif  // __cplusplus

} wuffs_base__frame_config;

static inline wuffs_base__frame_config  //
wuffs_base__null_frame_config() {
  wuffs_base__frame_config ret;
  ret.private_impl.bounds = wuffs_base__make_rect_ie_u32(0, 0, 0, 0);
  ret.private_impl.duration = 0;
  ret.private_impl.index = 0;
  ret.private_impl.io_position = 0;
  ret.private_impl.disposal = 0;
  ret.private_impl.opaque_within_bounds = false;
  ret.private_impl.overwrite_instead_of_blend = false;
  return ret;
}

static inline void  //
wuffs_base__frame_config__update(
    wuffs_base__frame_config* c,
    wuffs_base__rect_ie_u32 bounds,
    wuffs_base__flicks duration,
    uint64_t index,
    uint64_t io_position,
    wuffs_base__animation_disposal disposal,
    bool opaque_within_bounds,
    bool overwrite_instead_of_blend,
    wuffs_base__color_u32_argb_premul background_color) {
  if (!c) {
    return;
  }

  c->private_impl.bounds = bounds;
  c->private_impl.duration = duration;
  c->private_impl.index = index;
  c->private_impl.io_position = io_position;
  c->private_impl.disposal = disposal;
  c->private_impl.opaque_within_bounds = opaque_within_bounds;
  c->private_impl.overwrite_instead_of_blend = overwrite_instead_of_blend;
  c->private_impl.background_color = background_color;
}

static inline wuffs_base__rect_ie_u32  //
wuffs_base__frame_config__bounds(const wuffs_base__frame_config* c) {
  if (c) {
    return c->private_impl.bounds;
  }

  wuffs_base__rect_ie_u32 ret;
  ret.min_incl_x = 0;
  ret.min_incl_y = 0;
  ret.max_excl_x = 0;
  ret.max_excl_y = 0;
  return ret;
}

static inline uint32_t  //
wuffs_base__frame_config__width(const wuffs_base__frame_config* c) {
  return c ? wuffs_base__rect_ie_u32__width(&c->private_impl.bounds) : 0;
}

static inline uint32_t  //
wuffs_base__frame_config__height(const wuffs_base__frame_config* c) {
  return c ? wuffs_base__rect_ie_u32__height(&c->private_impl.bounds) : 0;
}

// wuffs_base__frame_config__duration returns the amount of time to display
// this frame. Zero means to display forever - a still (non-animated) image.
static inline wuffs_base__flicks  //
wuffs_base__frame_config__duration(const wuffs_base__frame_config* c) {
  return c ? c->private_impl.duration : 0;
}

// wuffs_base__frame_config__index returns the index of this frame. The first
// frame in an image has index 0, the second frame has index 1, and so on.
static inline uint64_t  //
wuffs_base__frame_config__index(const wuffs_base__frame_config* c) {
  return c ? c->private_impl.index : 0;
}

// wuffs_base__frame_config__io_position returns the I/O stream position before
// the frame config.
static inline uint64_t  //
wuffs_base__frame_config__io_position(const wuffs_base__frame_config* c) {
  return c ? c->private_impl.io_position : 0;
}

// wuffs_base__frame_config__disposal returns, for an animated image, how to
// dispose of this frame after displaying it.
static inline wuffs_base__animation_disposal  //
wuffs_base__frame_config__disposal(const wuffs_base__frame_config* c) {
  return c ? c->private_impl.disposal : 0;
}

// wuffs_base__frame_config__opaque_within_bounds returns whether all pixels
// within the frame's bounds are fully opaque. It makes no claim about pixels
// outside the frame bounds but still inside the overall image. The two
// bounding rectangles can differ for animated images.
//
// Its semantics are conservative. It is valid for a fully opaque frame to have
// this value be false: a false negative.
//
// If true, drawing the frame with WUFFS_BASE__PIXEL_BLEND__SRC and
// WUFFS_BASE__PIXEL_BLEND__SRC_OVER should be equivalent, in terms of
// resultant pixels, but the former may be faster.
static inline bool  //
wuffs_base__frame_config__opaque_within_bounds(
    const wuffs_base__frame_config* c) {
  return c && c->private_impl.opaque_within_bounds;
}

// wuffs_base__frame_config__overwrite_instead_of_blend returns, for an
// animated image, whether to ignore the previous image state (within the frame
// bounds) when drawing this incremental frame. Equivalently, whether to use
// WUFFS_BASE__PIXEL_BLEND__SRC instead of WUFFS_BASE__PIXEL_BLEND__SRC_OVER.
//
// The WebP spec (https://developers.google.com/speed/webp/docs/riff_container)
// calls this the "Blending method" bit. WebP's "Do not blend" corresponds to
// Wuffs' "overwrite_instead_of_blend".
static inline bool  //
wuffs_base__frame_config__overwrite_instead_of_blend(
    const wuffs_base__frame_config* c) {
  return c && c->private_impl.overwrite_instead_of_blend;
}

static inline wuffs_base__color_u32_argb_premul  //
wuffs_base__frame_config__background_color(const wuffs_base__frame_config* c) {
  return c ? c->private_impl.background_color : 0;
}

#ifdef __cplusplus

inline void  //
wuffs_base__frame_config::update(
    wuffs_base__rect_ie_u32 bounds,
    wuffs_base__flicks duration,
    uint64_t index,
    uint64_t io_position,
    wuffs_base__animation_disposal disposal,
    bool opaque_within_bounds,
    bool overwrite_instead_of_blend,
    wuffs_base__color_u32_argb_premul background_color) {
  wuffs_base__frame_config__update(
      this, bounds, duration, index, io_position, disposal,
      opaque_within_bounds, overwrite_instead_of_blend, background_color);
}

inline wuffs_base__rect_ie_u32  //
wuffs_base__frame_config::bounds() const {
  return wuffs_base__frame_config__bounds(this);
}

inline uint32_t  //
wuffs_base__frame_config::width() const {
  return wuffs_base__frame_config__width(this);
}

inline uint32_t  //
wuffs_base__frame_config::height() const {
  return wuffs_base__frame_config__height(this);
}

inline wuffs_base__flicks  //
wuffs_base__frame_config::duration() const {
  return wuffs_base__frame_config__duration(this);
}

inline uint64_t  //
wuffs_base__frame_config::index() const {
  return wuffs_base__frame_config__index(this);
}

inline uint64_t  //
wuffs_base__frame_config::io_position() const {
  return wuffs_base__frame_config__io_position(this);
}

inline wuffs_base__animation_disposal  //
wuffs_base__frame_config::disposal() const {
  return wuffs_base__frame_config__disposal(this);
}

inline bool  //
wuffs_base__frame_config::opaque_within_bounds() const {
  return wuffs_base__frame_config__opaque_within_bounds(this);
}

inline bool  //
wuffs_base__frame_config::overwrite_instead_of_blend() const {
  return wuffs_base__frame_config__overwrite_instead_of_blend(this);
}

inline wuffs_base__color_u32_argb_premul  //
wuffs_base__frame_config::background_color() const {
  return wuffs_base__frame_config__background_color(this);
}

#endif  // __cplusplus

// --------

typedef struct {
  wuffs_base__pixel_config pixcfg;

  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    wuffs_base__table_u8 planes[WUFFS_BASE__PIXEL_FORMAT__NUM_PLANES_MAX];
    // TODO: color spaces.
  } private_impl;

#ifdef __cplusplus
  inline wuffs_base__status set_from_slice(
      const wuffs_base__pixel_config* pixcfg,
      wuffs_base__slice_u8 pixbuf_memory);
  inline wuffs_base__status set_from_table(
      const wuffs_base__pixel_config* pixcfg,
      wuffs_base__table_u8 pixbuf_memory);
  inline wuffs_base__slice_u8 palette();
  inline wuffs_base__pixel_format pixel_format() const;
  inline wuffs_base__table_u8 plane(uint32_t p);
#endif  // __cplusplus

} wuffs_base__pixel_buffer;

static inline wuffs_base__pixel_buffer  //
wuffs_base__null_pixel_buffer() {
  wuffs_base__pixel_buffer ret;
  ret.pixcfg = wuffs_base__null_pixel_config();
  ret.private_impl.planes[0] = wuffs_base__empty_table_u8();
  ret.private_impl.planes[1] = wuffs_base__empty_table_u8();
  ret.private_impl.planes[2] = wuffs_base__empty_table_u8();
  ret.private_impl.planes[3] = wuffs_base__empty_table_u8();
  return ret;
}

static inline wuffs_base__status  //
wuffs_base__pixel_buffer__set_from_slice(wuffs_base__pixel_buffer* b,
                                         const wuffs_base__pixel_config* pixcfg,
                                         wuffs_base__slice_u8 pixbuf_memory) {
  if (!b) {
    return wuffs_base__make_status(wuffs_base__error__bad_receiver);
  }
  memset(b, 0, sizeof(*b));
  if (!pixcfg) {
    return wuffs_base__make_status(wuffs_base__error__bad_argument);
  }
  if (wuffs_base__pixel_format__is_planar(&pixcfg->private_impl.pixfmt)) {
    // TODO: support planar pixel formats, concious of pixel subsampling.
    return wuffs_base__make_status(wuffs_base__error__unsupported_option);
  }
  uint32_t bits_per_pixel =
      wuffs_base__pixel_format__bits_per_pixel(&pixcfg->private_impl.pixfmt);
  if ((bits_per_pixel == 0) || ((bits_per_pixel % 8) != 0)) {
    // TODO: support fraction-of-byte pixels, e.g. 1 bit per pixel?
    return wuffs_base__make_status(wuffs_base__error__unsupported_option);
  }
  uint64_t bytes_per_pixel = bits_per_pixel / 8;

  uint8_t* ptr = pixbuf_memory.ptr;
  uint64_t len = pixbuf_memory.len;
  if (wuffs_base__pixel_format__is_indexed(&pixcfg->private_impl.pixfmt)) {
    // Split a 1024 byte chunk (256 palette entries × 4 bytes per entry) from
    // the start of pixbuf_memory. We split from the start, not the end, so
    // that the both chunks' pointers have the same alignment as the original
    // pointer, up to an alignment of 1024.
    if (len < 1024) {
      return wuffs_base__make_status(
          wuffs_base__error__bad_argument_length_too_short);
    }
    wuffs_base__table_u8* tab =
        &b->private_impl.planes[WUFFS_BASE__PIXEL_FORMAT__INDEXED__COLOR_PLANE];
    tab->ptr = ptr;
    tab->width = 1024;
    tab->height = 1;
    tab->stride = 1024;
    ptr += 1024;
    len -= 1024;
  }

  uint64_t wh = ((uint64_t)pixcfg->private_impl.width) *
                ((uint64_t)pixcfg->private_impl.height);
  size_t width = (size_t)(pixcfg->private_impl.width);
  if ((wh > (UINT64_MAX / bytes_per_pixel)) ||
      (width > (SIZE_MAX / bytes_per_pixel))) {
    return wuffs_base__make_status(wuffs_base__error__bad_argument);
  }
  wh *= bytes_per_pixel;
  width *= bytes_per_pixel;
  if (wh > len) {
    return wuffs_base__make_status(
        wuffs_base__error__bad_argument_length_too_short);
  }

  b->pixcfg = *pixcfg;
  wuffs_base__table_u8* tab = &b->private_impl.planes[0];
  tab->ptr = ptr;
  tab->width = width;
  tab->height = pixcfg->private_impl.height;
  tab->stride = width;
  return wuffs_base__make_status(NULL);
}

static inline wuffs_base__status  //
wuffs_base__pixel_buffer__set_from_table(wuffs_base__pixel_buffer* b,
                                         const wuffs_base__pixel_config* pixcfg,
                                         wuffs_base__table_u8 pixbuf_memory) {
  if (!b) {
    return wuffs_base__make_status(wuffs_base__error__bad_receiver);
  }
  memset(b, 0, sizeof(*b));
  if (!pixcfg ||
      wuffs_base__pixel_format__is_planar(&pixcfg->private_impl.pixfmt)) {
    return wuffs_base__make_status(wuffs_base__error__bad_argument);
  }
  uint32_t bits_per_pixel =
      wuffs_base__pixel_format__bits_per_pixel(&pixcfg->private_impl.pixfmt);
  if ((bits_per_pixel == 0) || ((bits_per_pixel % 8) != 0)) {
    // TODO: support fraction-of-byte pixels, e.g. 1 bit per pixel?
    return wuffs_base__make_status(wuffs_base__error__unsupported_option);
  }
  uint64_t bytes_per_pixel = bits_per_pixel / 8;

  uint64_t width_in_bytes =
      ((uint64_t)pixcfg->private_impl.width) * bytes_per_pixel;
  if ((width_in_bytes > pixbuf_memory.width) ||
      (pixcfg->private_impl.height > pixbuf_memory.height)) {
    return wuffs_base__make_status(wuffs_base__error__bad_argument);
  }

  b->pixcfg = *pixcfg;
  b->private_impl.planes[0] = pixbuf_memory;
  return wuffs_base__make_status(NULL);
}

// wuffs_base__pixel_buffer__palette returns the palette color data. If
// non-empty, it will have length 1024.
static inline wuffs_base__slice_u8  //
wuffs_base__pixel_buffer__palette(wuffs_base__pixel_buffer* b) {
  if (b &&
      wuffs_base__pixel_format__is_indexed(&b->pixcfg.private_impl.pixfmt)) {
    wuffs_base__table_u8* tab =
        &b->private_impl.planes[WUFFS_BASE__PIXEL_FORMAT__INDEXED__COLOR_PLANE];
    if ((tab->width == 1024) && (tab->height == 1)) {
      return wuffs_base__make_slice_u8(tab->ptr, 1024);
    }
  }
  return wuffs_base__make_slice_u8(NULL, 0);
}

static inline wuffs_base__pixel_format  //
wuffs_base__pixel_buffer__pixel_format(const wuffs_base__pixel_buffer* b) {
  if (b) {
    return b->pixcfg.private_impl.pixfmt;
  }
  return wuffs_base__make_pixel_format(WUFFS_BASE__PIXEL_FORMAT__INVALID);
}

static inline wuffs_base__table_u8  //
wuffs_base__pixel_buffer__plane(wuffs_base__pixel_buffer* b, uint32_t p) {
  if (b && (p < WUFFS_BASE__PIXEL_FORMAT__NUM_PLANES_MAX)) {
    return b->private_impl.planes[p];
  }

  wuffs_base__table_u8 ret;
  ret.ptr = NULL;
  ret.width = 0;
  ret.height = 0;
  ret.stride = 0;
  return ret;
}

#ifdef __cplusplus

inline wuffs_base__status  //
wuffs_base__pixel_buffer::set_from_slice(const wuffs_base__pixel_config* pixcfg,
                                         wuffs_base__slice_u8 pixbuf_memory) {
  return wuffs_base__pixel_buffer__set_from_slice(this, pixcfg, pixbuf_memory);
}

inline wuffs_base__status  //
wuffs_base__pixel_buffer::set_from_table(const wuffs_base__pixel_config* pixcfg,
                                         wuffs_base__table_u8 pixbuf_memory) {
  return wuffs_base__pixel_buffer__set_from_table(this, pixcfg, pixbuf_memory);
}

inline wuffs_base__slice_u8  //
wuffs_base__pixel_buffer::palette() {
  return wuffs_base__pixel_buffer__palette(this);
}

inline wuffs_base__pixel_format  //
wuffs_base__pixel_buffer::pixel_format() const {
  return wuffs_base__pixel_buffer__pixel_format(this);
}

inline wuffs_base__table_u8  //
wuffs_base__pixel_buffer::plane(uint32_t p) {
  return wuffs_base__pixel_buffer__plane(this, p);
}

#endif  // __cplusplus

// --------

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    uint8_t TODO;
  } private_impl;

#ifdef __cplusplus
#endif  // __cplusplus

} wuffs_base__decode_frame_options;

#ifdef __cplusplus

#endif  // __cplusplus

// --------

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    // TODO: should the func type take restrict pointers?
    uint64_t (*func)(wuffs_base__slice_u8 dst,
                     wuffs_base__slice_u8 dst_palette,
                     wuffs_base__slice_u8 src);
  } private_impl;

#ifdef __cplusplus
  inline wuffs_base__status prepare(wuffs_base__pixel_format dst_format,
                                    wuffs_base__slice_u8 dst_palette,
                                    wuffs_base__pixel_format src_format,
                                    wuffs_base__slice_u8 src_palette,
                                    wuffs_base__pixel_blend blend);
  inline uint64_t swizzle_interleaved(wuffs_base__slice_u8 dst,
                                      wuffs_base__slice_u8 dst_palette,
                                      wuffs_base__slice_u8 src) const;
#endif  // __cplusplus

} wuffs_base__pixel_swizzler;

wuffs_base__status  //
wuffs_base__pixel_swizzler__prepare(wuffs_base__pixel_swizzler* p,
                                    wuffs_base__pixel_format dst_format,
                                    wuffs_base__slice_u8 dst_palette,
                                    wuffs_base__pixel_format src_format,
                                    wuffs_base__slice_u8 src_palette,
                                    wuffs_base__pixel_blend blend);

uint64_t  //
wuffs_base__pixel_swizzler__swizzle_interleaved(
    const wuffs_base__pixel_swizzler* p,
    wuffs_base__slice_u8 dst,
    wuffs_base__slice_u8 dst_palette,
    wuffs_base__slice_u8 src);

#ifdef __cplusplus

inline wuffs_base__status  //
wuffs_base__pixel_swizzler::prepare(wuffs_base__pixel_format dst_format,
                                    wuffs_base__slice_u8 dst_palette,
                                    wuffs_base__pixel_format src_format,
                                    wuffs_base__slice_u8 src_palette,
                                    wuffs_base__pixel_blend blend) {
  return wuffs_base__pixel_swizzler__prepare(this, dst_format, dst_palette,
                                             src_format, src_palette, blend);
}

uint64_t  //
wuffs_base__pixel_swizzler::swizzle_interleaved(
    wuffs_base__slice_u8 dst,
    wuffs_base__slice_u8 dst_palette,
    wuffs_base__slice_u8 src) const {
  return wuffs_base__pixel_swizzler__swizzle_interleaved(this, dst, dst_palette,
                                                         src);
}

#endif  // __cplusplus
