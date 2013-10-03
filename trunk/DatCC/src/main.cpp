#include "datcc.h"
#include "data.h"
#include <tclap/CmdLine.h>
#include <iostream>

int main(const int argc, const char* argv[]) {
  std::cout << "DatCC v0.1 created by pastelmind\n" << std::endl;
  datcc::setCurrentProgramDir(argv[0]);

  try {
    TCLAP::CmdLine cmd("", ' ', "0.1");

    TCLAP::SwitchArg isCompileModeArg("c", "compile", "Compiles INI files to DAT files.", false);
    TCLAP::SwitchArg isDecompileModeArg("d", "decompile", "Decompiles DAT files to INI files.", true);
    cmd.xorAdd(isCompileModeArg, isDecompileModeArg);

    TCLAP::ValueArg<std::string> ordersFileArg("o", "orders",
      "In compile mode, specify INI file to compile into orders.dat. "
      "In decompile mode, specify orders.dat to decompile (use . to decompile default).",
      false, "", "orders file");
    cmd.add(ordersFileArg);

    TCLAP::ValueArg<std::string> techFileArg("t", "tech",
      "In compile mode, specify INI file to compile into techdata.dat. "
      "In decompile mode, specify techdata.dat to decompile (use . to decompile default).",
      false, "", "tech file");
    cmd.add(techFileArg);

    TCLAP::ValueArg<std::string> upgradesFileArg("g", "upgrades",
      "In compile mode, specify INI file to compile into upgrades.dat. "
      "In decompile mode, specify upgrades.dat to decompile (use . to decompile default).",
      false, "", "upgrades file");
    cmd.add(upgradesFileArg);
    
    TCLAP::ValueArg<std::string> imagesFileArg("i", "images",
      "In compile mode, specify INI file to compile into images.dat. "
      "In decompile mode, specify images.dat to decompile (use . to decompile default).",
      false, "", "images file");
    cmd.add(imagesFileArg);
    
    TCLAP::ValueArg<std::string> spritesFileArg("s", "sprites",
      "In compile mode, specify INI file to compile into sprites.dat. "
      "In decompile mode, specify sprites.dat to decompile (use . to decompile default).",
      false, "", "sprites file");
    cmd.add(spritesFileArg);
    
    TCLAP::ValueArg<std::string> flingyFileArg("f", "flingy",
      "In compile mode, specify INI file to compile into flingy.dat. "
      "In decompile mode, specify flingy.dat to decompile (use . to decompile default).",
      false, "", "flingy file");
    cmd.add(flingyFileArg);

    TCLAP::ValueArg<std::string> weaponsFileArg("w", "weapons",
      "In compile mode, specify INI file to compile into weapons.dat. "
      "In decompile mode, specify weapons.dat to decompile (use . to decompile default).",
      false, "", "weapons file");
    cmd.add(weaponsFileArg);

    TCLAP::ValueArg<std::string> unitsFileArg("u", "units",
      "In compile mode, specify INI file to compile into units.dat. "
      "In decompile mode, specify units.dat to decompile (use . to decompile default).",
      false, "", "units file");
    cmd.add(unitsFileArg);

    cmd.parse(argc, argv);

    //-------- Main program logic start --------//

    datcc::loadData();

    //@TODO: Add check for zero file options
    if (isCompileModeArg.isSet()) {
      //Compile mode
      if (unitsFileArg.isSet())
        datcc::compileUnits(unitsFileArg.getValue());
      
      if (weaponsFileArg.isSet())
        datcc::compileWeapons(weaponsFileArg.getValue());
      
      if (flingyFileArg.isSet())
        datcc::compileFlingy(flingyFileArg.getValue());

      if (spritesFileArg.isSet())
        datcc::compileSprites(spritesFileArg.getValue());

      if (imagesFileArg.isSet())
        datcc::compileImages(imagesFileArg.getValue());

      if (upgradesFileArg.isSet())
        datcc::compileUpgrades(upgradesFileArg.getValue());
    }
    else if (isDecompileModeArg.isSet()) {
      //Decompile mode
      if (unitsFileArg.isSet())
        datcc::decompileUnits(unitsFileArg.getValue());
      
      if (weaponsFileArg.isSet())
        datcc::decompileWeapons(weaponsFileArg.getValue());

      if (flingyFileArg.isSet())
        datcc::decompileFlingy(flingyFileArg.getValue());

      if (spritesFileArg.isSet())
        datcc::decompileSprites(spritesFileArg.getValue());

      if (imagesFileArg.isSet())
        datcc::decompileImages(imagesFileArg.getValue());

      if (upgradesFileArg.isSet())
        datcc::decompileUpgrades(upgradesFileArg.getValue());
    }
    else { //Should never reach here
      throw new TCLAP::ArgException("Cannot determine compile/decompile mode");
    }
  }
  catch (TCLAP::ArgException &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

/*
  //No args
  if (argc <= 1) {
    std::cout << "Usage: DatCC.exe [-c|-d] [-u units_file]\n\n"
      "Options:\n"
      "  -c\t Compiles an INI file to a DAT file. Default.\n"
      "  -d\t Decompiles a DAT file to an INI file.\n"
      "  -u\t Path of INI/DAT file to compile/decompile. If unspecified, DatCC uses the default units.dat.\n";

    return 0;
  }

  bool isCompileMode = true;
  char inputFile[1000], outputFile[1000] = "units.ini";
  bool useDefaultUnitsDat = true;

  //Argument parsing
  for (int i = 0; i < argc; ++i) {
    std::cout << '"' << argv[i] << '"' << std::endl;

    if (strcmp(argv[i], "-c") == 0)
      isCompileMode = true;
    else if (strcmp(argv[i], "-d") == 0)
      isCompileMode = false;
    else if (strcmp(argv[i], "-u") == 0) {
      if (i + 1 >= argc) {
        std::cerr << "Error: Missing filename" << std::endl;
        return 1;
      }

      strcpy(inputFile, argv[i + 1]);
      std::ifstream in(inputFile, std::ios::in | std::ios::binary);
      if (in.fail()) {
        std::cerr << "Error: Cannot read " << inputFile << std::endl;
        return 1;
      }
      in.close();

      if (!isCompileMode)
        useDefaultUnitsDat = false;

      if (isCompileMode)
        sprintf(outputFile, "%s.dat", inputFile);
      else
        sprintf(outputFile, "%s.ini", inputFile);

      std::ofstream out(outputFile, std::ios::out | std::ios::binary);
      if (out.fail()) {
        std::cerr << "Error: Cannot open " << outputFile << " for writing." << std::endl;
        return 1;
      }
      out.close();
    }
  }

  std::cout << "Loading entry names..." << std::endl;
  datcc::loadEntryNames();

  std::cout << "Loading stat_txt.tbl..." << std::endl;
  statTxtTbl.loadFile("./data/stat_txt.tbl");

  datcc::UnitsDat unitsDat;

  if (!isCompileMode && useDefaultUnitsDat)
    strcpy(inputFile, "./data/units.dat");

  if (isCompileMode) {
    std::cout << "Compiling..." << std::endl;
    datcc::compileDat(unitsDat, inputFile, outputFile);
  }
  else {
    std::cout << "Decompiling..." << std::endl;
    datcc::decompileDat(unitsDat, inputFile, outputFile);
  }

  datcc::WeaponsDat weaponsDat;
  datcc::decompileDat(weaponsDat, "./data/weapons.dat", "weapons.ini");

  std::cout << "Done!" << std::endl;*/

  return 0;
}
