// tcl_ex.c - How to embedd TCL library - example

// EXIT_* constants
#include <stdlib.h>
// printf(3)....
#include <stdio.h>

// uname(2)
#include <sys/utsname.h>

#include <tcl.h>

// custom command uname -m - returns machine name
static int
UnameMachineCmd(
    ClientData dummy,                /* Not used. */
    Tcl_Interp *interp,                /* Current interpreter. */
    int objc,                        /* Number of arguments. */
    Tcl_Obj *const objv[])        /* Argument objects. */
{
    Tcl_Obj *objResultPtr;

    struct utsname un;

    if (uname(&un)){
    	// TODO: strerror
        objResultPtr = Tcl_NewStringObj("ERROR calling uname(2)", -1);
    } else {
        objResultPtr = Tcl_NewStringObj(un.machine, -1);
    }

    Tcl_SetObjResult(interp, objResultPtr);
    return TCL_OK;
}



// add custom commands to Tcl
static int Ex_ExtendTcl (Tcl_Interp *interp) {
	// WARNING! Tcl_CreateObjCommand() no longer returns error code - but rather
	//          commadn token.
	// Original example is wrong at: https://wiki.tcl-lang.org/page/How+to+embed+Tcl+in+C+applications
	Tcl_CreateObjCommand(
	        interp, "::ex::uname_machine", UnameMachineCmd, NULL, NULL);
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
		rc = Ex_RunTcl("puts \"Hello, world2!\"");
	}

	Tcl_Finalize();
	return rc;
}

