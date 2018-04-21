#pragma once
template <typename ...Types> class xtuple;

// empty tuple (specialization)
template <> class xtuple<> {};

// recursive tuple definition (partial specialization)
template <typename T, typename ... Types>
class xtuple<T, Types...> : private xtuple<Types ...>
{
public:
	xtuple() {}
	xtuple(const T& a, Types ... args) : first_(a), xtuple<Types...>(args...)
	{}

	enum { N = 1 + sizeof...(Types) };

	auto Get() { return first_; }

public:
	T first_;
};
//
//template <size_t N, typename xtuple>
//struct xtuple_element;
//
//// type of the first element
//template <typename T, typename ... Types>
//struct xtuple_element<0, xtuple<T, Types...>>
//{
//	using Type = T;
//	using TupleType = xtuple<T, Types...>;
//};
//
//// recursive tuple_element definition
//template <size_t N, typename T, typename ... Types>
//struct xtuple_element<N, xtuple<T, Types...>>
//	: public xtuple_element<N - 1, xtuple<Types...>>
//{};
//
//template <size_t N, typename ... Types>
//inline typename xtuple_element<N, xtuple<Types...>>::Type& Get(xtuple<Types...>& t)
//{
//	using TupleType = xtuple_element<N, xtuple<Types...>>::TupleType;
//	return ((TupleType&)t).first_;
//}


#define DECLARE_VAR(x) int x;
//#define DECLARE_VAR_(x, ...) int x; DECLARE_VAR_(__VA_ARGS__)

#define DECLARE_TEMPATLE(arg, ...) \
template<class T = std::decay<arg>::type, class... Args>

#define ARRAY_TEMPALTE(...) \
class XTUPLE \
{\
public:\
	DECLARE_VAR(__VA_ARGS__)\
};\

template <class... Args>
class TestClass
{
public:
	Args... arg;
};