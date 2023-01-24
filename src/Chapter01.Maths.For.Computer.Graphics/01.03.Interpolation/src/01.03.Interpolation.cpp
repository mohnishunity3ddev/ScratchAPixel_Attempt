#include <iostream>
#include <file_operations.h>

int
main()
{
    read_result Result = JReadFileC("apple.png");

    WriteTestPPMFile(128, 128);
    
    std::cout << "File Size: " << Result.ContentSize << std::endl;
}
