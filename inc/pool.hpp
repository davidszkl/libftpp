# pragma once

# include <iostream>
# include <stack>

template <typename TType>
class Pool {

public:
    Pool();
    Pool(const size_t& initial_size);
    ~Pool();
    void resize(const size_t& numberOfObjectStored);
    void clear();
    void release(TType* object);
    size_t size();

    class Object {
        public:
            Object(TType* object, Pool<TType>& pool);
            Object(Pool<TType>::Object && other);
            ~Object();
            TType* operator -> ();
            Object& operator=(Object&& other);
        
        private:
            TType* object_pointer;
            Pool& pool;
            bool _constructed; // subject requires to return Object from acquire() but this will call the destructor which has to call pool.release(),
                                  //  this flag makes sure release() is not called on the first destructor call which happens when the object returns from acquire()
    };

    template<typename ... TArgs>
    Object acquire(TArgs&&... p_args);

private:
    std::stack<TType*> _objects;
};


# include "pool.tpp"