#pragma once

//////////////////////////////////////////////////////////////////

#define SOCKET_INVALID_CHECK(SOCK) if(SOCK == INVALID_SOCKET) return FALSE;
typedef void(*SOCKET_CALLBACK)(void);


///////////////////////////////////////////////////////////////////

#define SAFE_DELETE(p) { if((p)) delete (p); (p) = NULL; }
#define SAFE_DELETE_ARR(p) { if((p)) delete[] (p); (p) = NULL; }

///////////////////////////////////////////////////////////////////

#define _STRING_CAT(a,b) a ## b
#define STRING_CAT(a,b) _STRING_CAT(a,b)

template<typename T>
struct ScopeExit
{
	ScopeExit(T t) : _t(t) {}
	~ScopeExit() { _t(); }

	T _t;
};

template<typename T>
ScopeExit<T> MakeScopeExit(T t)
{
	return ScopeExit<T>(t);
}

#define SCOPE_EXIT(expression) auto STRING_CAT(socpe_exit, __LINE__) = MakeScopeExit( [=](){expression} );

///////////////////////////////////////////////////////////////////

#define PROPERTY(_get, _set) _declspec(property(get = _get, put = _set))
#define G_PROPERTY(_get) _declspec(property(get = _get))
#define S_PROPERTY(_set) _declspec(property(set = _set))

/////////////////////////////////////////////////////////////////////////////////////////////////////

//Enum to String
#define EXPAND_ENUM( x )     x

#define STRINGIZE_ENUM(arg)  STRINGIZE_ENUM1(arg)
#define STRINGIZE_ENUM1(arg) STRINGIZE_ENUM2(arg)
#define STRINGIZE_ENUM2(arg) #arg

#define CONCATENATE_ENUM(arg1, arg2)   CONCATENATE_ENUM1(arg1, arg2)
#define CONCATENATE_ENUM1(arg1, arg2)  CONCATENATE_ENUM2(arg1, arg2)
#define CONCATENATE_ENUM2(arg1, arg2)  arg1##arg2

#define BEGIN_PACKET_ENUM(type) \
    static const char*  type##ToString( int iEnum_ ) \
    {

#define END_PACKET_ENUM() ; return ""; }

#define PRN_ENUM_TEXT_1( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field);
#define PRN_ENUM_TEXT_2( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_1( __VA_ARGS__) )
#define PRN_ENUM_TEXT_3( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_2( __VA_ARGS__) )
#define PRN_ENUM_TEXT_4( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_3( __VA_ARGS__) )
#define PRN_ENUM_TEXT_5( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_4( __VA_ARGS__) )
#define PRN_ENUM_TEXT_6( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_5( __VA_ARGS__) )
#define PRN_ENUM_TEXT_7( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_6( __VA_ARGS__) )
#define PRN_ENUM_TEXT_8( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_7( __VA_ARGS__) )
#define PRN_ENUM_TEXT_9( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_8( __VA_ARGS__) )
#define PRN_ENUM_TEXT_10( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_9( __VA_ARGS__) )
#define PRN_ENUM_TEXT_11( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_10( __VA_ARGS__) )
#define PRN_ENUM_TEXT_12( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_11( __VA_ARGS__) )
#define PRN_ENUM_TEXT_13( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_12( __VA_ARGS__) )
#define PRN_ENUM_TEXT_14( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_13( __VA_ARGS__) )
#define PRN_ENUM_TEXT_15( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_14( __VA_ARGS__) )
#define PRN_ENUM_TEXT_16( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_15( __VA_ARGS__) )
#define PRN_ENUM_TEXT_17( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_16( __VA_ARGS__) )
#define PRN_ENUM_TEXT_18( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_17( __VA_ARGS__) )
#define PRN_ENUM_TEXT_19( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_18( __VA_ARGS__) )
#define PRN_ENUM_TEXT_20( field, ... ) \
    if( iEnum_ == field ) return STRINGIZE_ENUM(field); \
    EXPAND_ENUM( PRN_ENUM_TEXT_19( __VA_ARGS__) )

#define PRN_ENUM_TEXT_NARG(...) EXPAND_ENUM( PRN_ENUM_TEXT_NARG_(__VA_ARGS__, PRN_ENUM_TEXT_RSEQ_N()) )
#define PRN_ENUM_TEXT_NARG_(...) EXPAND_ENUM( PRN_ENUM_TEXT_ARG_N(__VA_ARGS__) )
#define PRN_ENUM_TEXT_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N
#define PRN_ENUM_TEXT_RSEQ_N() 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define PRN_ENUM_TEXT_( N, ... ) CONCATENATE_ENUM(PRN_ENUM_TEXT_, N)( __VA_ARGS__)

#define PRN_ENUM_TEXT( ... ) EXPAND_ENUM( PRN_ENUM_TEXT_( EXPAND_ENUM( PRN_ENUM_TEXT_NARG( __VA_ARGS__) ), __VA_ARGS__ ) )

#define IMPLEMENT_ENUM( type, ... ) \
    BEGIN_PACKET_ENUM(type) \
        EXPAND_ENUM( PRN_ENUM_TEXT( __VA_ARGS__ ) ) \
    END_PACKET_ENUM()

#define DECLARE_ENUM( type, ... ) \
    enum type\
    { \
    __VA_ARGS__ \
    }; \
    EXPAND_ENUM( IMPLEMENT_ENUM(type, __VA_ARGS__) );

//////////////////////////////////////////////////////////////////////////////////////////////////////////



