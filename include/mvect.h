#pragma once

#include <stddef.h>


/**
** \brief a byte vector, along with macros for using it with any type
*/
typedef struct mvect
{
    // the number of bytes in use
    size_t size;
    // the current maximum size .data can hold
    size_t capacity;
    char *data;
} s_mvect;


#define MVECT_EMPTY                         \
    (s_mvect)                               \
    {                                       \
        .size = 0,                          \
        .capacity = 0,                      \
        .data = NULL,                       \
    }


/**
** \param Vect the vector to fetch the data from
** \param Type the resulting type
** \return the data of Vect as an array of Type
*/
#define MVECT_DATA(Vect, Type) ((Type*)(Vect)->data)


/**
** \brief initialize a vector so that it can initialy hold cap bytes
** \param vect the vector to initialize
** \param cap the initial capacity
*/
void mvect_init(s_mvect *vect, size_t cap);


/**
** \brief initialize a vector so that it can initialy hold Count
**    elements of type Type
** \param Type the type of the vector elements
** \param Count the required capacity
*/
#define MVECT_INIT(Vect, Type, Count) mvect_init(Vect, sizeof(Type) * (Count))


/**
** \brief push size bytes from data to vect
** \param vect the vector to push data to
** \param data the data to pull
*/
void mvect_push(s_mvect *vect, size_t size, const void *data);


/**
** \brief push an element of type Type to Vect
** \param Vect the vector to push an element to
** \param Type the type of the pushed element
** \param Data the element to push
*/
#define MVECT_PUSH(Vect, Type, Data) \
    mvect_push(Vect, sizeof(Type), &(const Type[]){(Data)});


/**
** \brief shrink the vector if meaningful, and return a pointer to the
**    last size bytes of the vector
** \desc poping more bytes than the vector has is undefined behavior
** \param vect the vector to pop data from
** \param size the number of bytes to pop off
** \return a pointer to the just poped data
*/
void *mvect_pop(s_mvect *vect, size_t size);


/**
** \brief pops the last element off a vector
** \desc poping elements off an empty vector, or poping elements with
**   a different type sequence than the reverse pushing type sequence
**   is undefined behavior.
** \param Vect the vector to pop an element off
** \param Type type of the element to pop
** \return a pointer to the just poped element
*/
#define MVECT_POP(Vect, Type) \
    (*(Type*)mvect_pop(Vect, sizeof(Type)));


/**
** \brief returns the size of the vector, assuming it only contains
**   elements of type Type
** \param Vect the vector to consider the size of
** \param Type the type of the vector elements
** \return the number of elements inside the vector
*/
#define MVECT_SIZE(Vect, Type) (((Vect)->size) / sizeof(Type))


/**
** \brief iterate over the elements of a vector
** \param Vect the vector to iterate over
** \param Type the type of the vector elements
** \param IName the name of the iterator variable
** \return the number of elements inside the vector
*/
#define MVECT_FOREACH(Vect, Type, IName)                                \
    for (Type *IName = MVECT_DATA(Vect, Type);                          \
         IName < MVECT_DATA(Vect, Type) + MVECT_SIZE(Vect, Type);       \
         IName++)


/**
** \brief pops raw data off the back of the vector
** \param vect the vector to pop data from
** \param res where to store the poped data
** \param size the count of bytes to pop
*/
void mvect_pop_back(s_mvect *vect, void *res, size_t size);


/**
** \brief pops raw data off the front of the vector
** \param vect the vector to pop data from
** \param res where to store the poped data
** \param size the count of bytes to pop
*/
void mvect_pop_front(s_mvect *vect, void *res, size_t size);
