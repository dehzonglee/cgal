// ======================================================================
//
// Copyright (c) 1999,2003 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// 
// release       : 
// release_date  : 
// 
// file          : Quotient.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Stefan Schirra, Sylvain Pion
//
// The template class Quotient<NT> is based on the LEDA class
// leda_rational written by Stefan Naeher and Christian Uhrig.
// It is basically a templated version with restricted functionality
// of the version of rational in LEDA release 3.3.
// The modification was done by Stefan.Schirra@mpi-sb.mpg.de
//
// coordinator   : MPI, Saarbruecken
// ======================================================================

#ifndef CGAL_QUOTIENT_H
#define CGAL_QUOTIENT_H

#include <CGAL/basic.h>
#include <utility>

#ifndef CGAL_CFG_NO_LOCALE
#  include <locale>
#else
#  include <cctype>
#endif

#include <CGAL/Interval_arithmetic.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

  // Mini helper to prevent clashes when NT == int.
  template < typename T >
  struct Int_if_not_int { typedef int type; };

  template <>
  struct Int_if_not_int<int> { struct type{}; };

} // namespace CGALi

#define CGAL_int(T) typename CGALi::Int_if_not_int<T>::type


template <class NT_>
class Quotient
{
 public:
  typedef NT_        NT;
  typedef Tag_false  Has_gcd;
  typedef Tag_true   Has_division;
  typedef Tag_false  Has_sqrt;
  
  Quotient() : num( NT(0) ), den( NT(1) ) {}

  template <class T>
  Quotient(const T& n) : num(n), den( NT(1) ) {}

  Quotient(const NT& n, const NT& d) : num(n), den(d)
  { CGAL_kernel_precondition( d!= NT(0) ); }
  
  Quotient(const Quotient<NT>& r) : num(r.num), den(r.den) {}
  
  Quotient<NT>& operator+= (const Quotient<NT>& r);
  Quotient<NT>& operator-= (const Quotient<NT>& r);
  Quotient<NT>& operator*= (const Quotient<NT>& r);
  Quotient<NT>& operator/= (const Quotient<NT>& r);
  Quotient<NT>& operator+= (const NT& r);
  Quotient<NT>& operator-= (const NT& r);
  Quotient<NT>& operator*= (const NT& r);
  Quotient<NT>& operator/= (const NT& r);
  Quotient<NT>& operator+= (const CGAL_int(NT)& r);
  Quotient<NT>& operator-= (const CGAL_int(NT)& r);
  Quotient<NT>& operator*= (const CGAL_int(NT)& r);
  Quotient<NT>& operator/= (const CGAL_int(NT)& r);
  
  Quotient<NT>&    normalize();
  
  const NT&   numerator() const;
  const NT&   denominator() const;

 public:
  NT   num;
  NT   den;
};


template <class NT>
CGAL_KERNEL_MEDIUM_INLINE
Quotient<NT>&
Quotient<NT>::normalize()
{
  if (num == den)
  {
      num = den = NT(1);
      return (*this);
  }
  if (-num == den)
  {
      num = NT(-1);
      den = NT( 1);
      return (*this);
  }
  NT ggt = gcd(num, den);
  if (ggt != NT(1) )
  {
      num /= ggt;
      den /= ggt;
  }
  return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator+= (const Quotient<NT>& r)
{
    num = num * r.den + r.num * den;
    den *= r.den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator-= (const Quotient<NT>& r)
{
    num = num * r.den - r.num * den;
    den *= r.den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator*= (const Quotient<NT>& r)
{
    num *= r.num;
    den *= r.den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator/= (const Quotient<NT>& r)
{
    typedef NT NT;
    CGAL_kernel_precondition( r.num != NT(0) );
    num *= r.den;
    den *= r.num;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator+= (const NT& r)
{
    num = num + r * den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator-= (const NT& r)
{
    num = num - r * den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator*= (const NT& r)
{
    num *= r ;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator/= (const NT& r)
{
    CGAL_kernel_precondition( r != NT(0) );
    den *= r ;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator+= (const CGAL_int(NT)& r)
{
    num = num + r * den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator-= (const CGAL_int(NT)& r)
{
    num = num - r * den;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator*= (const CGAL_int(NT)& r)
{
    num *= r;
    return (*this);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>&
Quotient<NT>::operator/= (const CGAL_int(NT)& r)
{
    CGAL_kernel_precondition( r != NT(0) );
    den *= r;
    return (*this);
}

template <class NT>
CGAL_KERNEL_MEDIUM_INLINE
Comparison_result
quotient_cmp(const Quotient<NT>& x, const Quotient<NT>& y)
{
    // In contrast to LEDA class rational, no assumptions
    // on the sign of  den  are made

    // code assumes that SMALLER == - 1;
    CGAL_kernel_precondition( SMALLER == static_cast<Comparison_result>(-1) );

    int xsign = CGAL_NTS sign(x.num) * CGAL_NTS sign(x.den) ;
    int ysign = CGAL_NTS sign(y.num) * CGAL_NTS sign(y.den) ;
    if (xsign == 0) return static_cast<Comparison_result>(-ysign);
    if (ysign == 0) return static_cast<Comparison_result>(xsign);
    // now (x != 0) && (y != 0)
    int diff = xsign - ysign;
    if (diff == 0)
    {
        int msign = CGAL_NTS sign(x.den) * CGAL_NTS sign(y.den);
        NT leftop  = x.num * y.den * NT(msign);
        NT rightop = y.num * x.den * NT(msign);
        if (leftop < rightop)
        {
            return SMALLER;
        }
        else
        {
            return (rightop < leftop) ? LARGER : EQUAL;
        }
    }
    else
    {
        return (xsign < ysign) ? SMALLER : LARGER;
    }
}

template <class NT>
std::ostream&
operator<<(std::ostream& s, const Quotient<NT>& r)
{
   return s << r.numerator() << "/" << r.denominator();
}

template <class NT>
std::istream&
operator>>(std::istream& in, Quotient<NT>& r)
{
  /* format  num/den  or simply  num  */

  char c = 0;

#ifndef CGAL_CFG_NO_LOCALE
  while (in.get(c) && std::isspace(c, std::locale::classic() ));
#else
  while (in.get(c) && CGAL_CLIB_STD::isspace(c));
#endif // CGAL_CFG_NO_LOCALE
  if ( !in ) return in;
  in.putback(c);

  NT num;
  NT den(1);
  in >> num;

#ifndef CGAL_CFG_NO_LOCALE
  while (in.get(c) && std::isspace(c, std::locale::classic() ));
#else
  while (in.get(c) && CGAL_CLIB_STD::isspace(c));
#endif // CGAL_CFG_NO_LOCALE
  if (( in ) && ( c == '/'))
  {
#ifndef CGAL_CFG_NO_LOCALE
      while (in.get(c) && std::isspace(c, std::locale::classic() ));
#else
      while (in.get(c) && CGAL_CLIB_STD::isspace(c));
#endif // CGAL_CFG_NO_LOCALE
      CGAL_kernel_assertion( in );
      in.putback(c);
      in >> den;
  }
  else
  {
      in.putback(c);
      if ( in.eof() ) in.clear();
  }
  r = Quotient<NT>( num, den);
  return in;
}

template <class NT>
inline
io_Operator
io_tag(const Quotient<NT>&)
{ return io_Operator(); }

template <class NT>
inline
const NT&
Quotient<NT>::numerator() const
{ return num; }

template <class NT>
inline
const NT&
Quotient<NT>::denominator() const
{ return den; }

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator+(const Quotient<NT>& x, const Quotient<NT>& y)
{
  Quotient<NT> z = x;
  return z += y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator-(const Quotient<NT>& x, const Quotient<NT>& y)
{ return (Quotient<NT>(x) -= y); }

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator*(const Quotient<NT>& x, const Quotient<NT>& y)
{
  Quotient<NT> z = x;
  return z *= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator/(const Quotient<NT>& x, const Quotient<NT>& y)
{
  Quotient<NT> z = x;
  return z /= y;
}

template <class NT>
inline
Quotient<NT>
operator-(const Quotient<NT>& x)
{ return Quotient<NT>(-x.num,x.den); }

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator+(const NT& x, const Quotient<NT>& y)
{
  Quotient<NT> z(x);
  return z += y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator+(const Quotient<NT>& x, const NT& y)
{
  Quotient<NT> z = x;
  return z += y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator+(const Quotient<NT>& x, const CGAL_int(NT)& y)
{
  Quotient<NT> z = x;
  return z += NT(y);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator+(const CGAL_int(NT)& x, const Quotient<NT>& y)
{ return y + x; }

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator-(const NT& x, const Quotient<NT>& y)
{
  Quotient<NT> z(x);
  return z -= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator-(const Quotient<NT>& x, const NT& y)
{
  Quotient<NT> z = x;
  return z -= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator-(const Quotient<NT>& x, const CGAL_int(NT)& y)
{
  Quotient<NT> z = x;
  return z -= NT(y);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator-(const CGAL_int(NT)& x, const Quotient<NT>& y)
{
  Quotient<NT> z = x;
  return z -= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator*(const NT& x, const Quotient<NT>& y)
{
  Quotient<NT> z(x);
  return z *= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator*(const Quotient<NT>& x, const NT& y)
{
  Quotient<NT> z = x;
  return z *= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator*(const Quotient<NT>& x, const CGAL_int(NT)& y)
{
  Quotient<NT> z = x;
  return z *= NT(y);
}

template <class NT>
inline
Quotient<NT>
operator*(const CGAL_int(NT)& x, const Quotient<NT>& y)
{ return y*x; }

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator/(const NT& x, const Quotient<NT>& y)
{
  Quotient<NT> z(x) ;
  return z /= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator/(const Quotient<NT>& x, const NT& y)
{
  Quotient<NT> z = x;
  return z /= y;
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator/(const Quotient<NT>& x, const CGAL_int(NT)& y)
{
  Quotient<NT> z = x;
  return z /= NT(y);
}

template <class NT>
CGAL_KERNEL_INLINE
Quotient<NT>
operator/(const CGAL_int(NT)& x, const Quotient<NT>& y)
{
  Quotient<NT> z = x;
  return z /= y;
}

template <class NT>
CGAL_KERNEL_INLINE
NT
quotient_truncation(const Quotient<NT>& r)
{ return (r.num / r.den); }



template <class NT>
CGAL_KERNEL_INLINE
bool
operator==(const Quotient<NT>& x, const Quotient<NT>& y)
{ return x.num * y.den == x.den * y.num; }

template <class NT>
CGAL_KERNEL_INLINE
bool
operator==(const Quotient<NT>& x, const NT& y)
{ return x.den * y == x.num; }

template <class NT>
inline
bool
operator==(const NT& x, const Quotient<NT>& y)
{ return y == x; }

template <class NT>
CGAL_KERNEL_INLINE
bool
operator==(const CGAL_int(NT) & x, const Quotient<NT>& y)
{ return y.den * x == y.num; }

template <class NT>
inline
bool
operator==(const Quotient<NT>& x, const CGAL_int(NT) & y)
{ return y == x; }


template <class NT>
inline
bool
operator!=(const Quotient<NT>& x, const Quotient<NT>& y)
{ return ! (x == y); }

template <class NT>
inline
bool
operator!=(const Quotient<NT>& x, const NT& y)
{ return ! (x == y); }

template <class NT>
inline
bool
operator!=(const NT& x, const Quotient<NT>& y)
{ return ! (x == y); }

template <class NT>
inline
bool
operator!=(const CGAL_int(NT) & x, const Quotient<NT>& y)
{ return ! (x == y); }

template <class NT>
inline
bool
operator!=(const Quotient<NT>& x, const CGAL_int(NT) & y)
{ return ! (x == y); }


template <class NT>
CGAL_KERNEL_INLINE
bool
operator<(const Quotient<NT>& x, const Quotient<NT>& y)
{
  return quotient_cmp(x,y) == SMALLER;
}

template <class NT>
CGAL_KERNEL_INLINE
bool
operator<(const Quotient<NT>& x, const NT& y)
{
  return quotient_cmp(x,Quotient<NT>(y)) == SMALLER;
}

template <class NT>
CGAL_KERNEL_INLINE
bool
operator<(const NT& x, const Quotient<NT>& y)
{
  return quotient_cmp(Quotient<NT>(x),y) == SMALLER;
}

template <class NT>
CGAL_KERNEL_INLINE
bool
operator<(const CGAL_int(NT)& x, const Quotient<NT>& y)
{
  return quotient_cmp(Quotient<NT>(x),y) == SMALLER;
}

template <class NT>
CGAL_KERNEL_INLINE
bool
operator<(const Quotient<NT>& x, const CGAL_int(NT)& y)
{
  return quotient_cmp(x,Quotient<NT>(y)) == SMALLER;
}


template <class NT>
inline
bool
operator>(const Quotient<NT>& x, const Quotient<NT>& y)
{ return y < x; }

template <class NT>
inline
bool
operator>(const Quotient<NT>& x, const NT& y)
{ return y < x; }

template <class NT>
inline
bool
operator>(const NT& x, const Quotient<NT>& y)
{ return y < x; }

template <class NT>
inline
bool
operator>(const Quotient<NT>& x, const CGAL_int(NT)& y)
{ return y < x; }

template <class NT>
inline
bool
operator>(const CGAL_int(NT)& x, const Quotient<NT>& y)
{ return y < x; }



template <class NT>
inline
bool
operator<=(const Quotient<NT>& x, const Quotient<NT>& y)
{ return ! (y < x); }

template <class NT>
inline
bool
operator<=(const Quotient<NT>& x, const NT& y)
{ return ! (y < x); }

template <class NT>
inline
bool
operator<=(const NT& x, const Quotient<NT>& y)
{ return ! (y < x); }

template <class NT>
inline
bool
operator<=(const Quotient<NT>& x, const CGAL_int(NT)& y)
{ return ! (y < x); }

template <class NT>
inline
bool
operator<=(const CGAL_int(NT)& x, const Quotient<NT>& y)
{ return ! (y < x); }



template <class NT>
inline
bool
operator>=(const Quotient<NT>& x, const Quotient<NT>& y)
{ return ! (x < y); }

template <class NT>
inline
bool
operator>=(const Quotient<NT>& x, const NT& y)
{ return ! (x < y); }

template <class NT>
inline
bool
operator>=(const NT& x, const Quotient<NT>& y)
{ return ! (x < y); }

template <class NT>
inline
bool
operator>=(const Quotient<NT>& x, const CGAL_int(NT)& y)
{ return ! (x < y); }

template <class NT>
inline
bool
operator>=(const CGAL_int(NT)& x, const Quotient<NT>& y)
{ return ! (x < y); }


template <class NT>
double
to_double(const Quotient<NT>& q)   /* TODO */
{
  if (q.num == NT(0) )
  { return 0; }

  double nd = CGAL::to_double( q.num );

  if (q.den == NT(1) )
  { return nd; }

  double dd = CGAL::to_double( q.den );

  if ( is_finite( q.den )&&(is_finite( q.num )) )
  { return nd/dd ; }
  if ( CGAL_NTS abs(q.num) > CGAL_NTS abs(q.den) )
  {
      NT  nt_div = q.num / q.den;
      double divd = CGAL::to_double(nt_div);
      if ( divd >= CGAL_CLIB_STD::ldexp(1.0,53) )
      { return divd; }
  }
  if ( CGAL_NTS abs(q.num) < CGAL_NTS abs(q.den) )
  { return 1.0 / CGAL::to_double( NT(1) / q ); }

  return nd/dd ;
}

template <class RT>
std::pair<double,double>
to_interval (const Quotient<RT>& z)
{
    Interval_nt<> quot = Interval_nt<>(CGAL::to_interval(z.numerator())) /
		         Interval_nt<>(CGAL::to_interval(z.denominator()));
    return std::make_pair(quot.inf(), quot.sup());
}

template <class NT>
bool
is_valid(const Quotient<NT>& q)
{ return is_valid(q.num) && is_valid(q.den); }

template <class NT>
bool
is_finite(const Quotient<NT>& q)
{ return is_finite(q.num) && is_finite(q.den); }

template <class NT>
inline
const NT&
denominator(const Quotient<NT>& q)
{ return q.den ; }

template <class NT>
inline
const NT&
numerator(const Quotient<NT>& q)
{ return q.num ; }

/*
template <class NT>
NT
gcd(const NT&, const NT&)
{ return NT(1); }
*/

#undef CGAL_int

CGAL_END_NAMESPACE

#endif  // CGAL_QUOTIENT_H
