#ifndef _GIL_PROC_SOBEL_HPP_
#define _GIL_PROC_SOBEL_HPP_

#include <cmath>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>
#include <boost/version.hpp>

#include "filter_8_neighbor.hpp"

namespace gil_proc
{

namespace sobel_detail
{
template <typename T>
inline T square( const T &x ) { return x * x; }
}

struct sobel_functor
{
  template <typename Pixel>
  Pixel operator()(const Pixel &top_left,    const Pixel &top,        const Pixel &top_right,
                   const Pixel &left,        const Pixel &/*center*/, const Pixel &right,
                   const Pixel &bottom_left, const Pixel &bottom,     const Pixel &bottom_right ) const
  {
    using namespace boost;
    // pixel concept check
    gil::gil_function_requires< gil::PixelConcept<Pixel> >();

    typedef typename gil::channel_type<Pixel>::type channel_t;
    const channel_t min_val = gil::channel_traits<channel_t>::min_value();
    const channel_t max_val = gil::channel_traits<channel_t>::max_value();

    Pixel dst;
    for ( int c = 0; c < gil::num_channels<Pixel>::value; ++c )
    {
      const float sum_square = sobel_detail::square(
            ( top_left[c]     * -1 ) +
            ( top[c]          * -2 ) +
            ( top_right[c]    * -1 ) +
            ( bottom_left[c]  *  1 ) +
            ( bottom[c]       *  2 ) +
            ( bottom_right[c] *  1 )
            )
          +
          sobel_detail::square(
            ( top_left[c]     * -1 ) +
            ( left[c]         * -2 ) +
            ( bottom_left[c]  * -1 ) +
            ( top_right[c]    *  1 ) +
            ( right[c]        *  2 ) +
            ( bottom_right[c] *  1 )
            );

#if BOOST_VERSION > 105000
      dst[c] = boost::algorithm::clamp( std::sqrt( sum_square ), min_val, max_val );
#else
      dst[c] = std::min<channel_t>(std::max<channel_t>(std::sqrt(sum_square), min_val), max_val);
#endif
    }
    return dst;
  }
};


template <typename SrcView, typename DstView>
void sobel( const SrcView& src, const DstView& dst )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  filer_8_neighbor( src, dst, sobel_functor() );
}

}

#endif // _GIL_PROC_SOBEL_HPP_
