#include "datcc.h"
#include "data.h"
#include <tclap/CmdLine.h>
#include <iostream>

int main(const int argc, const char* argv[]) {
  std::cout << "DatCC v0.1 created by pastelmind\n" << std::endl;
  datcc::setCurrentProgramDir(argv[0]);

  try {
    const char exampleStr[] = "EXAMPLES:"
      "\nDatCC -d -u ."
      "\n\tDecompiles the default units.dat file."
      "\nDatCC -c -w \"C:\\My Mod\\my weapons.ini\""
      "\n\tCompiles \"C:\\My Mod\\my weapons.ini\" into \"C:\\My Mod\\my weapons.dat\""
      "\nDatCC -c -t \"C:\\test\\tech.ini\" -b C:\\test\\techdata.dat"
      "\n\tCompiles \"C:\\test\\tech.ini\" on top of C:\\test\\techdata.dat"
      "\nDatCC -r -f \"example mod-flingy.dat\" -b ."
      "\n\tCompares \"example mod-flingy.dat\" with the default flingy.dat and save the differences to \"example mod-flingy.ini\"";
    TCLAP::CmdLine cmd(exampleStr, ' ', "0.1");

    TCLAP::SwitchArg isCompileModeArg  ("c", "compile",   "Compiles INI files to DAT files.");
    TCLAP::SwitchArg isDecompileModeArg("d", "decompile", "Decompiles DAT files to INI files.");
    TCLAP::SwitchArg isCompareModeArg  ("r", "compare",   "Compares the DAT file with the base DAT file and decompiles the differences to an INI file");

    std::vector<TCLAP::Arg*> modeSwitchArgs;
    modeSwitchArgs.push_back(&isCompileModeArg);
    modeSwitchArgs.push_back(&isDecompileModeArg);
    modeSwitchArgs.push_back(&isCompareModeArg);
    cmd.xorAdd(modeSwitchArgs);

    TCLAP::ValueArg<std::string> baseDatArg("b", "basedat",
      "Base DAT file to use when compiling/comparing. If omitted, the default DAT files are used.",
      false, ".", "base file");
    cmd.add(baseDatArg);

    TCLAP::UnlabeledValueArg<std::string> inputFileArg("input",
      "In compile mode, specify the INI file to compile. In decompile or compare mode, specify the DAT file to decompile or compare. Use . to decompile the default DAT files.",
      true, "", "input file");
    cmd.add(inputFileArg);

    TCLAP::UnlabeledValueArg<std::string> outputFileArg("output",
      "Specify the output DAT file (in compile mode) or INI file (in decompile/compare mode). If omitted, the output file is named after the input file.",
      false, "", "output file");
    cmd.add(outputFileArg);

    TCLAP::SwitchArg useUnitsDatArg   ("u", "units",    "Operate on units.dat");
    TCLAP::SwitchArg useWeaponsDatArg ("w", "weapons",  "Operate on weapons.dat");
    TCLAP::SwitchArg useFlingyDatArg  ("f", "flingy",   "Operate on flingy.dat");
    TCLAP::SwitchArg useSpritesDatArg ("s", "sprites",  "Operate on sprites.dat");
    TCLAP::SwitchArg useImagesDatArg  ("i", "images",   "Operate on images.dat");
    TCLAP::SwitchArg useUpgradesDatArg("g", "upgrades", "Operate on upgrades.dat");
    TCLAP::SwitchArg useTechdataDatArg("t", "techdata", "Operate on techdata.dat");
    TCLAP::SwitchArg useSfxdataDatArg ("x", "sfxdata",  "Operate on sfxdata.dat (NOT SUPPORTED YET!)");
    TCLAP::SwitchArg usePortdataDatArg("p", "portdata", "Operate on portdata.dat (NOT SUPPORTED YET!)");
    TCLAP::SwitchArg useMapdataDatArg ("m", "mapdata",  "Operate on mapdata.dat (NOT SUPPORTED YET)");
    TCLAP::SwitchArg useOrdersDatArg  ("o", "orders",   "Operate on orders.dat");

    std::vector<TCLAP::Arg*> datSwitchArgs;
    datSwitchArgs.push_back(&useUnitsDatArg);
    datSwitchArgs.push_back(&useWeaponsDatArg);
    datSwitchArgs.push_back(&useFlingyDatArg);
    datSwitchArgs.push_back(&useSpritesDatArg);
    datSwitchArgs.push_back(&useImagesDatArg);
    datSwitchArgs.push_back(&useUpgradesDatArg);
    datSwitchArgs.push_back(&useTechdataDatArg);
    datSwitchArgs.push_back(&useSfxdataDatArg);
    datSwitchArgs.push_back(&usePortdataDatArg);
    datSwitchArgs.push_back(&useMapdataDatArg);
    datSwitchArgs.push_back(&useOrdersDatArg);
    cmd.xorAdd(datSwitchArgs);

    cmd.parse(argc, argv);

    //-------- Main program logic start --------//

    datcc::loadData();

    if (isCompileModeArg.isSet()) {
      //Compile mode
      if (useUnitsDatArg.isSet())
        datcc::compileUnits(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useWeaponsDatArg.isSet())
        datcc::compileWeapons(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else if (useFlingyDatArg.isSet())
        datcc::compileFlingy(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useSpritesDatArg.isSet())
        datcc::compileSprites(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useImagesDatArg.isSet())
        datcc::compileImages(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useUpgradesDatArg.isSet())
        datcc::compileUpgrades(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useTechdataDatArg.isSet())
        datcc::compileTechdata(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else if (useOrdersDatArg.isSet())
        datcc::compileOrders(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else
        throw TCLAP::ArgException("Unsupported DAT file format, please wait for new version.",
          "UnsupportedFormat", "Unsupported DAT format exception");
    }
    else if (isDecompileModeArg.isSet()) {
      if (baseDatArg.isSet())
        throw TCLAP::ArgException("Base DAT argument is unnecessary for decompile mode", "unused_basedat", "Unused base DAT argument");

      //Decompile mode
      if (useUnitsDatArg.isSet())
        datcc::decompileUnits(inputFileArg.getValue(), outputFileArg.getValue());
      
      else if (useWeaponsDatArg.isSet())
        datcc::decompileWeapons(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useFlingyDatArg.isSet())
        datcc::decompileFlingy(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useSpritesDatArg.isSet())
        datcc::decompileSprites(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useImagesDatArg.isSet())
        datcc::decompileImages(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useUpgradesDatArg.isSet())
        datcc::decompileUpgrades(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useTechdataDatArg.isSet())
        datcc::decompileTechdata(inputFileArg.getValue(), outputFileArg.getValue());

      else if (useOrdersDatArg.isSet())
        datcc::decompileOrders(inputFileArg.getValue(), outputFileArg.getValue());

      else
        throw TCLAP::ArgException("Unsupported DAT file format, please wait for new version.",
          "UnsupportedFormat", "Unsupported DAT format exception");
    }
    else if (isCompareModeArg.isSet()) {
      //Compare mode
      if (useUnitsDatArg.isSet())
        datcc::compareUnits(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useWeaponsDatArg.isSet())
        datcc::compareWeapons(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else if (useFlingyDatArg.isSet())
        datcc::compareFlingy(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useSpritesDatArg.isSet())
        datcc::compareSprites(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useImagesDatArg.isSet())
        datcc::compareImages(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useUpgradesDatArg.isSet())
        datcc::compareUpgrades(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());

      else if (useTechdataDatArg.isSet())
        datcc::compareTechdata(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else if (useOrdersDatArg.isSet())
        datcc::compareOrders(inputFileArg.getValue(), outputFileArg.getValue(), baseDatArg.getValue());
      
      else
        throw TCLAP::ArgException("Unsupported DAT file format, please wait for new version.",
          "UnsupportedFormat", "Unsupported DAT format exception");
    }
    else { //Should never reach here
      throw TCLAP::ArgException("Cannot determine compile/decompile mode");
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
