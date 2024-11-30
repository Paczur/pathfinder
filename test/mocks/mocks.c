#include "mocks.h"

CTF_MOCK(void *, malloc, (size_t size), (size))
CTF_MOCK_VOID_RET(free, (void *ptr), (ptr))
CTF_MOCK(void *, calloc, (size_t nmemb, size_t size), (nmemb, size))
CTF_MOCK(void *, realloc, (void *ptr, size_t size), (ptr, size))
CTF_MOCK(void *, reallocarray, (void *ptr, size_t nmemb, size_t size),
         (ptr, nmemb, size))
