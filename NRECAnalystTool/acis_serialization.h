#pragma once

#include <stdio.h>
#include "acis.hxx"
#include "api.hxx"
#include "kernapi.hxx"
#include "lists.hxx"
#include "fileinfo.hxx"
#include "ckoutcom.hxx"


outcome my_save_entity_list(
    ENTITY_LIST& elist,    // (in) List of the entities to save
    const char* file_name  // (in) Name of the SAT file
);

outcome my_restore_entity_list(
    const char* file_name,  // (in) Name of the SAT file
    ENTITY_LIST& elist      // (out) List of the restored entities
);

outcome my_save_entity(
    ENTITY* elist,    // (in) List of the entities to save
    const char* file_name  // (in) Name of the SAT file
);

outcome aei_SAVE_ENTITY_LIST(ENTITY_LIST& output_ents, AcisOptions* ptrAcisOpt);

outcome aei_RESTORE_ENTITY_LIST(ENTITY_LIST& output_ents, AcisOptions* ptrAcisOpt);
