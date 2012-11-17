#ifndef _GIL_PROC_FILER_8_NEIGHBOR_HPP_
#define _GIL_PROC_FILER_8_NEIGHBOR_HPP_

#include <cmath>

#include <boost/optional.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

namespace detail
{

template <typename Pixel>
Pixel min_pixel()
{
  using namespace boost;
  typedef typename gil::channel_type<Pixel>::type channel_t;
  Pixel p;
  gil::static_fill( p, gil::channel_traits<channel_t>::min_value() );
  return p;
}

template <typename Pixel>
Pixel max_pixel()
{
  using namespace boost;
  typedef typename gil::channel_type<Pixel>::type channel_t;
  Pixel p;
  gil::static_fill( p, gil::channel_traits<channel_t>::max_value() );
  return p;
}
}

template <typename SrcView, typename DstView, typename Func>
void filer_8_neighbor( const SrcView& src, const DstView& dst, Func func )
{
  using namespace boost;

  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  typedef typename SrcView::xy_locator SrcXYLocater_t;
  typedef typename SrcView::value_type SrcPixel_t;
  typedef typename gil::channel_type<SrcView>::type SrcChannel_t;
  typedef typename SrcXYLocater_t::cached_location_t SrcChachedLocation_t;

  typedef typename DstView::value_type DstPixel_t;

  SrcXYLocater_t src_loc = src.xy_at( 0, 0 );

  const SrcChachedLocation_t top_l    = src_loc.cache_location( -1, -1 );
  const SrcChachedLocation_t top      = src_loc.cache_location(  0, -1 );
  const SrcChachedLocation_t top_r    = src_loc.cache_location(  1, -1 );
  const SrcChachedLocation_t left     = src_loc.cache_location( -1,  0 );
  const SrcChachedLocation_t right    = src_loc.cache_location(  1,  0 );
  const SrcChachedLocation_t bottom_l = src_loc.cache_location( -1,  1 );
  const SrcChachedLocation_t bottom   = src_loc.cache_location(  0,  1 );
  const SrcChachedLocation_t bottom_r = src_loc.cache_location(  1,  1 );

  const int w = src.width();
  const int h = src.height();

  for( int y = 0; y < h; ++y )
  {
    typename DstView::x_iterator dst_it = dst.row_begin(y);

    for( int x = 0; x < w; ++x )
    {

      const bool on_top = y < 1;
      const bool on_bottom = y >= (h - 1);
      const bool on_left = x < 1;
      const bool on_right = x >= (w - 1);

      const SrcPixel_t &center_pix   = src_loc( 0, 0 );
      const SrcPixel_t &top_l_pix    = ( on_top || on_left )     ? center_pix : src_loc[top_l];
      const SrcPixel_t &top_pix      = ( on_top )                ? center_pix : src_loc[top];
      const SrcPixel_t &top_r_pix    = ( on_top || on_right )    ? center_pix : src_loc[top_r];
      const SrcPixel_t &left_pix     = ( on_left )               ? center_pix : src_loc[left];
      const SrcPixel_t &right_pix    = ( on_right )              ? center_pix : src_loc[right];
      const SrcPixel_t &bottom_l_pix = ( on_bottom || on_left )  ? center_pix : src_loc[bottom_l];
      const SrcPixel_t &bottom_pix   = ( on_bottom )             ? center_pix : src_loc[bottom];
      const SrcPixel_t &bottom_r_pix = ( on_bottom || on_right ) ? center_pix : src_loc[bottom_r];

      gil::color_convert( func(
                           top_l_pix,    top_pix,    top_r_pix,
                           left_pix,     center_pix, right_pix,
                           bottom_l_pix, bottom_pix, bottom_r_pix
                               ),
                         *dst_it );

      ++dst_it;
      ++src_loc.x();
    }
    src_loc += gil::point2<std::ptrdiff_t>( -w, 1 );
  }
}


}

#endif // _GIL_PROC_FILER_8_NEIGHBOR_HPP_
