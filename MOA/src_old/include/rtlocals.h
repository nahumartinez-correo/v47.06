// ======================================================================
// |																	|
// |  LOCAL variables are compiled as an ASCII string constant plus		|
// |  a flag to indicate whether or not the LOCAL is an array			|
// |																	|
// |  In compiled form, the first local looks like a FIELD reference:	|
// |																	|
// |		C4FF0001													|
// |																	|
// |  and the second one has an index value of 0002, etc.				|
// |																	|
// ======================================================================

#define MAX_LOCALS	128		// Max LOCALs in a SUBROUTINE

	struct rt_local {
		char	name[20+1];
		short	array;
		};

	XDLL int	local_create_group(int);
	XDLL int	local_allocate(char *, int, int, int, byte *);
	XDLL int	local_deallocate(int);
	XDLL void	local_deallocate_all(void);
	XDLL index	local_index(char *);
	XDLL char	*local_name(int);
	XDLL char	*local_name_by_fp(field *, int);
	XDLL field	*local_get(int);
	XDLL void	local_set_ddh(int);
	XDLL int 	local_dump_locals(int);
