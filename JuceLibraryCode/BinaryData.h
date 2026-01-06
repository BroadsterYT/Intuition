/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Exo2Bold_ttf;
    const int            Exo2Bold_ttfSize = 151520;

    extern const char*   Exo2Italic_ttf;
    const int            Exo2Italic_ttfSize = 151336;

    extern const char*   Exo2Regular_ttf;
    const int            Exo2Regular_ttfSize = 151388;

    extern const char*   Inter_28ptMedium_ttf;
    const int            Inter_28ptMedium_ttfSize = 342808;

    extern const char*   Inter_28ptRegular_ttf;
    const int            Inter_28ptRegular_ttfSize = 342484;

    extern const char*   JetBrainsMonoRegular_ttf;
    const int            JetBrainsMonoRegular_ttfSize = 273900;

    extern const char*   OutfitBold_ttf;
    const int            OutfitBold_ttfSize = 55392;

    extern const char*   OutfitRegular_ttf;
    const int            OutfitRegular_ttfSize = 54912;

    extern const char*   tooltips_json;
    const int            tooltips_jsonSize = 8368;

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
    const int            intumi_exeSize = 12078151;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 15;

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
