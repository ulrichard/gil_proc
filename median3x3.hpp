#ifndef _GIL_PROC_MEDIAN3X3_HPP_
#define _GIL_PROC_MEDIAN3X3_HPP_

#include <cmath>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

#include "filter_8_neighbor.hpp"

namespace gil_proc
{



struct median3x3_functor
{
  template <typename Pixel>
  Pixel operator()( const Pixel &top_left,    const Pixel &top,        const Pixel &top_right,
                    const Pixel &left,        const Pixel &center, const Pixel &right,
                    const Pixel &bottom_left, const Pixel &bottom,     const Pixel &bottom_right ) const
  {
    using namespace boost;
    // pixel concept check
    gil::gil_function_requires< gil::PixelConcept<Pixel> >();

    typedef typename gil::channel_type<Pixel>::type channel_t;

    // pixel concept check
    gil::gil_function_requires< gil::PixelConcept<Pixel> >();

    Pixel dst;
    for ( int c = 0; c < gil::num_channels<Pixel>::value; ++c )
    {
      channel_t array[9] = {
        top_left[c],    top[c],    top_right[c],
        left[c],        center[c], right[c],
        bottom_left[c], bottom[c], bottom_right[c]
      };

      // calc median
      std::partial_sort( array, array + 5, array + 9 );
      dst[c] = array[4];
    }
    return dst;
  }
};


template <typename SrcView, typename DstView>
void median3x3( const SrcView& src, const DstView& dst )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  filer_8_neighbor( src, dst, median3x3_functor() );
}



}

#endif // _GIL_PROC_MEDIAN3X3_HPP_
