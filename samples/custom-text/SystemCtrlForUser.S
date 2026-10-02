	.set noreorder

#include "pspimport.s"

    IMPORT_START "SystemCtrlForUser",0x40090000
    IMPORT_FUNC  "SystemCtrlForUser",0x2A868045,sctrlModuleTextAddr
    IMPORT_FUNC  "SystemCtrlForUser",0x1C90BECB,sctrlHENSetStartModuleHandler