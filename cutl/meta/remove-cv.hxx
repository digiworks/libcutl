// file      : cutl/meta/remove-cv.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CUTL_META_REMOVE_CV_HXX
#define CUTL_META_REMOVE_CV_HXX

#include <cutl/meta/remove-c.hxx>
#include <cutl/meta/remove-v.hxx>

namespace cutl
{
  namespace meta
  {
    template <typename X>
    struct remove_cv
    {
      typedef typename remove_v<typename remove_c<X>::r>::r r;
    };
  }
}

#endif // CUTL_META_REMOVE_CV_HXX
