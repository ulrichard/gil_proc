#ifndef _GIL_PROC_INVERT_HPP_
#define _GIL_PROC_INVERT_HPP_

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

template <typename DstChannel>
struct invert_channel
{
  template <typename T>
  DstChannel operator()(const T& src ) const
  {
    return DstChannel( boost::gil::channel_invert(src) );
  }
};

template <typename DstPixel>
struct invert_pixel
{
  template<typename SrcPixel>
  DstPixel operator()( const SrcPixel &src ) const
  {
    using namespace boost;
    // pixel concept check
    gil::gil_function_requires< gil::PixelConcept<SrcPixel> >();
    gil::gil_function_requires< gil::PixelConcept<DstPixel> >();

    typedef typename gil::channel_type<DstPixel>::type result_channel_t;
    DstPixel dst;
    gil::static_transform( src, dst, invert_channel<result_channel_t>() );
    return dst;
  }
};

template <typename SrcView, typename DstView>
void invert( const SrcView &src, const DstView &dst )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  boost::gil::transform_pixels(
                         src, dst,
                         invert_pixel<DstView::value_type>()
                        );
}

}

#endif // _GIL_PROC_INVERT_HPP_
