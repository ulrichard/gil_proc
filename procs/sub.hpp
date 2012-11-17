#ifndef _GIL_PROC_SUB_HPP_
#define _GIL_PROC_SUB_HPP_

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

template <typename DstChannel>
struct sub_channel
{
  template <typename T>
  DstChannel operator()(const T& src1, const T& src2) const
  {
    const DstChannel min_val = boost::gil::channel_traits<DstChannel>::min_value();
    return DstChannel( (src2 > src1) ? min_val : (src1 - src2) );

  }
};

template <typename DstPixel>
struct sub_pixel
{
  template<typename SrcPixel1, typename SrcPixel2>
  DstPixel operator()( const SrcPixel1 &src1, const SrcPixel2 &src2 ) const
  {
    using namespace boost;
    // pixel concept check
    gil::gil_function_requires< gil::PixelConcept<SrcPixel1> >();
    gil::gil_function_requires< gil::PixelConcept<SrcPixel2> >();
    gil::gil_function_requires< gil::PixelConcept<DstPixel> >();

    typedef typename gil::channel_type<DstPixel>::type result_channel_t;
    DstPixel dst;
    gil::static_transform( src1, src2, dst, sub_channel<result_channel_t>() );
    return dst;
  }
};

template <typename SrcView1, typename SrcView2, typename DstView>
void sub( const SrcView1 &src1, const SrcView2 &src2, const DstView &dst )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView1> >();
  gil::gil_function_requires< gil::ImageViewConcept<SrcView2> >();
  gil::gil_function_requires< gil::ImageViewConcept<DstView> >();

  boost::gil::transform_pixels(
                         src1, src2, dst,
                         sub_pixel<DstView::value_type>()
                        );
}

}

#endif // _GIL_PROC_SUB_HPP_
