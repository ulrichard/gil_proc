#ifndef _GIL_PROC_ADD_HPP_
#define _GIL_PROC_ADD_HPP_

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

template <typename DstChannel>
struct add_channel
{
  template <typename T>
  DstChannel operator()(const T& src1, const T& src2) const
  {
    const DstChannel max_val = boost::gil::channel_traits<DstChannel>::max_value();
    return DstChannel( (src1 + src2) > max_val ? max_val : src1 + src2 );

  }
};

template <typename DstPixel>
struct add_pixel
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
    gil::static_transform( src1, src2, dst, add_channel<result_channel_t>() );
    return dst;
  }
};

template <typename SrcView1, typename SrcView2, typename DstView>
void add( const SrcView1 &src1, const SrcView2 &src2, const DstView &dst )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView1> >();
  gil::gil_function_requires< gil::ImageViewConcept<SrcView2> >();
  gil::gil_function_requires< gil::ImageViewConcept<DstView> >();

  boost::gil::transform_pixels(
                         src1, src2, dst,
                         add_pixel<DstView::value_type>()
                        );
}

}

#endif // _GIL_PROC_ADD_HPP_
