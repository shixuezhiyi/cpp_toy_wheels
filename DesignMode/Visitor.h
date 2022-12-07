template<typename... Types>
class Visitor;

template<typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...>
{
public:
    using Visitor<Types...>::visit;

    virtual void visit(T &) = 0;
};

template<typename T>
class Visitor<T>
{
public:
    virtual void visit(T &) = 0;
};



