#ifndef _GIL_PROC_LOGICAL_OPERATION_HPP_
#define _GIL_PROC_LOGICAL_OPERATION_HPP_

#include <cmath>
#include <utility>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

template <typename DstChannel>
struct logical_or_channel
{
  template <typename T>
  DstChannel operator()(const T& src1, const T& src2) const
  {
    return DstChannel( std::max(src1, src2) );
  }
};

template <typename DstPixel>
struct logical_or_pixel_functor
{
  template<typename SrcPixel1, typename SrcPixel2>
  DstPixel operator()( const SrcPixel1 &src1, const SrcPixel2 &src2 ) const
  {
    using namespace boost;
    typedef typename gil::channel_type<DstPixel>::type result_channel_t;
    DstPixel dst;
    gil::static_transform( src1, src2, dst, logical_or_channel<result_channel_t>() );
    return dst;
  }
};

template <typename SrcView1, typename SrcView2, typename DstView>
void logical_or( const SrcView1 &src1, const SrcView2 &src2, const DstView &dst )
{
  using namespace boost;

  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView1> >();
  gil::gil_function_requires< gil::ImageViewConcept<SrcView2> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  //color space concept check
  gil::gil_function_requires< gil::ColorSpacesCompatibleConcept<
          typename gil::color_space_type<SrcView1>::type,
          typename gil::color_space_type<SrcView2>::type> >();
  gil::gil_function_requires< gil::ColorSpacesCompatibleConcept<
          typename gil::color_space_type<SrcView1>::type,
          typename gil::color_space_type<DstView>::type> >();

  gil::transform_pixels(
                         src1, src2, dst,
                         logical_or_pixel_functor<DstView::value_type>()
                        );
}

template <typename DstChannel>
struct logical_and_channel
{
  template <typename T>
  DstChannel operator()(const T& src1, const T& src2) const
  {
    return DstChannel( std::min(src1, src2) );
  }
};

template <typename DstPixel>
struct logical_and_pixel_functor
{
  template<typename SrcPixel1, typename SrcPixel2>
  DstPixel operator()( const SrcPixel1 &src1, const SrcPixel2 &src2 ) const
  {
    using namespace boost;
    typedef typename gil::channel_type<DstPixel>::type result_channel_t;
    DstPixel dst;
    gil::static_transform( src1, src2, dst, logical_and_channel<result_channel_t>() );
    return dst;
  }
};

template <typename SrcView1, typename SrcView2, typename DstView>
void logical_and( const SrcView1 &src1, const SrcView2 &src2, const DstView &dst )
{
  using namespace boost;

  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView1> >();
  gil::gil_function_requires< gil::ImageViewConcept<SrcView2> >();
  gil::gil_function_requires< gil::MutableImageViewConcept<DstView> >();

  //color space concept check
  gil::gil_function_requires< gil::ColorSpacesCompatibleConcept<
          typename gil::color_space_type<SrcView1>::type,
          typename gil::color_space_type<SrcView2>::type> >();
  gil::gil_function_requires< gil::ColorSpacesCompatibleConcept<
          typename gil::color_space_type<SrcView1>::type,
          typename gil::color_space_type<DstView>::type> >();

  gil::transform_pixels(
                         src1, src2, dst,
                         logical_and_pixel_functor<DstView::value_type>()
                        );
}

}

#endif // _GIL_PROC_LOGICAL_OPERATION_HPP_
