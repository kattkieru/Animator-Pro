	include aasyslib.i

CGROUP	group	code

code	segment dword 'CODE'
	assume cs:CGROUP,ds:CGROUP

SYSLIB_JUMP strlen sysl_strlen

code	ends
	end
