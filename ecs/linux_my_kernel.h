#ifndef __CLIB_DATA_STRUCTURE_LINUX_MY_KERNEL_H__
#define __CLIB_DATA_STRUCTURE_LINUX_MY_KERNEL_H__


#ifndef offsetof
    #define offsetof(s,m) (size_t)&(((s *)0)->m)
#endif
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#ifndef container_of
#define container_of(ptr, type, member) ({			\
	(type *)( (char *)ptr - offsetof(type,member) );})
#endif

#endif
