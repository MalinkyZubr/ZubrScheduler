#include "memory.hpp"


template<typename Wrapped>
ObjectWrapper<Wrapped>::ObjectWrapper(MemorySafeObject<Wrapped> *object, MemoryLocation<Wrapped> *location) : associated_object(object), memory_location(location) {
    location->push_member(this);
}

template<typename Wrapped>
MemorySafeObject<Wrapped>* ObjectWrapper<Wrapped>::get_associated_object() {
    return this->associated_object;
}

template<typename Wrapped>
ObjectWrapper<Wrapped>* ObjectWrapper<Wrapped>::get_next_internal_wrapper(MemorySafeObject<Wrapped> *requester) {
    if(requester == this->associated_object) {
        return this->next_internal_wrapper;
    }
    return nullptr;
}

template<typename Wrapped>
void ObjectWrapper<Wrapped>::set_next_internal_wrapper(MemorySafeObject<Wrapped> *requester, ObjectWrapper<Wrapped> *next) {
    if(requester == this->associated_object) {
        this->next_internal_wrapper = next;
    }
}

template<typename Wrapped>
ObjectWrapper<Wrapped>* ObjectWrapper<Wrapped>::next() {
    return this->next_wrapper;
}

template<typename Wrapped>
void ObjectWrapper<Wrapped>::set_next(ObjectWrapper<Wrapped> *next) {
    this->next_wrapper = next;
}

template<typename Wrapped>
ObjectWrapper<Wrapped>* ObjectWrapper<Wrapped>::previous() {
    return this->previous_wrapper;
}

template<typename Wrapped>
void ObjectWrapper<Wrapped>::set_previous(ObjectWrapper<Wrapped> *previous) {
    this->previous_wrapper = previous;
}

template<typename Wrapped>
MemorySafeObject<Wrapped>* ObjectWrapper<Wrapped>::get_memory_safe() {
    return this->associated_object;
}

template<typename Wrapped>
Wrapped* ObjectWrapper<Wrapped>::get_wrapped() {
    return this->associated_object->get_wrapped_object();
}

template<typename Wrapped>
void ObjectWrapper<Wrapped>::location_transmit_object_receive_delete() {
    this->associated_object->receive_wrapper_delete_request(this);
}

template<typename Wrapped>
void ObjectWrapper<Wrapped>::object_transmit_location_receive_delete() {
    this->memory_location->receive_wrapper_delete_request(this);
}

