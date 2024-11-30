#ifndef H_TEST_MOCKS
#define H_TEST_MOCKS

#include <ctf/ctf.h>

CTF_MOCK_EXTERN(void *, malloc, (size_t size))
CTF_MOCK_VOID_RET_EXTERN(free, (void *ptr))
CTF_MOCK_EXTERN(void *, calloc, (size_t nmemb, size_t size))
CTF_MOCK_EXTERN(void *, realloc, (void *ptr, size_t size))
CTF_MOCK_EXTERN(void *, reallocarray, (void *ptr, size_t nmemb, size_t size))

#endif
