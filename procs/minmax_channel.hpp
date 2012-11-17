#ifndef _GIL_PROC_MINMAX_CHANNEL_HPP_
#define _GIL_PROC_MINMAX_CHANNEL_HPP_

#include <cmath>
#include <utility>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/algorithm.hpp>

namespace gil_proc
{


template <typename Pixel>
std::pair<typename boost::gil::channel_type<Pixel>::type,
typename boost::gil::channel_type<Pixel>::type>
minmax_channel_of_pixel( const Pixel &pixel )
{
  using namespace boost;
  return std::make_pair( gil::static_min(pixel), gil::static_max(pixel) );
}


namespace detail
{

template <typename Pixel>
struct minmax_channel_functor
{
  typedef typename boost::gil::channel_type<Pixel>::type channel_t;

  void operator()( const Pixel &pixel )
  {
    // pixel concept check
    boost::gil::gil_function_requires< boost::gil::PixelConcept<Pixel> >();

    const std::pair<channel_t,channel_t> minmax =
        minmax_channel_of_pixel( pixel );
    min = std::min( min, minmax.first );
    max = std::max( max, minmax.second );
  }

  channel_t min;
  channel_t max;

  minmax_channel_functor() :
    min( boost::gil::channel_traits<channel_t>::max_value() ),
    max( boost::gil::channel_traits<channel_t>::min_value() )
  {}

};

}

template <typename View>
std::pair<typename boost::gil::channel_type<View>::type,
          typename boost::gil::channel_type<View>::type>
minmax_channel( const View &src )
{
  // image view concept check
  boost::gil::gil_function_requires< boost::gil::ImageViewConcept<View> >();

  typedef detail::minmax_channel_functor<View::value_type> minmax_functor_t;
  const minmax_functor_t &minmax_func = boost::gil::for_each_pixel( src, minmax_functor_t() );
  return std::make_pair( minmax_func.min, minmax_func.max );
}

}

#endif // _GIL_PROC_MINMAX_CHANNEL_HPP_
