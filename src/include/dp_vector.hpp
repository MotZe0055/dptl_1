//
// dp_vector.hpp - sample customization of std::vector for deep_ptr
//
//      2019/01/07, version 1.0.0
//
//      © Kazunari Saitoh, 2018-2019.
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DPTL_DP_VECTOR_HPP
#define __DPTL_DP_VECTOR_HPP

#include <vector>
#include <dptl/deep_ptr.hpp>
		
#ifndef DPTL_NAMESPACE
#define DPTL_NAMESPACE dptl
#endif  
		
#ifndef NO_NAMESPACE
namespace DPTL_NAMESPACE {
#endif

// policy based pointer set
// -----------------------------------------------------------------------------
template <typename T>
class __dp_vector : public std::vector<T>
{
	typedef dp_type<T>				VT;
	typedef dp_pointer<T>				VP;
	typedef std::vector<T>				super;

    public:
	typedef typename super::value_type		value_type;
	typedef typename super::allocator_type		allocator_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference		const_reference;
	typedef typename super::pointer			pointer;
	typedef	typename super::const_pointer		const_pointer;
	typedef	typename super::iterator		iterator;
	typedef	typename super::const_iterator		const_iterator;
	typedef	typename super::reverse_iterator	reverse_iterator;
	typedef	typename super::const_reverse_iterator	const_reverse_iterator;
	typedef	typename super::difference_type		difference_type;
	typedef	typename super::size_type		size_type;

	typedef VP					value_pointer;

#if defined(__cplusplus) && (__cplusplus >= 201402)
	__dp_vector() = default;
	explicit __dp_vector( const allocator_type& alloc ) : super( alloc ) { }
	explicit __dp_vector( size_type n, const allocator_type& alloc = allocator_type()) : super( n, alloc ) { }
	__dp_vector( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_vector( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_vector( const __dp_vector& ) = default;
	__dp_vector( const __dp_vector& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_vector( __dp_vector&& ) = default;
	__dp_vector( __dp_vector&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	__dp_vector( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}

#elif defined(__cplusplus) && (__cplusplus >= 201103)
	// ---- for c++11
	explicit __dp_vector( const allocator_type& alloc = allocator_type()) : super( alloc ) { }
	explicit __dp_vector( size_type n ) : super( n ) { }
	__dp_vector( size_type n, const value_pointer& v, const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		assign( n, v );
	}

	template <class II>
	__dp_vector( II first, II last, const allocator_type& alloc = allocator_type()) : super( first, last, alloc ) { }

	__dp_vector( const __dp_vector& ) = default;
	__dp_vector( const __dp_vector& l, const allocator_type& alloc ) : super( l, alloc ) { }
	__dp_vector( __dp_vector&& ) = default;
	__dp_vector( __dp_vector&& l, const allocator_type& alloc ) : super( std::move( l ), alloc ) { }

	__dp_vector( std::initializer_list<value_pointer> il,
		 const allocator_type& alloc = allocator_type()) : super( alloc )
	{
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}
#else
#error "c++11 up required"
#endif

	__dp_vector& operator=( const __dp_vector& ) = default;
	__dp_vector& operator=( __dp_vector&& ) = default;
	__dp_vector& operator=( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
		return *this;
	}

	~__dp_vector() = default;

	// wrapper for appropriate casting of raw pointer
	template <class II> void assign( II fst, II lst )
	{
		super::assign( fst, lst );
	}
	void assign( size_type n, const value_pointer& v )
	{
		super::assign( n, reinterpret_cast<const T&>( v ));
	}
	void assign( std::initializer_list<value_pointer> il )
	{
		super::clear();
		for ( auto&& p : il ) super::push_back( std::forward<decltype( p )>( p ));
	}

	template <typename... Args> iterator emplace( const_iterator pos, Args&&... args )
	{
		return super::emplace( pos, std::forward<Args>( args )... );
	}
	template <typename... Args> void emplace_back( Args&&... args )
	{
		super::emplace_back( std::forward<Args>( args )... );
	}

	iterator insert( const_iterator i, const value_type& p )
	{
		return super::insert( i, p );
	}
	iterator insert( const_iterator i, value_type&& p )
	{
		return super::insert( i, std::move( p ));
	}
	iterator insert( const_iterator i, size_type n, const value_pointer& v )
	{
		return super::insert( i, n, reinterpret_cast<const T&>( v ));
	}
	template <class II> iterator insert( const_iterator i, II fst, II lst )
	{
		return super::insert( i, fst, lst );
	}
        iterator insert( const_iterator i, std::initializer_list<value_pointer> il )
        {
		iterator		r = super::insert( i, il.size(), nullptr );
		iterator		j = r;

		for ( auto p = il.begin(); p != il.end(); ++p, ++j ) *j = *p;
		return r; 
        }

	// wrapper for appropriate casting of raw pointer avoiding unnecessary duplication
	void resize( size_type n ) { super::resize( n ); }
	void resize( size_type n, const value_pointer& v )
	{
		super::resize( n, reinterpret_cast<const T&>( v ));
	}
};
		
// alias for deep pointer vector: T=V*/V[]/V[N]
//
//	dp_vector: vector for deep pointer
//	dr_vector: vector for deep reference pointer
// -----------------------------------------------------------------------------
template <typename T> using dp_vector = __dp_vector<deep_ptr<T>>;
template <typename T> using dr_vector = __dp_vector<dref_ptr<T>>;


#ifndef NO_NAMESPACE
}       
#endif
	
#endif // __DPTL_DP_VECTOR_HPP

