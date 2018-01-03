#pragma once

#include <stddef.h>


/**
** \page mlist Mdbg lists
**
**  This is a fairly standard type of intrusive list, which takes advantage of
** the preprocessor to factor field accesses.
**
**  It can be used as follows:
**
** ```c
** struct list_element
** {
**     s_mlist_e annoying_list;
** };
**
** #define SMLIST(F) F(struct list_element, annoying_list)
**
**
** int main(void)
** {
**     s_mlist l;
**     mlist_init(&l);
**     struct list_element a, b;
**
**     MLIST_ADD(SMLIST, &l, &a);
**     MLIST_ADD(SMLIST, &l, &b);
**     MLIST_FOREACH(i, SMLIST, &l)
**         if (i == &a)
**             puts("A");
**         else if (i == &b)
**             puts("B");
** }
** ```
*/


/**
** \brief a list element
*/
typedef struct mlist_e
{
    // pointer to the self-reference from the previous element
    struct mlist_e **prev;
    // either the next element or NULL
    struct mlist_e *next;
} s_mlist_e;


/**
** \brief a list container
*/
typedef struct mlist
{
    size_t size;
    s_mlist_e *head;
} s_mlist;


#define MLIST_EMPTY (s_mlist){ .size = 0, .head = NULL }


/**
** \brief returns the mother structure of one of its fields
** \param Type the type of the structure to get back to
** \param Field the field Inst points to
** \param Inst a pointer to the field of the mother structure
** \return a pointer to the mother structure
*/
#define CONTAINEROF(Type, Field, Inst) \
    (Type*)((char*)(Inst) - offsetof(Type, Field))


// #define EXAMPLE_LIST(F) F(mytype_t, myfield_t)


/**
** \brief returns the type from a metadata tuple
** \param Type the type of list elements
** \param Field the field the list uses
** \return the list element type
*/
#define MLIST_TYPE(Type, Field) Type


/**
** \brief returns the field from a metadata tuple
** \param Type the type of list elements
** \param Field the field the list uses
** \return the list element field
*/
#define MLIST_FIELD(Type, Field) Field


/**
** \param LType the metadata tuple
** \param Elem a pointer to a list element
** \return the field of Elem according to LType, of type s_mlist_e
*/
#define MLIST_GETFIELD(LType, Elem) \
    &(Elem)->LType(MLIST_FIELD)


/**
** \brief initializes a list
** \param list the list to initialize
*/
void mlist_init(s_mlist *list);


/**
** \brief adds an element to a list, at head position
** \param list the list to add the element to
** \param elem the element to add
*/
void mlist_add(s_mlist *list, s_mlist_e *elem);
#define MLIST_ADD(LType, List, Elem)            \
    mlist_add(List, MLIST_GETFIELD(LType, Elem))


/**
** \brief removes an element from a list
** \param list the list to remove the element from
** \param elem the element to remove
*/
void mlist_remove(s_mlist *list, s_mlist_e *elem);
#define MLIST_REMOVE(LType, List, Elem)         \
    mlist_remove(List, MLIST_GETFIELD(LType, Elem))


/**
** \brief inserts an element in a list
** \param list the list to insert the element into
** \param elem the element to insert
** \param ip a pointer to the insertion point
*/
void mlist_insert(s_mlist *list, s_mlist_e *elem, s_mlist_e **ip);
#define MLIST_INSERT(LType, List, Elem, Ip)             \
    mlist_insert(List, MLIST_GETFIELD(LType, Elem),     \
                 &MLIST_GETFIELD(LType, Ip)->next)

// Elem is the list element
#define MLIST_NEXT_SUB(Type, Field, Elem)                       \
    ((Elem)->next                                               \
     ? CONTAINEROF(Type, Field, (Elem)->next)                   \
     : NULL)


/**
** \brief returns the next element in a list
** \param LType the metadata tuple
** \param Elem the current element
** \return the next element if any, which has the same
**   type as Elem, or NULL
*/
#define MLIST_NEXT(LType, Elem)                                 \
    MLIST_NEXT_SUB(LType(MLIST_TYPE), LType(MLIST_FIELD),       \
                   MLIST_GETFIELD(LType, Elem))

#define MLIST_HEAD_SUB(Type, Field, List)                       \
    ((List)->head                                               \
     ? CONTAINEROF(Type, Field, (List)->head)                   \
     : NULL)


/**
** \brief returns the head element of a list
** \param LType the metadata tuple
** \param List the list container
** \return the head element if any, or NULL
*/
#define MLIST_HEAD(LType, List)                                 \
    MLIST_HEAD_SUB(LType(MLIST_TYPE), LType(MLIST_FIELD), List)


/**
** \brief iterates over the elements of a list
** \param IName the name of the iterator variable
** \param LType the metadata tuple
** \param List the list to iterate over
*/
#define MLIST_FOREACH(IName, LType, List)                       \
    for (LType(MLIST_TYPE) *IName = MLIST_HEAD(LType, List);    \
         IName;                                                 \
         IName = MLIST_NEXT(LType, IName))
