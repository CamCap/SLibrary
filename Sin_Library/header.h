#pragma once

#include <stdarg.h>
#include <vector>
#include <string>


#define MAX_MATCH_USER 8

#define MAX_MATCH_ROOM 10

////////////////////////////////////////////////////////////

#define LOGIN_SERVER 10
#define LOGIN_SERVER_IP "127.0.0.1"
#define LOGIN_SERVER_PORT 14483

#define INGAME_SERVER 20
#define INGAME_SERVER_IP "127.0.0.1"
#define INGAME_SERVER_PORT 14572

#define MASTER_SERVER_IP "127.0.0.1"
#define MASTER_SERVER_PORT 16523

///////////////////////////////////////////////////////////////////////////

#define PING_CHECK_TIME  10000 //핑 체크 시간

//#define SERVER_PORT 14483
//#define SERVER_IP "127.0.0.1"

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

#define DECLARE_VAR_1(arg) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 1);\

#define DECLARE_VAR_2(arg, ...) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 2);\
EXPAND_ENUM(DECLARE_VAR_1(__VA_ARGS__))

#define DECLARE_VAR_3(arg, ...) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 3);\
EXPAND_ENUM(DECLARE_VAR_2(__VA_ARGS__))

#define DECLARE_VAR_4(arg, ...) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 4);\
EXPAND_ENUM(DECLARE_VAR_3(__VA_ARGS__))

#define DECLARE_VAR_5(arg, ...) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 5);\
EXPAND_ENUM(DECLARE_VAR_4(__VA_ARGS__))

#define DECLARE_VAR_6(arg, ...) \
arg CONCATENATE_ENUM(CONCATENATE_ENUM(m_, arg), 6);\
EXPAND_ENUM(DECLARE_VAR_5(__VA_ARGS__))

#define DECLARE_VAR(arg, ...) \
CONCATENATE_ENUM(DECLARE_VAR_, EXPAND_ENUM(PRN_ENUM_TEXT_NARG(arg, __VA_ARGS__))(arg, __VA_ARGS__) ) 



////////////////////////////////////////////////////////
//#define STRART_TEMPLATE() \
//template<
//#define END_TEMPLATE() \
//>
//
//#define DECLARE_TEMPALTE_1(arg) \
//class T1 = std::decay<arg>::type
//
//#define DECLARE_TEMPALTE_2(arg, ...) \
//class T2 = std::decay<arg>::type, \
//EXPAND_ENUM(DECLARE_TEMPALTE_1(__VA_ARGS__))
//
//#define DECLARE_TEMPALTE_3(arg, ...) \
//class T3 = std::decay<arg>::type, \
//EXPAND_ENUM(DECLARE_TEMPALTE_2(__VA_ARGS__))
//
//#define DECLARE_TEMPALTE_4(arg, ...) \
//class T4 = std::decay<arg>::type, \
//EXPAND_ENUM(DECLARE_TEMPALTE_3(__VA_ARGS__))
//
//#define DECLARE_TEMPALTE_5(arg, ...) \
//class T5 = std::decay<arg>::type, \
//EXPAND_ENUM(DECLARE_TEMPALTE_4(__VA_ARGS__))
//
//#define DECLARE_TEMPALTE_6(arg, ...) \
//class T6 = std::decay<arg>::type, \
//EXPAND_ENUM(DECLARE_TEMPALTE_5(__VA_ARGS__))
//
//#define DECLARE_TEMPATLE(arg, ...) \
//CONCATENATE_ENUM(DECLARE_TEMPALTE_ ,EXPAND_ENUM(PRN_ENUM_TEXT_NARG(arg, __VA_ARGS__))(arg, __VA_ARGS__) ) 
//
//
/////////////////////////////////////
//
//#define DECLARE_CLASS(arg, ...) \
//STRART_TEMPLATE\
//END_TEMPLATE\
//class SDeclare_class \
//{\
//public:\
//DECLARE_VAR(arg, __VA_ARGS__) \
//};\
//

////////////////////////////////////////////////////

#define STRART_TEMPLATE \
template<
#define END_TEMPLATE \
,class... Args>

template <typename... Args>
class TemplateClass {};

template <>
class TemplateClass<> {};

//
//template <class T1, class... Args>
//class TemplateClass<T1, Args...> : public TemplateClass<Args...>
//{
//	T1 m_t;
//};


#define START_TEMPLATE_CLASS(num) class TemplateClass< \
EXPAND_ENUM( CONCATENATE_ENUM( DECLARE_TEMPLATE_, num) ) , Args...> \
 : private TemplateClass<Args...> { public: \

#define END_TEMPLATE_CLASS };

#define DECLARE_TEMPLATE_1 typename T1
#define DECLARE_TEMPLATE_2 typename T2, DECLARE_TEMPLATE_1
#define DECLARE_TEMPLATE_3 typename T3, DECLARE_TEMPLATE_2
#define DECLARE_TEMPLATE_4 typename T4, DECLARE_TEMPLATE_3
#define DECLARE_TEMPLATE_5 typename T5, DECLARE_TEMPLATE_4
#define DECLARE_TEMPLATE_6 typename T6, DECLARE_TEMPLATE_5
#define DECLARE_TEMPLATE_7 typename T7, DECLARE_TEMPLATE_6
#define DECLARE_TEMPLATE_8 typename T8, DECLARE_TEMPLATE_7
#define DECLARE_TEMPLATE_9 typename T9, DECLARE_TEMPLATE_8

#define DECLARE_TEMPLATE(N) \
STRART_TEMPLATE \
CONCATENATE_ENUM( DECLARE_TEMPLATE_, N) \
END_TEMPLATE

#define DECLATE_TEMPLATE_VAR_1 T1 m_1; 
#define DECLATE_TEMPLATE_VAR_2 T2 m_2; DECLATE_TEMPLATE_VAR_1
#define DECLATE_TEMPLATE_VAR_3 T3 m_3; DECLATE_TEMPLATE_VAR_2
#define DECLATE_TEMPLATE_VAR_4 T4 m_4; DECLATE_TEMPLATE_VAR_3
#define DECLATE_TEMPLATE_VAR_5 T5 m_5; DECLATE_TEMPLATE_VAR_4
#define DECLATE_TEMPLATE_VAR_6 T6 m_6; DECLATE_TEMPLATE_VAR_5
#define DECLATE_TEMPLATE_VAR_7 T7 m_7; DECLATE_TEMPLATE_VAR_6
#define DECLATE_TEMPLATE_VAR_8 T8 m_8; DECLATE_TEMPLATE_VAR_7
#define DECLATE_TEMPLATE_VAR_9 T9 m_9; DECLATE_TEMPLATE_VAR_8

#define INIT_CLASS_1 TemplateClass(T1 t1, Args... args) : m_1(t1), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_2 TemplateClass(T1 t1, T2 t2, Args... args) : m_1(t1), m_2(t2), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_3 TemplateClass(T1 t1, T2 t2, T3 t3, Args... args) : m_1(t1),  m_2(t2), m_3(t3), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_4 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, Args... args) : m_1(t1),  m_2(t2), m_3(t3), m_4(t4), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_5 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, Args... args) : m_1(t1),  m_2(t2), m_3(t3), m_4(t4), m_5(t5), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_6 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, Args... args) : m_1(t1),  m_2(t2), m_3(t3), m_4(t4), m_5(t5), m_6(t6), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_7 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, Args... args) : m_1(t1),  m_2(t2), m_3(t3), m_4(t4), m_5(t5), m_6(t6), m_7(t7), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_8 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, Args... args) : m_1(t1),  m_2(t2), m_3(t3), m_4(t4), m_5(t5), m_6(t6), m_7(t7), m_8(t8), TemplateClass<Args...>(args...) {}
#define INIT_CLASS_9 TemplateClass(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, Args... args) :  m_1(t1),  m_2(t2), m_3(t3), m_4(t4), m_5(t5), m_6(t6), m_7(t7), m_8(t8), m_9(t9), TemplateClass<Args...>(args...) {}

#define DECLARE_TEMPALTE_CLASS_(num) \
DECLARE_TEMPLATE(num) \
START_TEMPLATE_CLASS(num) \
CONCATENATE_ENUM(DECLATE_TEMPLATE_VAR_,num) \
CONCATENATE_ENUM(INIT_CLASS_, num)\
END_TEMPLATE_CLASS

#define DECLARE_TEMPLATE_CLASS(num) DECLARE_TEMPALTE_CLASS_(num)

DECLARE_TEMPLATE_CLASS(1)
DECLARE_TEMPLATE_CLASS(2)
DECLARE_TEMPLATE_CLASS(3)
DECLARE_TEMPLATE_CLASS(4)
DECLARE_TEMPLATE_CLASS(5)
DECLARE_TEMPLATE_CLASS(6)
DECLARE_TEMPLATE_CLASS(7)
DECLARE_TEMPLATE_CLASS(8)
DECLARE_TEMPLATE_CLASS(9)

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#define DECLARE_CLASS(type, ...) \
DECLARE_ENUM(type, __VA_ARGS__); \
typedef std::vector<type> VEC_##type; \
class Enum##type##Type \
{\
public:\
	Enum##type##Type(int number) {\
		type cur_type = (type)0; \
		int size = sizeof(type);\
		for (; cur_type < size ;) \
		{\
			m_vecEnum.push_back(cur_type); \
			cur_type = (type)(cur_type+1);\
		}\
		m_type = number;\
	}\
	~Enum##type##Type() {}\
	type GetType() {return m_vecEnum[m_type];}\
\
private:\
	Enum##type##Type() {} \
	VEC_##type m_vecEnum;\
	int m_type;\
};\
private:\
	Enum##type##Type m_enum##type;
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------- 게임 메세지 ------------------//
#define GM_ERROR                WM_USER + 1 //에러 메세지를 출력.
#define GM_ACCEPTUPEER           WM_USER + 2 //새로운 유저가 접속했을 경우.
#define GM_DISCONNECTUSER       WM_USER + 3 //유저가 접속했을 종료했을 경우.
#define GM_PKTRECEIVE           WM_USER + 4 //클라이언트로 부터 패킷이 왔을 경우.
#define GM_QUIT					WM_USER + 5 //클라이언트로 부터 패킷이 왔을 경우.
#define GM_TIMER				WM_USER + 6

