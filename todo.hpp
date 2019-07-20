// Copyright (c) 2018 Daniel Frey
// Please see LICENSE for license or visit https://github.com/d-frey/todo/

#ifndef TODO_HPP
#define TODO_HPP

#ifdef DISABLE_TODO_CHECK
#define TODO( ... ) static_assert( true, "" )
#else

#include <string_view>
namespace todo
{
   constexpr unsigned check( const unsigned i, const unsigned l )
   {
      return ( i < l ) ? i : throw "out_of_range";
   }

   constexpr unsigned digit( const std::string_view s, const unsigned i )
   {
      if( s[ i ] < '0' || s[ i ] > '9' ) {
         throw "invalid character";
      }
      return s[ i ] - '0';
   }

   constexpr unsigned two( const std::string_view s, const unsigned i )
   {
      return digit( s, i ) * 10 + digit( s, i + 1 );
   }

   constexpr unsigned four( const std::string_view s, const unsigned i )
   {
      return two( s, i ) * 100 + two( s, i + 2 );
   }

   constexpr unsigned year_cpp( const std::string_view s )
   {
      return four( s, 7 );
   }

   constexpr unsigned month_cpp( const std::string_view s )
   {
      // clang-format off
      return ( s == "Jan" ) ? 0
           : ( s == "Feb" ) ? 1
           : ( s == "Mar" ) ? 2
           : ( s == "Apr" ) ? 3
           : ( s == "May" ) ? 4
           : ( s == "Jun" ) ? 5
           : ( s == "Jul" ) ? 6
           : ( s == "Aug" ) ? 7
           : ( s == "Sep" ) ? 8
           : ( s == "Oct" ) ? 9
           : ( s == "Nov" ) ? 10
           : ( s == "Dec" ) ? 11
           : throw "invalid month";
      // clang-format on
   }

   constexpr unsigned day_cpp( const std::string_view s )
   {
      return ( ( s[ 4 ] == ' ' ) ? digit( s, 5 ) : two( s, 4 ) ) - 1;
   }

   constexpr unsigned year_iso( const std::string_view s )
   {
      return four( s, 0 );
   }

   constexpr unsigned month_iso( const std::string_view s )
   {
      return check( two( s, 5 ) - 1, 12 );
   }

   constexpr bool is_leap_year( const unsigned y )
   {
      return ( y % 4 == 0 ) && ( ( y % 400 == 0 ) || ( y % 100 != 0 ) );
   }

   constexpr unsigned days( const unsigned y, const unsigned m )
   {
      if( m == 1 ) {
         return is_leap_year( y ) ? 29 : 28;
      }
      return ( m == 3 || m == 5 || m == 8 || m == 10 ) ? 30 : 31;
   }

   constexpr unsigned day_iso( const std::string_view s )
   {
      return check( two( s, 8 ) - 1, days( year_iso( s ), month_iso( s ) ) );
   }

   constexpr unsigned total_cpp( const std::string_view s )
   {
      if( s.size() != 11 || s[ 3 ] != ' ' || s[ 6 ] != ' ' ) {
         throw "invalid string";
      }
      return ( year_cpp( s ) * 12 + month_cpp( s.substr( 0, 3 ) ) ) * 31 + day_cpp( s );
   }

   constexpr unsigned total_iso( const std::string_view s )
   {
      if( s.size() != 10 || s[ 4 ] != '-' || s[ 7 ] != '-' ) {
         throw "invalid string";
      }
      return ( year_iso( s ) * 12 + month_iso( s ) ) * 31 + day_iso( s );
   }

}  // namespace todo

#define TODO( DATE, MSG ) \
   static_assert( todo::total_cpp( __DATE__ ) < todo::total_iso( DATE ), MSG )

#endif  // DISABLE_TODO_CHECK

#endif  // TODO_HPP
