// Copyright (c) 2018 Daniel Frey
// Please see LICENSE for license or visit https://github.com/d-frey/todo/

#ifndef TODO_HPP
#define TODO_HPP

#ifdef DISABLE_TODO_CHECK
#define TODO( ... ) static_assert( true, "" )
#else

namespace todo
{
   class ct_string
   {
      const char* const begin_;
      const unsigned size_;

   public:
      template< unsigned N >
      constexpr ct_string( const char ( &arr )[ N ] )
         : begin_( arr ), size_( N - 1 )
      {
      }

      constexpr unsigned size() const
      {
         return size_;
      }

      constexpr char operator[]( const unsigned i ) const
      {
         return begin_[ i ];
      }
   };

   constexpr unsigned check( const unsigned i, const unsigned l )
   {
      return ( i < l ) ? i : throw "out_of_range";
   }

   constexpr unsigned digit( const ct_string s, const unsigned i )
   {
      return ( s[ i ] >= '0' && s[ i ] <= '9' ) ? ( s[ i ] - '0' ) : throw "invalid character";
   }

   constexpr unsigned two( const ct_string s, const unsigned i )
   {
      return digit( s, i ) * 10 + digit( s, i + 1 );
   }

   constexpr unsigned four( const ct_string s, const unsigned i )
   {
      return two( s, i ) * 100 + two( s, i + 2 );
   }

   constexpr unsigned year_cpp( const ct_string s )
   {
      return four( s, 7 );
   }

   constexpr unsigned month_cpp( const ct_string s )
   {
      // clang-format off
      return ( s[ 0 ] == 'J' && s[ 1 ] == 'a' && s[ 2 ] == 'n' ) ? 0
           : ( s[ 0 ] == 'F' && s[ 1 ] == 'e' && s[ 2 ] == 'b' ) ? 1
           : ( s[ 0 ] == 'M' && s[ 1 ] == 'a' && s[ 2 ] == 'r' ) ? 2
           : ( s[ 0 ] == 'A' && s[ 1 ] == 'p' && s[ 2 ] == 'r' ) ? 3
           : ( s[ 0 ] == 'M' && s[ 1 ] == 'a' && s[ 2 ] == 'y' ) ? 4
           : ( s[ 0 ] == 'J' && s[ 1 ] == 'u' && s[ 2 ] == 'n' ) ? 5
           : ( s[ 0 ] == 'J' && s[ 1 ] == 'u' && s[ 2 ] == 'l' ) ? 6
           : ( s[ 0 ] == 'A' && s[ 1 ] == 'u' && s[ 2 ] == 'g' ) ? 7
           : ( s[ 0 ] == 'S' && s[ 1 ] == 'e' && s[ 2 ] == 'p' ) ? 8
           : ( s[ 0 ] == 'O' && s[ 1 ] == 'c' && s[ 2 ] == 't' ) ? 9
           : ( s[ 0 ] == 'N' && s[ 1 ] == 'o' && s[ 2 ] == 'v' ) ? 10
           : ( s[ 0 ] == 'D' && s[ 1 ] == 'e' && s[ 2 ] == 'c' ) ? 11
           : throw "invalid month";
      // clang-format on
   }

   constexpr unsigned day_cpp( const ct_string s )
   {
      return ( ( s[ 4 ] == ' ' ) ? digit( s, 5 ) : two( s, 4 ) ) - 1;
   }

   constexpr unsigned year_iso( const ct_string s )
   {
      return four( s, 0 );
   }

   constexpr unsigned month_iso( const ct_string s )
   {
      return check( two( s, 5 ) - 1, 12 );
   }

   constexpr bool is_leap_year( const unsigned y )
   {
      return ( y % 4 == 0 ) && ( ( y % 400 == 0 ) || ( y % 100 != 0 ) );
   }

   constexpr unsigned days( const unsigned y, const unsigned m )
   {
      return ( m == 1 ) ? ( is_leap_year( y ) ? 29 : 28 ) : ( ( m == 3 || m == 5 || m == 8 || m == 10 ) ? 30 : 31 );
   }

   constexpr unsigned day_iso( const ct_string s )
   {
      return check( two( s, 8 ) - 1, days( year_iso( s ), month_iso( s ) ) );
   }

   constexpr unsigned total_cpp( const ct_string s )
   {
      return ( s.size() == 11 && s[ 3 ] == ' ' && s[ 6 ] == ' ' ) ? year_cpp( s ) * 366 + month_cpp( s ) * 31 + day_cpp( s ) : throw "invalid string";
   }

   constexpr unsigned total_iso( const ct_string s )
   {
      return ( s.size() == 10 && s[ 4 ] == '-' && s[ 7 ] == '-' ) ? year_iso( s ) * 366 + month_iso( s ) * 31 + day_iso( s ) : throw "invalid string";
   }

}  // namespace todo

#define TODO( DATE, MSG ) \
   static_assert( todo::total_cpp( __DATE__ ) < todo::total_iso( DATE ), MSG )

#endif  // DISABLE_TODO_CHECK

#endif  // TODO_HPP
