#include <iostream>
#include <fstream>
#include <filesystem>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cerr << "Error: no kernel file specified.\n";
        std::exit(1);
    }
    std::string outFile = "fat.img";
    if(argc > 2)
        outFile = argv[2];
    system("dd if=/dev/zero of=fat.img bs=1M count=128");
    system("mformat -i fat.img -F ::");
    system("mmd -i fat.img ::/EFI");
	system("mmd -i fat.img ::/EFI/BOOT");
	system("mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI");
    //std::string cmd = "mcopy -i fat.img ";
    //cmd += argv[1];
    //cmd += " ::";
	//system(cmd.data());
    std::filesystem::path p = argv[1];
    std::filesystem::recursive_directory_iterator it = std::filesystem::recursive_directory_iterator(p);
    for(const std::filesystem::path& entry : it)
    {
        std::string cmd = "mcopy -i fat.img ";
        cmd += entry.string();
        cmd += " ::";
        system(cmd.data());
        std::cout << cmd << "\n";
    }
    if(outFile != "fat.img")
    {
        std::string cmd = "mv fat.img " + outFile;
        system(cmd.data());
    }   
    return 0;
}