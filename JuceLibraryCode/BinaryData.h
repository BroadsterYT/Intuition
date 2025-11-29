/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   tooltips_json;
    const int            tooltips_jsonSize = 5634;

    extern const char*   AKWF_saw_wav;
    const int            AKWF_saw_wavSize = 1344;

    extern const char*   AKWF_sin_wav;
    const int            AKWF_sin_wavSize = 1344;

    extern const char*   AKWF_squ_wav;
    const int            AKWF_squ_wavSize = 1344;

    extern const char*   AKWF_tri_wav;
    const int            AKWF_tri_wavSize = 1344;

    extern const char*   intumi;
    const int            intumiSize = 11135600;

    extern const char*   intumi_exe;
    const int            intumi_exeSize = 12078802;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
