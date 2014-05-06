#ifndef GFXVEC2_INCLUDED // -*- C++ -*-
#define GFXVEC2_INCLUDED
#if !defined(__GNUC__)
#  pragma once
#endif

/************************************************************************

  2D Vector class

  $Id: vec2.h 427 2004-09-27 04:45:31Z garland $

 ************************************************************************/

template<class T>
class TVec2 {
private:
    T elt[2];

public:
    // Standard constructors
    //
    TVec2(T s=0) { *this = s; }
    TVec2(T x, T y) { elt[0]=x; elt[1]=y; }

    // Copy constructors & assignment operators
    template<class U> TVec2(const TVec2<U>& v) { *this = v; }
    template<class U> TVec2(const U v[2]) { elt[0]=v[0]; elt[1]=v[1]; }
    template<class U> TVec2& operator=(const TVec2<U>& v)
	{ elt[0]=v[0];  elt[1]=v[1];  return *this; }
    TVec2& operator=(T s) { elt[0]=elt[1]=s; return *this; }


    // Descriptive interface
    //
    typedef T value_type;
    static int dim() { return 2; }


    // Access methods
    //
    operator       T*()       { return elt; }
    operator const T*() const { return elt; }

#ifndef HAVE_CASTING_LIMITS
    T& operator[](int i)       { return elt[i]; }
    T  operator[](int i) const { return elt[i]; }
    operator const T*()       { return elt; }
#endif

    // In-place arithmetic methods
    //
    inline TVec2& operator+=(const TVec2& v);
    inline TVec2& operator-=(const TVec2& v);
    inline TVec2& operator*=(T s);
    inline TVec2& operator/=(T s);
};

////////////////////////////////////////////////////////////////////////
//
// Method definitions
//
template<class T> inline TVec2<T>& TVec2<T>::operator+=(const TVec2<T>& v)
	{ elt[0] += v[0];   elt[1] += v[1];   return *this; }

template<class T> inline TVec2<T>& TVec2<T>::operator-=(const TVec2<T>& v)
	{ elt[0] -= v[0];   elt[1] -= v[1];   return *this; }

template<class T> inline TVec2<T>& TVec2<T>::operator*=(T s)
	{ elt[0] *= s;   elt[1] *= s;   return *this; }

template<class T> inline TVec2<T>& TVec2<T>::operator/=(T s)
	{ elt[0] /= s;   elt[1] /= s;   return *this; }

////////////////////////////////////////////////////////////////////////
//
// Operator defintions
//

template<class T>
inline TVec2<T> operator+(const TVec2<T> &u, const TVec2<T> &v)
	{ return TVec2<T>(u[0]+v[0], u[1]+v[1]); }

template<class T>
inline TVec2<T> operator-(const TVec2<T> &u, const TVec2<T> &v)
	{ return TVec2<T>(u[0]-v[0], u[1]-v[1]); }

template<class T> inline TVec2<T> operator-(const TVec2<T> &v)
	{ return TVec2<T>(-v[0], -v[1]); }

  template<class T, class N> inline TVec2<T> operator*(N s, const TVec2<T> &v)
	{ return TVec2<T>(v[0]*s, v[1]*s); }
  template<class T, class N> inline TVec2<T> operator*(const TVec2<T> &v, N s)
	{ return s*v; }

  template<class T, class N> inline TVec2<T> operator/(const TVec2<T> &v, N s)
	{ return TVec2<T>(v[0]/s, v[1]/s); }


template<class T> inline T operator*(const TVec2<T> &u, const TVec2<T>& v)
	{ return u[0]*v[0] + u[1]*v[1]; }

template<class T> inline TVec2<T> perp(const TVec2<T> &v)
	{ return TVec2<T>(v[1], -v[0]); }


////////////////////////////////////////////////////////////////////////
//
// Misc. function definitions
//

template<class T> inline T norm2(const TVec2<T>& v)  { return v*v; }
template<class T> inline T norm(const TVec2<T>& v)   { return sqrt(norm2(v)); }

template<class T> inline void unitize(TVec2<T>& v)
{
    T l = norm2(v);
    if( l!=1.0 && l!=0.0 )  v /= sqrt(l);
}

typedef TVec2<double> VEC2D;
typedef TVec2<float>  VEC2F;

// GFXVEC2_INCLUDED
#endif
