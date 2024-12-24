template <typename TType>
Pool<TType>::Pool() = default;

template <typename TType>
Pool<TType>::Pool(const size_t& initial_size) {
    this->resize(initial_size);
}

template <typename TType>
Pool<TType>::~Pool() {
    this->clear();
}

template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored) {
    this->clear();
    for (size_t i = 0; i < numberOfObjectStored; i++) {
        this->_objects.push(new TType());
    }
}

template <typename TType>
void Pool<TType>::clear() {
    auto size = this->_objects.size();
    for (size_t i = 0; i < size; i++) {
        delete this->_objects.top();
        this->_objects.pop();
    }
}

template<typename TType>
template<typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args) {
    TType* obj = this->_objects.top();
    this->_objects.pop();

    new (obj) TType(std::forward<TArgs>(p_args)...);

    return Object(obj, *this);
}

template<typename TType>
void Pool<TType>::release(TType* object) {
    this->_objects.push(object);
}

template<typename TType>
size_t Pool<TType>::size() {
    return this->_objects.size();
}

//**********//
//* Object *//
//**********//

template<typename TType>
Pool<TType>::Object::Object(TType* object, Pool<TType>& pool): object_pointer(object), pool(pool), _constructed(true){
};

template<typename TType>
Pool<TType>::Object::~Object() {
    if (this->object_pointer && !this->_constructed) {
        this->pool.release(this->object_pointer);
    }
};

template<typename TType>
Pool<TType>::Object::Object(Object&& other)
    : object_pointer(other.object_pointer), pool(other.pool), _constructed(other._constructed) {
    other.object_pointer = nullptr;
    other._constructed = false;
    this->_constructed = false;
}

template<typename TType>
typename Pool<TType>::Object& Pool<TType>::Object::operator=(Object&& other) {
    if (this != &other) {
        if (this->object_pointer && this->_constructed) {
            this->pool.release(this->object_pointer);
        }
        this->object_pointer = other.object_pointer;
        this->pool = other.pool;
        this->_constructed = other._constructed;

        other.object_pointer = nullptr;
        other._constructed = false; // Disable the original object
    }
    return *this;
}


template<typename TType>
TType* Pool<TType>::Object::operator->() {
    return this->object_pointer;
}
