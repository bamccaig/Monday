/**
 * Test the functionality of the Level loader.
 *
 *  The purpose of this file is just to make sure that loading a Map in from
 *  a file works properly.
 *
 *  We have to hard-code any values that we don't have which we need; the
 *  values usually come from config files.
 *
 */

#include "Test3_Worlds.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines show_verbose() */
#include "Debug.h"

/* Magic function that scans a config file for the first non-linecomment */
#include "Parse_Lines.h"

/* Defines parallax_type, MapData, MapArray
 */
#include "MapData.h"

#include "World.h"
#include "Map.h"
#include "Level.h"

#include "Map_Manager.h"
#include "Level_Manager.h"
#include "Layer_Manager.h"
#include "Trigger_Manager.h"

#include <stdlib.h>
#include <string>
#include <vector>


bool enableDebug;
bool enableVerbose;


std::vector<std::string> parseArgs(int argc, char *argv[])
{
	/* Inital values */
	enableDebug = false;
	enableVerbose = false;

	std::vector<std::string> filenames;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		/* Abbreviated options begin with "-", and full options begin with
		 * "--" to conform to standards.
		 */
		if (arg[0] == '-')
		{
			/* DEBUG */
			if (arg == "-d" || arg == "--debug")
			{
				enableDebug = true;
				verbose_good(std::cerr, "Setting debug ON\n");
				continue;
			}
			else if (arg == "-nd" || arg == "--nodebug")
			{
				verbose_good(std::cerr, "Setting debug OFF\n");
				enableDebug = false;
				continue;
			}

			/* VERBOSE */
			else if (arg == "-v" || arg == "--verbose")
			{
				enableVerbose = true;
				verbose_bad(std::cerr, "Setting verbose ON\n");
				continue;
			}
			else if (arg == "-nv" || arg == "--noverbose")
			{
				verbose_bad(std::cerr, "Setting verbose OFF\n");
				enableVerbose = false;
				continue;
			}
		}

		/* If we got here, we probably have a valid filename. */
		filenames.push_back(arg);
	}

	return filenames;
}


void usage(const std::string &filename)
{
	std::cout << "Usage: " << filename << " filename [-d|-nd|-v|-nv] [--debug|--nodebug|--verbose|--noverbose]\n";
	std::cout << "\tfilename:         Path to the \"world_.cfg\" config file.\n";
	std::cout << "\t-d, --debug:      Turns on extra non-error messages.\n";
	std::cout << "\t-nd, --nodebug:   (default) Turns off extra non-error messages.\n";
	std::cout << "\t-v, --verbose:    Turns on extra error messages.\n";
	std::cout << "\t-nv, --noverbose: (default) Turns off extra error messages.\n";
}


int main(int argc, char *argv[])
{
	std::vector<std::string> filenames = parseArgs(argc, argv);
	if (filenames.size() == 0)
	{
		usage(argv[0]);
		return 0;
	}

	/* Set some initial values for this test program that the program will
	 * need to properly extract the files we need.
	 *
	 * This data is typically pulled in from the config file:
	 *   "./media/world#.cfg"
	 */

	u32 tileWidth = 32;
	u32 tileHeight = 32;

	/* Points to the "./media/" base path */
	std::string currentDirectory = "./media/";

	for (u32 inputFiles = 0; inputFiles < filenames.size(); ++inputFiles)
	{
		/* Load in all of the Maps for a single World */
		World world;

		/* Tells the World what the base directory is:
		 *   "./media/"
		 */
		world.Set_Base_Directory(currentDirectory);

		/* Tell the World where the file is (relative to the World's base) where
		 * the "world#.cfg" file can be found.
		 */
		world.Set_Config_Path(filenames[inputFiles]);

		/* Now that the map knows where to pull the data from, actually load
		 * the data into the map (pass in the tile dimensions)
		 */
		world.Load_Maps(tileWidth, tileHeight);

		Map_Manager *mapManager = world.Get_Map_Manager();

		if (mapManager != NULL)
		{
			mapManager->Set_Tile_Width(tileWidth);
			mapManager->Set_Tile_Height(tileHeight);

			mapManager->Show_All(std::cout);

			/* The manager has parsed the World's config file.  Now, loop
			 * through each "map path" found and load that map.
			 *
			 * This should be a fairly large printout, since it will show
			 * this manager's Show_All() as well as each of the 3 maps'
			 * manager Show_All() readouts.
			 */


			MapArray mapArray = mapManager->Get_MapArray();

			u32 numMaps = mapArray.size();
			for (u32 i = 0; i < numMaps; ++i)
			{
				MapData mapData = mapArray[i];

				/* Load the Map:
				 * - Load each Level through the Map_Manager
				 * - Then call the manager's Show_All() function to display everything
				 *   to the screen
				 */
				Map map(tileWidth, tileHeight);
				map.Set_MapData(mapData);

				std::string mapBasePath = mapManager->Get_WorkingDirectory() + mapManager->Get_SubDirectory();

				/* Tells the Map what the base directory is:
				 *   "./media/world#/"
				 */
				map.Set_Base_Directory(mapBasePath);

				/* Now that the Map knows where to pull the data from,
				 * actually load the data into the map
				 */
				map.Load_Levels();

				Level_Manager *levelManager = map.Get_Level_Manager();
				if (levelManager != NULL)
				{
					levelManager->Show_All(std::cout);

					/* The manager has parsed the Map's config file.  Now, loop
					 * through each "level path" found and load that level.
					 *
					 * This should be a fairly large printout, since it will show
					 * this manager's Show_All() as well as each of the 3 levels'
					 * manager Show_All() readouts.
					 */


					LevelArray levelArray = levelManager->Get_LevelArray();

					u32 numLevels = levelArray.size();
					for (u32 i = 0; i < numLevels; ++i)
					{
						LevelData levelData = levelArray[i];

						/* Load the Level:
						 * - Load each Layer through the Level_Manager
						 * - Then call the manager's Show_All() function to display everything
						 *   to the screen
						 */
						Level level(levelManager->Get_Num_Rows(), levelManager->Get_Num_Columns());
						level.Set_LevelData(levelData);

						std::string levelBasePath = levelManager->Get_WorkingDirectory() + levelManager->Get_SubDirectory();

						/* Tells the Level what the base directory is:
						 *   "./media/world#/map#/"
						 */
						level.Set_Base_Directory(levelBasePath);

						/* Now that the Level knows where to pull the data from,
						 * actually load the data into the level
						 */
						level.Load_Layers();
						level.Load_Triggers();


						Layer_Manager *layerManager = level.Get_Layer_Manager();
						if (layerManager != NULL)
						{
							layerManager->Show_All(std::cout);
						}

						Trigger_Manager *triggerManager = level.Get_Trigger_Manager();
						if (triggerManager != NULL)
						{
							triggerManager->Show_All(std::cout);
						}
					}
				}
			}
		}
	}
	return 0;
}