// tcl_ex.cpp - How to embed TCL library - example

// EXIT_* constants
#include <stdlib.h>
// printf(3)....
#include <stdio.h>

// uname(2)
#include <sys/utsname.h>

// sysinfo(2) - uptime
#include <sys/sysinfo.h>

#include <tcl.h>

// custom command uname -m - returns machine name
static int
UnameMachineCmd(
    ClientData dummy,                /* Not used. */
    Tcl_Interp *interp,                /* Current interpreter. */
    int objc,                        /* Number of arguments. */
    Tcl_Obj *const objv[])        /* Argument objects. */
{
    struct utsname un;

    if (uname(&un)){
        Tcl_SetObjResult(interp, Tcl_ObjPrintf(
                      "error calling uname(): %s",
                      Tcl_PosixError(interp)));
        return TCL_ERROR;
    } else {
        Tcl_SetObjResult(interp, Tcl_NewStringObj(un.machine, -1) );
        return TCL_OK;
    }
}

static int UptimeSecondsCmd(
	    ClientData dummy,                /* Not used. */
	    Tcl_Interp *interp,                /* Current interpreter. */
	    int objc,                        /* Number of arguments. */
	    Tcl_Obj *const objv[])        /* Argument objects. */
{
	struct sysinfo in;

    if (sysinfo(&in)){
        Tcl_SetObjResult(interp, Tcl_ObjPrintf(
                      "error calling sysinfo(): %s",
                      Tcl_PosixError(interp)));
        return TCL_ERROR;
    } else {
        Tcl_SetObjResult(interp, Tcl_NewLongObj(in.uptime) );
        return TCL_OK;
    }
}



// add custom commands to Tcl
static int Ex_ExtendTcl (Tcl_Interp *interp) {
	// WARNING! Tcl_CreateObjCommand() no longer returns error code - but rather
	//          command token.
	// Original example is wrong at: https://wiki.tcl-lang.org/page/How+to+embed+Tcl+in+C+applications
	Tcl_CreateObjCommand(
	        interp, "::ex::uname_machine", UnameMachineCmd, NULL, NULL);
	Tcl_CreateObjCommand(
	        interp, "::ex::uptime_seconds", UptimeSecondsCmd, NULL, NULL);
	return TCL_OK;
}


// runs TCL commands in tclCommands, returns EXIT_SUCESS or EXIT_FAILURE
static int Ex_RunTcl(const char *tclCommands){
	int rc = EXIT_FAILURE;
	int err = TCL_OK;

	Tcl_Interp *interp = Tcl_CreateInterp();
	if (interp == NULL){
		fprintf(stderr,"Tcl_CreateInterp() returned NULL");
		goto exit0;
	}

	err = Ex_ExtendTcl(interp);
	if ( err != TCL_OK ){
		fprintf(stderr,"Error calling Ex_ExtendTcl(): %s\n",Tcl_GetStringResult(interp));
		goto exit2;
	}

	err = Tcl_Eval(interp,tclCommands);
	if ( err != TCL_OK ){
		fprintf(stderr,"Error calling Tcl_Eval(): %s\n",Tcl_GetStringResult(interp));
		goto exit2;
	}

	rc = EXIT_SUCCESS;

	exit2:
	Tcl_FreeResult(interp);

//	exit1:
	Tcl_DeleteInterp(interp);

	exit0:
	return rc;
}


int main(int argc, char **argv)
{
	int rc = EXIT_SUCCESS;

	Tcl_FindExecutable(argv[0]);

	rc = Ex_RunTcl("puts \"Hello, world on [::ex::uname_machine]!\"");

	if (rc == EXIT_SUCCESS){
		rc = Ex_RunTcl("puts \"System uptime is [::ex::uptime_seconds] seconds.\"");
	}

	Tcl_Finalize();
	return rc;
}

