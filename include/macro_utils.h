#pragma once


#define ARR_SIZE(Arr) (sizeof(Arr) / sizeof(Arr[0]))
#define OFFSET(Ptr, Val) ((void*)((char*)(Ptr) + (Val)))
