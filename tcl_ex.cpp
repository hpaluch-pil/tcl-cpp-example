// tcl_ex.c - How to embedd TCL library - example

// EXIT_* constants
#include <stdlib.h>
// printf(3)....
#include <stdio.h>

#include <tcl.h>

// runs TCL commands in tclCommands, returns EXIT_SUCESS or EXIT_FAILURE
static int ExRunTcl(const char *tclCommands){
	int rc = EXIT_FAILURE;
	int err = TCL_OK;

	Tcl_Interp *interp = Tcl_CreateInterp();
	if (interp == NULL){
		fprintf(stderr,"Tcl_CreateInterp() returned NULL");
		goto exit0;
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

	rc = ExRunTcl("puts \"Hello, world!\"");

	if (rc == EXIT_SUCCESS){
		rc = ExRunTcl("puts \"Hello, world2!\"");
	}

	Tcl_Finalize();
	return rc;
}

