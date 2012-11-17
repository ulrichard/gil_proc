#ifndef _GIL_PROC_THRESHOLD_HPP_
#define _GIL_PROC_THRESHOLD_HPP_

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{

template <typename DstChannel, typename SrcChannel>
struct threshold_channel
{
  DstChannel operator()(const SrcChannel& src ) const
  {
    const DstChannel dst_min = boost::gil::channel_traits<DstChannel>::min_value();
    const DstChannel dst_max = boost::gil::channel_traits<DstChannel>::max_value();
    return DstChannel( (src >= thres_value_) ? dst_max : dst_min );
  }

  threshold_channel( SrcChannel thres_value ) : thres_value_( thres_value )
  {}

  SrcChannel thres_value_;
};

template <typename DstPixel,typename SrcPixel>
struct threshold_pixel
{
  typedef typename boost::gil::channel_type<SrcPixel>::type SrcChannel_t;
  typedef typename boost::gil::channel_type<DstPixel>::type DstChannel_t;

  DstPixel operator()( const SrcPixel &src ) const
  {
    using namespace boost;
    DstPixel dst;
    gil::static_transform( src, dst, threshold_channel<DstChannel_t,SrcChannel_t>( thres_value_ ) );
    return dst;
  }

  threshold_pixel( SrcChannel_t thres_value ) : thres_value_( thres_value )
  {}

  SrcChannel_t thres_value_;

};

template <typename SrcView, typename DstView>
void threshold( const SrcView &src, const DstView &dst,
                typename boost::gil::channel_type<SrcView>::type thres_value )
{
  using namespace boost;
  //image view concept check
  gil::gil_function_requires< gil::ImageViewConcept<SrcView> >();
  gil::gil_function_requires< gil::ImageViewConcept<DstView> >();

  boost::gil::transform_pixels(
                         src, dst,
                         threshold_pixel<DstView::value_type,SrcView::value_type>( thres_value )
                        );
}

}

#endif // _GIL_PROC_THRESHOLD_HPP_
