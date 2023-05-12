#include "pch.h"
#include "acis_serialization.h"


outcome my_save_entity(
    ENTITY* elist,    // (in) List of the entities to save
    const char* file_name  // (in) Name of the SAT file
) {
    API_NOP_BEGIN

    ENTITY_LIST temp_list;
    temp_list.add(elist);

    // Set the units and product_id.
    FileInfo fileinfo;
    fileinfo.set_units (1.0);
    fileinfo.set_product_id ("Example Application");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    check_outcome(result); 

    // Also set the option to produce sequence numbers in the SAT file.
    result = api_set_int_option("sequence_save_files", 1);
    check_outcome(result);

    // Open a file for writing, save the list of entities, and close the file.
    FILE * save_file = fopen(file_name, "wb");
    result = api_save_entity_list(save_file, TRUE, temp_list);
    fclose(save_file);

    API_NOP_END

    return result;
}

outcome my_save_entity_list(
    ENTITY_LIST& elist,    // (in) List of the entities to save
    const char* file_name  // (in) Name of the SAT file
) {
  API_NOP_BEGIN

    // Set the units and product_id.
    FileInfo fileinfo;
    fileinfo.set_units (1.0);
    fileinfo.set_product_id ("Example Application");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    check_outcome(result); 

    // Also set the option to produce sequence numbers in the SAT file.
    result = api_set_int_option("sequence_save_files", 1);
    check_outcome(result);

    // Open a file for writing, save the list of entities, and close the file.
    FILE * save_file = fopen(file_name, "wb");
    result = api_save_entity_list(save_file, TRUE, elist);
    fclose(save_file);

    API_NOP_END

    return result;
}

outcome my_restore_entity_list(
    const char* file_name,  // (in) Name of the SAT file
    ENTITY_LIST& elist      // (out) List of the restored entities
) {
  API_BEGIN

    // Open a file for reading, restore the model from the file, and close the file.
    FILE * save_file = fopen(file_name, "rb");
    result = api_restore_entity_list(save_file, TRUE, elist);
    fclose(save_file);
    check_outcome(result);

    API_END

    return result;
}

outcome aei_SAVE_ENTITY_LIST(ENTITY_LIST& output_ents, AcisOptions* ptrAcisOpt)
{
    return  my_save_entity_list(output_ents, "E:/temp/output.sat");
}

outcome aei_RESTORE_ENTITY_LIST(ENTITY_LIST& output_ents, AcisOptions* ptrAcisOpt)
{
    return my_restore_entity_list("E:/temp/output.sat",output_ents);
}



