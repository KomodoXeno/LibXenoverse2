#include "LibXenoverse.h"
#include <iostream>
#include <map>

#define BUFLEN 1024


std::vector<std::string> split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

bool isNumber(const string& val)
{
	std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}

std::string readLine()
{
	std::string line;
	int c = 0;

	fflush(stdin);

	while(c != '\n')
	{
		c = getchar();
		if (c != '\n')
		{
			line += string((char*)&c);
		}
		else{
			line += string("\0");
		}
	}

	printf("\n");
	return line;
}


int main(int argc, char** argv)
{

	LibXenoverse::initializeDebuggingLog();

	printf("Welcome to Ean organizer v0.9.1kx\n\n");


	string help = "Commands:\n\
	<Required Parameter> [Optional Parameter]\n\n\
	'Help [Command]' e.g. 'Help Load' for help with Load command\n\
	'Load <path\\filenameEanFile> <file2.ean> ...'\n\
	'Save <indexEan> <path\\filename>'\n\
	'GetEanFileList'\n\
	'GetAnimList <Eanindex>'\n\
	'Copy <indexEan> <indexAnimation>'\n\
	'Paste <indexEan> <indexAnimation>'\n\
	'Append <EanIndex> <indexAnimation>'\n\
	'Cut <EanIndex> <indexAnimation> <indexKeyFrame_Start or -1> <indexKeyFrame_End or -1>'\n\
	'FixedPositionComponent <EanIndex> <indexAnimation> <indexBone> <component 'X' 'Y' or 'Z'>'\n\
	'MovePositionComponent <EanIndex> <indexAnimation> <indexBone> <component 'X' 'Y' or 'Z'> X.xxx'\n\
	'Rename <indexEan> <indexAnimation> <newName>'\n\
	'Erase <EanIndex> <AnimIndex> [AnimIndex_rangeEnd]'\n\
	'Insert <indexEan> <indexAnimation>'\n\
	'GetBoneList <indexEan>'\n\
	'AddBoneFilter <indexEan> <indexBone1> [indexBone2] [indexBone3] ...'\n\
	'AddAllBoneInFilterFor <indexEan> <indexBone1_notIn> [indexBone2_notIn] [indexBone3_notIn] ...'\n\
	'AddBoneFilterPreset <indexEan> <presetName>'\n\
	'GetBoneFilter'\n\
	'ResetBoneFilter'\n\
	'PasteWithBoneFilter <indexEan> <indexAnimation>'\n\
	'CopyPasteRange <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End> [indexAnimation_DestionationStart]'\n\
	'CopyPasteRange_WithBoneFilter <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End> [indexAnimation_DestionationStart]'\n\
	'GetDuration <indexEan> <AnimIndex> [indexBone]'\n\
	'SetDuration <indexEan> <AnimIndex> [indexBone] X.xxx'\n\
	'SetDurationInFrames <indexEan> <AnimIndex> [indexBone] X'\n\
	'LoopAnimation <indexEan> <AnimIndex> <indexBone> X'\n\
	'AddBoneOffsetScaleOnAnimationPosition <indexEan> <AnimIndex> <indexBone> <offsetToAdd_X> <offsetToAdd_Y> <offsetToAdd_Z> <scaleToMultiply_X> <scaleToMultiply_Y> <scaleToMultiply_Z>'\n\
	'MatchAnimationDuration <indexEAN_src> <indexEAN_toMatch> <indexAnimation_Start> <indexAnimation_End>'\n\
	'Quit'\n\
	\n\
	note: <indexBone> can be substituted with the bone name, e.g. AddBoneFilter 0 b_R_Arm1\n";

	std::map<string, string> helptext;
	helptext["HELP"] = "Help [Command]\nList all commands\n\n";
	helptext["LOAD"] = "Load <path\\filenameEanFile> <file2.ean> ...\nLoad an EAN file\n\n";
	helptext["SAVE"] = "Save <indexEan> <path\\filename>\nSave your modified EAN file\n\n";
	helptext["GETEANFILELIST"] = "GetEanFileList\nGet list of loaded EAN files\n\n";
	helptext["GETANIMLIST"] = "GetAnimList <Eanindex>\nGet list of animations within a specified EAN file\n\n";
	helptext["GETBONELIST"] = "GetBoneList <indexEan>\nGet list of bones within a specified EAN file\n\n";	
	helptext["RENAME"] = "Rename <indexEan> <indexAnimation> <newName>\nRename a specified animation\n\n";
	helptext["COPY"] = "Copy\nCopy on the specified animation to clipboard\n\n";
	helptext["PASTE"] = "Paste <indexEan> <indexAnimation>\nPaste clipboard animation OVER destination animation\n\n";
	helptext["APPEND"] = "Append <EanIndex> <indexAnimation>\nAdd the animation to an EAN.\n	Note: A new keyframe will be added to the end, do not delete\n\n";
	helptext["CUT"] = "Cut <EanIndex> <indexAnimation> <indexKeyFrame_Start or -1> <indexKeyFrame_End or -1>\nCut the animation to keep the range. Use -1 to for default value\n\n";
	helptext["FIXEDPOSITIONCOMPONENT"] = "FixedPositionComponent <EanIndex> <indexAnimation> <indexBone> <component X Y or Z>\n	Use the value on the first keyframe to replace all other keyframes, for all the component of a animation of a bone\n\n";
	helptext["MOVEPOSITIONCOMPONENT"] = "MovePositionComponent <EanIndex> <indexAnimation> <indexBone> <component X Y or Z> X.xxx\n	Move the animation for a component of position of the bone on an animation\n\n";
	helptext["ERASE"] = "Erase <EanIndex> <AnimIndex> [AnimIndex_rangeEnd]\nErase only Animation in AnimIndex\n	If you use [AnimIndex_rangeEnd], it will erase the entire range between the two animIndex\n\n";
	helptext["INSERT"] = "Insert <indexEan> <indexAnimation>\nInsert an animation before index of Animation specified\n\n";
	helptext["ADDBONEFILTER"] = "AddBoneFilter <indexEan> <indexBone1> [indexBone2] [indexBone3] ...\nBy using the filter, when you paste with bone filter, only the specified bones will be affected\n	If you justto copy the animation from the tail, only specify the tail bones	<boneIndex> can be substituted for bone names e.g. b_R_Arm1\n\n";
	helptext["GETBONEFILTER"] = "GetBoneFilter\nGet bones currently in the bone filter\n\n";
	helptext["ADDALLBONEINFILTERFOR"] = "AddAllBoneInFilterFor <indexEan> <indexBone1_notIn> [indexBone2_notIn] [indexBone3_notIn] ...\nQuick way to add all bones to filter except for the bones specified\n\n";
	helptext["ADDBONEFILTERPRESET"] = "AddBoneFilterPreset <indexEan> <presetName>\nAdd a preset list of bones to the bone filter:\n\n	'torso'\n	'head'\n	'arms'\n	'arm_left'\n	'arm_right'\n	'hands'\n	'hand_left'\n	'hand_right'\n	'legs'\n	'leg_left'\n	'leg_right'\n	'tail'\n	'wings'\n	'spines'\n	'sword'\n	'cane'\n	'spear'\n	'accessories'\n\n";
	helptext["RESETBONEFILTER"] = "ResetBoneFilter\nClear the bonefilter\n\n";
	helptext["PASTEWITHBONEFILTER"] = "PasteWithBoneFilter <indexEan> <indexAnimation>\nPaste copied animation with bone filter'\n\n";
	helptext["COPYPASTERANGE"] = "CopyPasteRange <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End> [indexAnimation_DestionationStart]\nCopy and paste a range of animations\n	[indexAnimation_DestionationStart] is a index for start the paste on destination. (that will add new aniamtion if up to number of animations of destination)\n\n";
	helptext["COPYPASTERANGE_WITHBONEFILTER"] = "CopyPasteRange_WithBoneFilter <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End> [indexAnimation_DestionationStart]\nCopy and paste a range of animations with the bone filter\n	[indexAnimation_DestionationStart] will specify a start the paste on destination. (that will add new aniamtion if up to number of animations of destination)\n\n";
	helptext["GETDURATION"] = "GetDuration <indexEan> <AnimIndex> [indexBone] X.xxx\nGet the duration of a animation\n	You can specify a bone with [indexBone] argument\n\n";
	helptext["SETDURATION"] = "SetDuration <indexEan> <AnimIndex> [indexBone]\nSet the duration of a animation\n	You can specify a bone with [indexBone] argument\n\n";
	helptext["SETDURATIONINFRAMES"] = "SetDurationInFrames <indexEan> <AnimIndex> [indexBone] X\nSet the duration of a animation in frames\n	You can specify a bone with [indexBone] argument\n\n";
	helptext["LOOPANIMATION"] = "LoopAnimation <indexEan> <AnimIndex> <indexBone> X\nLoop the animation to increase the duration\n	You can specify a bone with [indexBone] argument\n\n";
	helptext["ADDBONEOFFSETSCALEONANIMATIONPOSITION"] = "AddBoneOffsetScaleOnAnimationPosition <indexEan> <AnimIndex> <indexBone> <offsetToAdd_X> <offsetToAdd_Y> <offsetToAdd_Z> <scaleToMultiply_X> <scaleToMultiply_Y> <scaleToMultiply_Z>\nAdd an offset to a bone and/or multiply by scale.\n	Note: Use -1 for <AnimIndex> to apply to all animations.\n	Neutral Values: offsetToAdd: 0.0, scaleToMultiply: 1.0\n\n";
	helptext["MATCHANIMATIONDURATION"] = "MatchAnimationDuration <indexEAN_src> <indexEAN_toMatch> <indexAnimation_Start> <indexAnimation_End>\nThe SECOND EAN will have the same duration of the animation from the source EAN\n\n";
	helptext["QUIT"] = "Quit\nExit the program\n\n";

	printf((string("You can load several EAN Files, list their animations, copy an animation from one EAN to another. KEEP ORDER of animations if you want to use modified ean in game (need configurator).\n\"path with spaces\" is now allowed!\n\n") + help +"\n").c_str());


	
	vector<LibXenoverse::EAN*> listEanFile;
	vector<string> listFileName;
	LibXenoverse::EANAnimation *mSavedAnimation = 0;
	
	vector<vector<size_t>> mListBoneFilters;
	vector<size_t> mListBoneFilters_eanIndex;

	string line = "";
	std::vector<string> arguments;
	std::vector<string> arg_tmp;
	bool automaticLine = false;

	while ((arguments.size()==0) || (arguments.at(0) != "Quit"))
	{
		if (!automaticLine)								//it's a one time automatic command
		{
			printf("So, what do you want to do next?\n");
			line = readLine();
		}
		automaticLine = false;




		//get full argument with take care of '"' to have filename with spaces.
		arg_tmp = split(line, ' ');
		size_t nbArg = arg_tmp.size();

		arguments.clear();
		string str_tmp = "";
		string str2_tmp = "";
		for (size_t i = 0; i < nbArg; i++)									//detection of options, remove then from arguments list, plus take care of folder with spaces inside (use " ")
		{
			str_tmp = arg_tmp.at(i);
			if ((str2_tmp.length() == 0) && (str_tmp.length() == 0))
				continue;
			
			if ((str2_tmp.length() == 0) && (str_tmp[0] == '"'))
			{
				if (str_tmp[str_tmp.length() - 1] != '"')
				{
					str2_tmp = str_tmp.substr(1);
				}else {
					str2_tmp = str_tmp.substr(1, str_tmp.length() - 2);
					arguments.push_back(str2_tmp);
					str2_tmp = "";
				}
				continue;

			}else if (str2_tmp.length() != 0) {

				str2_tmp += " "+ str_tmp;

				if (str_tmp[str_tmp.length() - 1] == '"')
				{
					arguments.push_back(str2_tmp.substr(0, str2_tmp.length() - 1));
					str2_tmp = "";
				}
				continue;
			}

			arguments.push_back(str_tmp);
		}
		if (str2_tmp.length() != 0)					//put the last argument if they miss a " at the end
			arguments.push_back(str2_tmp);


		if (arguments.size() == 0)
			continue;











		//begin to work on Each case.

		string command = arguments.at(0);
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
		arguments.erase(arguments.begin());
		nbArg = arguments.size();

		///////////////////////////////////////////////////////////////////////////////////////////////////
		if (command == "QUIT")
		{
			break;

		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "HELP"){
			if (nbArg > 0)
			{
				string commandKey = arguments.at(0);
                std::transform(commandKey.begin(), commandKey.end(), commandKey.begin(), ::toupper);
				if (helptext.count(commandKey) == 0){
					printf("\n\nCommand doesn't exist. Use Help for the full list of commands\n");
					continue;
				}
				printf("\n\n%s", helptext.find(commandKey)->second);
			}else{
				printf("\n\n%s\n",help.c_str());
			}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "LOAD"){
	

			size_t inc = 0;
			for (size_t i = 0; i < nbArg; i++)
			{
				string filename = arguments.at(i);
				string extension = LibXenoverse::extensionFromFilename(filename, true);
				if (extension != "ean")
				{
					printf("%s is not a ean file. Skipped.\n", filename.c_str());
					continue;
				}

				LibXenoverse::EAN* ean = new LibXenoverse::EAN();
				if (!ean->load(filename))
				{
					printf("faild to load %s. Skipped.\n", filename.c_str());
					delete ean;
					continue;
				}
				printf("Ean file loaded.\n");

				listEanFile.push_back(ean);
				listFileName.push_back(filename);
				inc++;
			}
			
			if (inc == 0)
			{
				printf("Not Enought or right arguments. try 'Help' command\n");
				continue;
			}





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "SAVE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			string filename = arguments.at(1);

			if (index < listEanFile.size())
			{
				listEanFile.at(index)->save(filename);
				printf("Ean file saved at %s.\n", filename.c_str());

			}else{
				printf("index %i is not in ean file list.\n", index);
			}





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETEANFILELIST"){

			size_t nbFile = listFileName.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listFileName.at(i).c_str());


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETANIMLIST"){

			if (nbArg == 0)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));

			if (index < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(index);
				
				size_t nbAnim = eanFile->getAnimations().size();
				for (size_t i = 0; i < nbAnim; i++)
					printf("%i : %s\n", i, eanFile->getAnimations().at(i).getName().c_str());
			}else{
				printf("index %i is not in ean file list.\n", index);
			}
		



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETBONELIST"){

			if (nbArg == 0)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));

			if (index < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(index);

				if (eanFile->getSkeleton())
				{
					std::vector<LibXenoverse::ESKBone*> &bones = eanFile->getSkeleton()->getBones();
					size_t nbBone = bones.size();
					for (size_t i = 0; i < nbBone; i++)
						printf("%i : %s\n", i, bones.at(i)->getName().c_str());
				}else{
					printf("No skeleton definition in this ean file\n");
				}
			}else{
				printf("index %i is not in ean file list.\n", index);
			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETBONEFILTER"){

			size_t nbEanFileFilter = mListBoneFilters.size();

			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				size_t nbBone = mListBoneFilters.at(i).size();
				for (size_t j = 0; j < nbBone; j++)
					if(listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton())
						printf("eanfile : %i  boneIndex : %i boneName %s\n", mListBoneFilters_eanIndex.at(i), mListBoneFilters.at(i).at(j), listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j))->getName().c_str());
			}

			if (nbEanFileFilter == 0)
				printf("filter list is empty.\n");


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "COPYPASTERANGE_WITHBONEFILTER"){

			if (nbArg < 4)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile_src   = std::stoi(arguments.at(0));
			size_t indexFile_dest  = std::stoi(arguments.at(1));
			size_t indexAnim_start = std::stoi(arguments.at(2));
			size_t indexAnim_end   = std::stoi(arguments.at(3));

			if (indexAnim_start > indexAnim_end)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			size_t indexAnimation_DestionationStart = indexAnim_start;
			if (nbArg >= 4)
				indexAnimation_DestionationStart = std::stoi(arguments.at(4));


			size_t nbAnimToCopy = indexAnim_end - indexAnim_start;
			for (size_t m = 0; m <= nbAnimToCopy; m++)
			{
				size_t indexAnim = indexAnim_start + m;
				size_t indexFile = indexFile_src;

				printf("****** copy paste for the indexAnim %i ******\n", indexAnim);

				if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
					mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
					printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
					continue;
				}

				size_t indexAnim_dest = indexAnimation_DestionationStart + m;
				indexFile = indexFile_dest;
				if (indexFile < listEanFile.size())
				{
					if (mSavedAnimation)
					{
						LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

						//make a list of name of bone for filter
						vector<string> listBoneFilterNames;
						size_t nbEanFileFilter = mListBoneFilters.size();
						for (size_t i = 0; i < nbEanFileFilter; i++)
						{
							size_t nbBone = mListBoneFilters.at(i).size();
							for (size_t j = 0; j < nbBone; j++)
								listBoneFilterNames.push_back(listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j))->getName());
						}

						//if miss a animation, go to add one
						if (indexAnim_dest >= eanFile->getAnimations().size())
						{
							printf("indexAnim %i don't existe, animation will push back of the list (take care of index of animation for file working in game).\n", indexAnim_dest);

							indexAnim_dest = eanFile->getAnimations().size();
							eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, listBoneFilterNames, eanFile));

						}else{

							LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim_dest);
							animationToChange.copy(*mSavedAnimation, listBoneFilterNames);
							printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
						}
					}
					else{
						printf("error : you must use Copy command first, to select the source.\n");
					}
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim_dest);
				}

			}






		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "COPYPASTERANGE"){

			if (nbArg < 4)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile_src = std::stoi(arguments.at(0));
			size_t indexFile_dest = std::stoi(arguments.at(1));
			size_t indexAnim_start = std::stoi(arguments.at(2));
			size_t indexAnim_end = std::stoi(arguments.at(3));

			
			if (indexAnim_start > indexAnim_end)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			size_t indexAnimation_DestionationStart = indexAnim_start;
			if (nbArg >= 4)
				indexAnimation_DestionationStart = std::stoi(arguments.at(4));

			size_t nbAnimToCopy = indexAnim_end - indexAnim_start;
			for (size_t m = 0; m <= nbAnimToCopy; m++)
			{
				size_t indexAnim = indexAnim_start + m;
				size_t indexFile = indexFile_src;

				printf("****** copy paste for the indexAnim %i ******\n", indexAnim);

				if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
					mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
					printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
					continue;
				}

				size_t indexAnim_dest = indexAnimation_DestionationStart + m;
				indexFile = indexFile_dest;

				if (indexFile < listEanFile.size())
				{
					if (mSavedAnimation)
					{
						LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

						//if miss a animation, go to add one
						if (indexAnim_dest >= eanFile->getAnimations().size())
						{
							printf("indexAnim %i don't existe, animation will push back of the list (take care of index of animation for file working in game).\n", indexAnim_dest);

							indexAnim_dest = eanFile->getAnimations().size();
							eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
						}else{
							LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim_dest);
							animationToChange.copy(*mSavedAnimation);

							printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
						}
					}
					else{
						printf("error : you must use Copy command first, to select the source.\n");
					}
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim_dest);
				}

			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "COPY"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
				printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}






		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "PASTEWITHBONEFILTER"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			if (indexFile < listEanFile.size()) 
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					//make a list of name of bone for filter
					vector<string> listBoneFilterNames;
					size_t nbEanFileFilter = mListBoneFilters.size();
					for (size_t i = 0; i < nbEanFileFilter; i++)
					{
						size_t nbBone = mListBoneFilters.at(i).size();
						for (size_t j = 0; j < nbBone; j++)
							listBoneFilterNames.push_back(listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j) )->getName());
					}

					//if miss a animation, go to add one
					if (indexAnim >= eanFile->getAnimations().size())
					{
						printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");

						indexAnim = eanFile->getAnimations().size();
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, listBoneFilterNames, eanFile));

					}else{

						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						animationToChange.copy(*mSavedAnimation, listBoneFilterNames);
						printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
					}
				}
				else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}



		
			
			
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "PASTE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}			
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			if (indexFile < listEanFile.size())
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					//if miss a animation, go to add one
					if (indexAnim >= eanFile->getAnimations().size())
					{
						printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");
						
						indexAnim = eanFile->getAnimations().size();
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
					}
					else{
						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						animationToChange.copy(*mSavedAnimation);

						printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
					}	
				}
				else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "APPEND") {

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			if (indexFile < listEanFile.size())
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					//if miss a animation, go to add one
					if (indexAnim >= eanFile->getAnimations().size())
					{
						printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");

						indexAnim = eanFile->getAnimations().size();
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
					}else {
						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						animationToChange.append(*mSavedAnimation);

						printf("animation is changed by append a copy of another. we keep old name (Use rename if you want).\n");
					}
				}else {
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}else {
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "CUT") {

			if (nbArg < 4)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			size_t indexKfStart = std::stoi(arguments.at(2));
			size_t indexKfEnd = std::stoi(arguments.at(3));

			if (indexFile < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				if (indexAnim < eanFile->getAnimations().size())
				{
					LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
					animationToChange.cut(indexKfStart, indexKfEnd);

					printf("animation is cutted.\n");
				}else {
					printf("indexAnim don't exist.\n");
				}
			}else {
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}






		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "FIXEDPOSITIONCOMPONENT") {

			if (nbArg < 4)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			string boneRef = arguments.at(2);
			string component = arguments.at(3);

			if (indexFile < listEanFile.size())
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));
				
				if ((component == "X") || (component == "Y") || (component == "Z"))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					if (indexAnim < eanFile->getAnimations().size())
					{
						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						std::vector<LibXenoverse::EANAnimationNode> &nodes = animationToChange.getNodes();

						bool isfound = false;
						size_t nbBones = nodes.size();
						for (size_t i = 0; i < nbBones; i++)
						{
							if (nodes.at(i).getBoneIndex() == indexBone)
							{
								std::vector<LibXenoverse::EANKeyframedAnimation> &anims = nodes.at(i).getKeyframed_animations();

								bool isfound2 = false;
								size_t nbAnim = anims.size();
								for (size_t j = 0; j < nbAnim; j++)
								{
									if (anims.at(j).getFlag() == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION)
									{
										vector<LibXenoverse::EANKeyframe> &keyframes = anims.at(j).getKeyframes();

										float value_ToFixe = ((keyframes.size() != 0) ? ((component =="X") ? keyframes.at(0).x : ((component == "Y") ? keyframes.at(0).y : keyframes.at(0).z)) : 0.0f);

										size_t nbKf = keyframes.size();
										for (size_t k = 0; k < nbKf; k++)
										{
											if (component == "X")
												keyframes.at(k).x = value_ToFixe;
											else if (component == "Y")
												keyframes.at(k).y = value_ToFixe;
											else
												keyframes.at(k).z = value_ToFixe;
										}
										
										printf("animation is fixed.\n");
										isfound2 = true;
										break;
									}

								}
								if (!isfound2)
									printf("no 'position' anima exist.\n");

								isfound = true;
								break;
							}
						}
						if (!isfound)
							printf("indexBone don't exist.\n");
					}else {
						printf("indexAnim don't exist.\n");
					}
				}else {
					printf("component %s is not X Y or Z.\n", component);
				}
			}else {
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		



		///////////////////////////////////////////////////////////////////////////////////////////////////
		} else if (command == "MOVEPOSITIONCOMPONENT") {

			if (nbArg < 5)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			string boneRef = arguments.at(2);
			string component = arguments.at(3);
			float translationValue = std::stof(arguments.at(4));

			if (indexFile < listEanFile.size())
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));
				
				if ((component == "X") || (component == "Y") || (component == "Z"))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					if (indexAnim < eanFile->getAnimations().size())
					{
						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						std::vector<LibXenoverse::EANAnimationNode> &nodes = animationToChange.getNodes();

						bool isfound = false;
						size_t nbBones = nodes.size();
						for (size_t i = 0; i < nbBones; i++)
						{
							if (nodes.at(i).getBoneIndex() == indexBone)
							{
								std::vector<LibXenoverse::EANKeyframedAnimation> &anims = nodes.at(i).getKeyframed_animations();

								bool isfound2 = false;
								size_t nbAnim = anims.size();
								for (size_t j = 0; j < nbAnim; j++)
								{
									if (anims.at(j).getFlag() == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION)
									{
										vector<LibXenoverse::EANKeyframe> &keyframes = anims.at(j).getKeyframes();

										float lastKeyframe = (float)((keyframes.size()!=0) ? keyframes.back().frame : 0);
										float valueToAdd = 0.0f;

										size_t nbKf = keyframes.size();
										for (size_t k = 0; k < nbKf; k++)
										{
											valueToAdd = (translationValue * keyframes.at(k).frame) / lastKeyframe;
											if (component == "X")
												keyframes.at(k).x += valueToAdd;
											else if (component == "Y")
												keyframes.at(k).y += valueToAdd;
											else
												keyframes.at(k).z += valueToAdd;
										}

										printf("animation is updated for moving.\n");
										isfound2 = true;
										break;
									}

								}
								if (!isfound2)
									printf("no 'position' anima exist.\n");

								isfound = true;
								break;
							}
						}
						if (!isfound)
							printf("indexBone don't exist.\n");
					}else {
						printf("indexAnim don't exist.\n");
					}
				}else {
					printf("component %s is not X Y or Z.\n", component);
				}
			}else {
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

			
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "RESETBONEFILTER"){

			mListBoneFilters.clear();
			mListBoneFilters_eanIndex.clear();
		



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "ADDBONEFILTER"){

			if (nbArg < 1)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			arguments.erase(arguments.begin());

			if (indexFile >= listEanFile.size())
			{
				printf("Error : Wrong indexfile.\n");
				continue;
			}


			size_t nbEanFileFilter = mListBoneFilters.size();
			size_t isfound = (size_t)-1;
			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				if (mListBoneFilters_eanIndex.at(i) == indexFile)
				{
					isfound = i;
					break;
				}
			}
			if (isfound==(size_t)-1)
			{
				isfound = mListBoneFilters_eanIndex.size();
				mListBoneFilters_eanIndex.push_back(indexFile);
				mListBoneFilters.push_back(vector<size_t>());
			}

			size_t nbBoneArg = arguments.size();
			for (size_t j = 0; j < nbBoneArg; j++)
			{
				string boneRef = arguments.at(j);
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));
				if (indexBone == (size_t)-1)
					continue;

				size_t nbBone = mListBoneFilters.at(isfound).size();
				bool isfound_2 = false;
				for (size_t i = 0; i < nbEanFileFilter; i++)
				{
					if (mListBoneFilters.at(isfound).at(i) == indexBone)
					{
						isfound_2 = true;
						break;
					}
				}
				if (!isfound_2)
					mListBoneFilters.at(isfound).push_back(indexBone);
			}



		
			


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "ADDALLBONEINFILTERFOR"){

			if (nbArg < 1)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			arguments.erase(arguments.begin());


			if (indexFile >= listEanFile.size())
			{
				printf("Error : Wrong indexfile.\n");
				continue;
			}


			size_t nbEanFileFilter = mListBoneFilters.size();
			size_t isfound = (size_t)-1;
			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				if (mListBoneFilters_eanIndex.at(i) == indexFile)
				{
					isfound = i;
					break;
				}
			}

			if (isfound == (size_t)-1)
			{
				isfound = mListBoneFilters_eanIndex.size();
				mListBoneFilters_eanIndex.push_back(indexFile);
				mListBoneFilters.push_back(vector<size_t>());
			}


			if (indexFile < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

				size_t  nbBone = eanFile->getSkeleton()->getBones().size();
				size_t nbBoneArg = arguments.size();
				for (size_t k = 0; k < nbBone; k++)
				{
					bool isfound_index = false;
					for (size_t j = 0; j < nbBoneArg; j++)
					{
						string boneRef = arguments.at(j);
						size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));
						if (indexBone == (size_t)-1)
							continue;

						if (indexBone == k)
						{
							isfound_index = true;
							break;
						}
					}

					if (!isfound_index)					//could add
					{
						size_t nbBone = mListBoneFilters.at(isfound).size();
						bool isfound_2 = false;
						for (size_t i = 0; i < nbEanFileFilter; i++)
						{
							if (mListBoneFilters.at(isfound).at(i) == k)
							{
								isfound_2 = true;
								break;
							}
						}
						if (!isfound_2)
							mListBoneFilters.at(isfound).push_back(k);
					}
				}
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "ADDBONEFILTERPRESET") {

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			string indexFile_str = arguments.at(0);
			string presetName = arguments.at(1);
			
			if (presetName == "torso")
			{
				line = "AddBoneFilter " + indexFile_str + " b_C_Base b_C_Pelvis g_C_Pelvis b_C_Spine1 b_C_Spine2 b_C_Chest";
				automaticLine = true;
			}else if (presetName == "head") {
				line = "AddBoneFilter " + indexFile_str + " b_C_Neck1 b_C_Head g_C_Head f_C_FaceRoot f_L_EyeInnerCorner f_R_EyeCorner f_L_EyeSocket f_C_ToothTop f_C_Jaw f_L_MouthBottom f_C_MouthBottom f_R_MouthBottom f_C_Tongue1 f_C_Tongue2 f_C_Tongue3 f_C_Tongue4 f_C_ToothBottom f_L_EyeCorner f_L_EyeBrows1 f_L_EyeBrows2 f_L_EyeBrows3 f_L_EyeBrowsHair3 f_L_EyeBrowsHair2 f_L_EyeBrowsHair1 f_L_CheekTop f_R_Eye f_R_EyeIris f_L_Eye f_L_EyeIris f_C_JawHalf f_L_MouthCorners f_L_CheekBottom f_R_CheekBottom f_R_MouthCorners f_C_NoseTop f_C_MouthTop f_L_EyelidBottom f_L_EyelidTop f_R_CheekTop f_R_EyeInnerCorner f_R_EyeSocket f_R_EyelidBottom f_R_EyelidTop f_R_EyeBrows1 f_R_EyeBrows2 f_R_EyeBrows3 f_R_EyeBrowsHair3 f_R_EyeBrowsHair2 f_R_EyeBrowsHair1 f_R_MouthTop f_L_MouthTop";
				automaticLine = true;
			}else if (presetName == "legs") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Leg1 b_L_Leg2 b_L_Foot g_L_Foot b_L_Toe b_L_Knee b_L_LegHelper b_R_Leg1 b_R_Leg2 b_R_Foot g_R_Foot b_R_Toe b_R_Knee b_R_LegHelper";
				automaticLine = true;
			}else if (presetName == "leg_left") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Leg1 b_L_Leg2 b_L_Foot g_L_Foot b_L_Toe b_L_Knee b_L_LegHelper";
				automaticLine = true;
			}else if (presetName == "leg_right") {
				line = "AddBoneFilter " + indexFile_str + " b_R_Leg1 b_R_Leg2 b_R_Foot g_R_Foot b_R_Toe b_R_Knee b_R_LegHelper";
				automaticLine = true;
			}else if (presetName == "arms") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Shoulder b_L_Arm1 b_L_Arm2 b_L_Hand h_L_Middle1 h_L_Middle2 h_L_Middle3 g_L_Hand h_L_Pinky1 h_L_Pinky2 h_L_Pinky3 h_L_Ring1 h_L_Ring2 h_L_Ring3 h_L_Index1 h_L_Index2 h_L_Index3 h_L_Thumb1 h_L_Thumb2 h_L_Thumb3 b_L_ArmRoll b_L_Elbow b_L_ArmHelper b_L_ArmorParts b_R_Shoulder b_R_Arm1 b_R_Elbow b_R_Arm2 b_R_Hand h_R_Middle1 h_R_Middle2 h_R_Middle3 g_R_Hand h_R_Pinky1 h_R_Pinky2 h_R_Pinky3 h_R_Ring1 h_R_Ring2 h_R_Ring3 h_R_Index1 h_R_Index2 h_R_Index3 h_R_Thumb1 h_R_Thumb2 h_R_Thumb3 b_R_ArmRoll b_R_ArmHelper b_R_ArmorParts";
				automaticLine = true;
			}else if (presetName == "arm_left") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Shoulder b_L_Arm1 b_L_Arm2 b_L_Hand h_L_Middle1 h_L_Middle2 h_L_Middle3 g_L_Hand h_L_Pinky1 h_L_Pinky2 h_L_Pinky3 h_L_Ring1 h_L_Ring2 h_L_Ring3 h_L_Index1 h_L_Index2 h_L_Index3 h_L_Thumb1 h_L_Thumb2 h_L_Thumb3 b_L_ArmRoll b_L_Elbow b_L_ArmHelper b_L_ArmorParts";
				automaticLine = true;
			}else if (presetName == "arm_right") {
				line = "AddBoneFilter " + indexFile_str + " b_R_Shoulder b_R_Arm1 b_R_Elbow b_R_Arm2 b_R_Hand h_R_Middle1 h_R_Middle2 h_R_Middle3 g_R_Hand h_R_Pinky1 h_R_Pinky2 h_R_Pinky3 h_R_Ring1 h_R_Ring2 h_R_Ring3 h_R_Index1 h_R_Index2 h_R_Index3 h_R_Thumb1 h_R_Thumb2 h_R_Thumb3 b_R_ArmRoll b_R_ArmHelper b_R_ArmorParts";
				automaticLine = true;
			}else if (presetName == "hands") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Hand h_L_Middle1 h_L_Middle2 h_L_Middle3 g_L_Hand h_L_Pinky1 h_L_Pinky2 h_L_Pinky3 h_L_Ring1 h_L_Ring2 h_L_Ring3 h_L_Index1 h_L_Index2 h_L_Index3 h_L_Thumb1 h_L_Thumb2 h_L_Thumb3 b_R_Hand h_R_Middle1 h_R_Middle2 h_R_Middle3 g_R_Hand h_R_Pinky1 h_R_Pinky2 h_R_Pinky3 h_R_Ring1 h_R_Ring2 h_R_Ring3 h_R_Index1 h_R_Index2 h_R_Index3 h_R_Thumb1 h_R_Thumb2 h_R_Thumb3";
				automaticLine = true;
			}else if (presetName == "hand_left") {
				line = "AddBoneFilter " + indexFile_str + " b_L_Hand h_L_Middle1 h_L_Middle2 h_L_Middle3 g_L_Hand h_L_Pinky1 h_L_Pinky2 h_L_Pinky3 h_L_Ring1 h_L_Ring2 h_L_Ring3 h_L_Index1 h_L_Index2 h_L_Index3 h_L_Thumb1 h_L_Thumb2 h_L_Thumb3";
				automaticLine = true;
			}else if (presetName == "hand_right") {
				line = "AddBoneFilter " + indexFile_str + " b_R_Hand h_R_Middle1 h_R_Middle2 h_R_Middle3 g_R_Hand h_R_Pinky1 h_R_Pinky2 h_R_Pinky3 h_R_Ring1 h_R_Ring2 h_R_Ring3 h_R_Index1 h_R_Index2 h_R_Index3 h_R_Thumb1 h_R_Thumb2 h_R_Thumb3";
				automaticLine = true;
			}else if (presetName == "tail") {
				line = "AddBoneFilter " + indexFile_str + " X_T_TAIL1 X_T_TAIL2 X_T_TAIL3 X_T_TAIL4 X_T_TAIL5 X_T_TAIL6 X_T_TAIL7 X_T_TAIL8 X_T_TAIL9 X_T_TAIL10";
				automaticLine = true;
			}else if (presetName == "spines") {
				line = "AddBoneFilter " + indexFile_str + " x_x_backhorn x_L_backtophorn1 x_L_backtophorn2 x_R_backmiddlehorn1 x_R_backmiddlehorn2 x_R_backtophorn1 x_R_backtophorn2 x_L_backmiddlehorn1 x_L_backmiddlehorn2";
				automaticLine = true;
			}else if (presetName == "wings") {
				line = "AddBoneFilter " + indexFile_str + " Wing_bases RWing_bases RWing_A RWing_B RWing_B_outsidethe1 RWing_B_outsidethe2 RWing_B_outsidethe3 RWing_B_outsidethe4 RWing_B_middle1 RWing_B_inside1 RWing_B_inside2 RWing_A1 RWing_A2 LWing_bases LWing_A LWing_B LWing_B_outsidethe1 LWing_B_outsidethe2 LWing_B_outsidethe3 LWing_B_outsidethe4 LWing_B_middle1 LWing_B_inside1 LWing_B_inside2 LWing_A1 LWing_A2";
				automaticLine = true;
			}else if (presetName == "sword") {
				line = "AddBoneFilter " + indexFile_str + " a_x_sword1 a_x_sword2 a_x_sword3 a_x_sword4";
				automaticLine = true;
			}else if (presetName == "cane") {
				line = "AddBoneFilter " + indexFile_str + " a_x_cane_root a_x_cane_front1 a_x_cane_front2 a_x_cane_back1";
				automaticLine = true;
			}else if (presetName == "spear") {
				line = "AddBoneFilter " + indexFile_str + " a_x_spear_root a_x_spear_front1 a_x_spear_front2 a_x_spear_front3 a_x_spear_back1 a_x_spear_back2 a_x_spear_back3";
				automaticLine = true;
			}else if (presetName == "accessories") {
				line = "AddBoneFilter " + indexFile_str + " a_x_flute a_x_glasses";
				automaticLine = true;
			}else {
				printf("Unknow preset's name. try 'Help' command\n");
			}

			if (automaticLine)
				printf("%s\n", line.c_str());


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "RENAME"){

			if (nbArg < 3)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			string newName = arguments.at(2);



			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				eanFile->getAnimations().at(indexAnim).setName(newName);
				printf("Name changed.\n");
			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "ERASE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			size_t indexAnim_range_end = (size_t)-1;
			if (nbArg>2)
				indexAnim_range_end = std::stoi(arguments.at(2));


			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				if ((indexAnim_range_end != (size_t)-1) && (indexAnim_range_end>indexAnim))
				{
					for (size_t i = indexAnim_range_end; i >= indexAnim; i--)
					{
						eanFile->getAnimations().erase(eanFile->getAnimations().begin() + i);
						printf("Animation %i Erased.\n", i);
					}

				}else{
					eanFile->getAnimations().erase(eanFile->getAnimations().begin() + indexAnim);
					printf("Animation Erased.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "INSERT"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));



			if ((indexFile < listEanFile.size()) && (indexAnim <= listEanFile.at(indexFile)->getAnimations().size()))
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					if (indexAnim == eanFile->getAnimations().size())
					{
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
					}else{
						eanFile->getAnimations().insert(eanFile->getAnimations().begin() + indexAnim, (LibXenoverse::EANAnimation(mSavedAnimation, eanFile)));
					}

					printf("animation added.\n");
				}else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "ADDBONEOFFSETSCALEONANIMATIONPOSITION"){

			if (nbArg < 9)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			string boneRef = arguments.at(2);
			float offsetToAdd_X = std::stof(arguments.at(3));
			float offsetToAdd_Y = std::stof(arguments.at(4));
			float offsetToAdd_Z = std::stof(arguments.at(5));
			float scaletoMultiply_X = std::stof(arguments.at(6));
			float scaletoMultiply_Y = std::stof(arguments.at(7));
			float scaletoMultiply_Z = std::stof(arguments.at(8));


			if ((indexFile < listEanFile.size()) && ((indexAnim==-1)||(indexAnim < listEanFile.at(indexFile)->getAnimations().size())))
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));

				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				
				vector<LibXenoverse::EANAnimation> &listAnimation = eanFile->getAnimations();
				
				size_t nbAnimations = listAnimation.size();
				for (size_t m = 0; m < nbAnimations; m++)
				{
					if ((m != indexAnim) && (indexAnim != -1))
						continue;

					LibXenoverse::EANAnimation *animation = &(listAnimation.at(m));
					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();

					bool isfound = false;
					size_t nbBones = listNodes.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);
							if (keyframeAnim.getFlag() != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION)	//only work on position.
								continue;

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								LibXenoverse::EANKeyframe &eanKeyframe = listKeyFrames.at(k);
								eanKeyframe.setXYZW(eanKeyframe.x * scaletoMultiply_X + offsetToAdd_X, eanKeyframe.y * scaletoMultiply_Y + offsetToAdd_Y, eanKeyframe.z * scaletoMultiply_Z + offsetToAdd_Z, eanKeyframe.w);
							}
						}

						if (!isfound)
							printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
					}

				}

				printf("Done.\n");

			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}




		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETDURATION"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));
			string boneRef = (nbArg>2) ? arguments.at(2)  : "";


			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));

				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));
				
				if (indexBone == (size_t)-1)
				{
					printf("animation %s have %i frames (60 fps) so %f second\n", animation->getName().c_str(), animation->getFrameCount(), animation->getFrameCount() * (1.0 / 60.0));
				}else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;
						
						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);
							
							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}
						
						printf("animation %s for bone index %i have %i frames (60 fps) so %f second\n", animation->getName().c_str(), indexBone, nblastKeyframe, nblastKeyframe * (1.0 / 60.0));
						break;
					}
					if (!isfound)
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
				}
			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}





	
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "SETDURATION" || command == "SETDURATIONINFRAMES"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			double duration = 2.0;
			string boneRef = "";
			if (nbArg > 3)
			{
				boneRef = arguments.at(2);
				duration = std::stod(arguments.at(3));
			}else if (nbArg > 2){
				duration = std::stod(arguments.at(2));
			}

			struct InterpolatedKey
			{
				float x;
				float y;
				float z;
				float w;

				InterpolatedKey(float x, float y, float z, float w)
				{
					this->x = x;
					this->y = y;
					this->z = z;
					this->w = w;
				}
			};

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));

				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));

				size_t original_Duration_inFrames = (size_t)-1;
				size_t target_Duration_inFrames = (size_t)0;
				if(command == "SETDURATION") 
				{
					target_Duration_inFrames = (size_t)round(duration * 60.0);
				}else{
					target_Duration_inFrames = (size_t)round(duration);
				}
				if (target_Duration_inFrames == 0)
					target_Duration_inFrames = 1;


				if (indexBone == (size_t)-1)
				{
					original_Duration_inFrames = animation->getFrameCount();

				}else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}

						original_Duration_inFrames = nblastKeyframe;
						break;
					}
					if (!isfound)
					{
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
						continue;
					}
				}

				double factor = ((double)target_Duration_inFrames) / ((double)original_Duration_inFrames);
				printf("Ok. it's about resample %i frames (%f seconds) into %i frames (%f seconds => factor of %f on keys)\nLet's go\n", original_Duration_inFrames, ((double)original_Duration_inFrames)*(1.0 / 60.0), target_Duration_inFrames, ((double)target_Duration_inFrames)*(1.0 / 60.0), factor);

				if (indexBone == (size_t)-1)
					animation->setFrameCount(target_Duration_inFrames);

				vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
				size_t nbBones = listNodes.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((indexBone != (size_t)-1) && (listNodes.at(i).getBoneIndex() != indexBone))
						continue;


					size_t nblastKeyframe = 0;
					vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
					size_t nbKeyFramesAnim = listKeyFramesAnim.size();
					for (size_t j = 0; j < nbKeyFramesAnim; j++)
					{
						LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

						size_t flag = keyframeAnim.getFlag();



						//first, we have to create interpolation
						std::vector<InterpolatedKey> listInterpolKey;
						float x, y, z, w;
						for (size_t k = 0; k < target_Duration_inFrames; k++)
						{
							listNodes.at(i).getInterpolatedFrame((unsigned int)round(((double)k) / factor), flag, x, y, z, w);

							listInterpolKey.push_back(InterpolatedKey(x, y, z, w));
						}

						//next, we replace for the flag
						vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy
						listKeyFrames.clear();

						for (size_t k = 0; k < target_Duration_inFrames; k++)				//change the frame number with the factor
						{
							InterpolatedKey &pos = listInterpolKey.at(k);
							listKeyFrames.push_back(LibXenoverse::EANKeyframe(k, pos.x, pos.y, pos.z, pos.w));
						}
					}

					if (indexBone != (size_t)-1)
						break;
				}

				printf("Done.\n");

			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "LOOPANIMATION"){

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			size_t indexAnim = std::stoi(arguments.at(1));

			int nbLoops = 2;
			string boneRef = "";
			if (nbArg > 3)
			{
				boneRef = arguments.at(2);
				nbLoops = std::stoi(arguments.at(3));
			}else if (nbArg > 2){
				nbLoops = std::stoi(arguments.at(2));
			}

			if (nbLoops <= 1)
			{
				printf("minimum loop parameter is 1. skipped\n");
				continue;
			}


			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				size_t indexBone = (isNumber(boneRef) ? std::stoi(boneRef) : ((listEanFile.at(indexFile)->getSkeleton()) ? listEanFile.at(indexFile)->getSkeleton()->getBoneIndex(boneRef) : (size_t)-1));
				
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));

				size_t original_Duration_inFrames = (size_t)-1;
				
				if (indexBone == (size_t)-1)
				{
					original_Duration_inFrames = animation->getFrameCount();

				}else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}

						original_Duration_inFrames = nblastKeyframe + 1;
						break;
					}
					if (!isfound)
					{
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
						continue;
					}
				}



				size_t target_Duration_inFrames = original_Duration_inFrames * nbLoops;
				if (target_Duration_inFrames == 0)
					target_Duration_inFrames = 1;

				
				if (target_Duration_inFrames>animation->getFrameCount())
					animation->setFrameCount(target_Duration_inFrames);

				vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
				size_t nbBones = listNodes.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((indexBone != (size_t)-1) && (listNodes.at(i).getBoneIndex() != indexBone))
						continue;


					size_t nblastKeyframe = 0;
					vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
					size_t nbKeyFramesAnim = listKeyFramesAnim.size();
					for (size_t j = 0; j < nbKeyFramesAnim; j++)
					{
						LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

						size_t flag = keyframeAnim.getFlag();


						//next, we replace for the flag
						vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy

						size_t currentNbFrames = listKeyFrames.size();
						size_t targetNbFrames = currentNbFrames * nbLoops;
						size_t currentLoops = 0;

						for (size_t k = currentNbFrames; k < targetNbFrames; k++)				//change the frame number with the factor
						{
							LibXenoverse::EANKeyframe &originalFrame = listKeyFrames.at(k % currentNbFrames);							
							currentLoops = (size_t)floor(k / currentNbFrames);
							listKeyFrames.push_back(LibXenoverse::EANKeyframe(originalFrame.getFrame() + currentLoops * original_Duration_inFrames, originalFrame.x, originalFrame.y, originalFrame.z, originalFrame.w));
						}
					}

					if (indexBone != (size_t)-1)
						break;
				}

				printf("Done.\n");

			}else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "MATCHANIMATIONDURATION"){
			
			if (nbArg < 4)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile_src = std::stoi(arguments.at(0));
			size_t indexFile_dest = std::stoi(arguments.at(1));
			size_t indexAnim_start = std::stoi(arguments.at(2));
			size_t indexAnim_end = std::stoi(arguments.at(3));



			if (indexAnim_end < indexAnim_start)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			struct InterpolatedKey
			{
				float x;
				float y;
				float z;
				float w;

				InterpolatedKey(float x, float y, float z, float w)
				{
					this->x = x;
					this->y = y;
					this->z = z;
					this->w = w;
				}
			};

			
			if ((indexFile_src < listEanFile.size()) && (indexFile_dest < listEanFile.size()) && (indexAnim_start < listEanFile.at(indexFile_dest)->getAnimations().size()) && (indexAnim_end < listEanFile.at(indexFile_dest)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile_src = listEanFile.at(indexFile_src);
				LibXenoverse::EAN *eanFile_dest = listEanFile.at(indexFile_dest);


				for (size_t animIndex = indexAnim_start; animIndex <= indexAnim_end; animIndex++)
				{
					LibXenoverse::EANAnimation *animation = &(eanFile_dest->getAnimations().at(animIndex));
					string animationName = animation->getName();
					printf("******* animation %i : %s\n", animIndex, animationName.c_str());
					
					
					string animationName_withoutChara = animationName;					//we can't match animation by name because of chara part : "APL_xxx", so we skip the first part.
					if (animationName_withoutChara.find('_') != std::string::npos)
						animationName_withoutChara = animationName_withoutChara.substr(animationName_withoutChara.find('_'));
					
					// first: find duration
					// as we don't have filter on bones, we can take the frame number directly.
					vector<LibXenoverse::EANAnimation> &listSrcAnima = eanFile_src->getAnimations();
					LibXenoverse::EANAnimation *animation_src = 0;

					string animationName_withoutChara_b = "";
					size_t nbSrcAnim = listSrcAnima.size();
					for (size_t i = 0; i < nbSrcAnim; i++)
					{
						animationName_withoutChara_b = listSrcAnima.at(i).getName();
						if (animationName_withoutChara_b.find('_') != std::string::npos)
							animationName_withoutChara_b = animationName_withoutChara_b.substr(animationName_withoutChara_b.find('_'));

						if (animationName_withoutChara_b == animationName_withoutChara)
						{
							animation_src = &(listSrcAnima.at(i));
							break;
						}
					}

					if (animation_src == 0)
					{
						printf("Warning there isn't animation named %s in Ean_scr. skipped\n", animation->getName().c_str());
						continue;
					}
					else{
						printf("animation %s is found in Ean_scr and have %i frames (60 fps) so %f second\n", animation_src->getName().c_str(), animation_src->getFrameCount(), animation_src->getFrameCount() * (1.0 / 60.0));
					}
					

					
					



					size_t original_Duration_inFrames = animation->getFrameCount();			//same : no filter on bones, so is faster.
					size_t target_Duration_inFrames = animation_src->getFrameCount();
					if (target_Duration_inFrames == 0)
						target_Duration_inFrames = 1;
					if (original_Duration_inFrames == 0)
						original_Duration_inFrames = 1;
					
					double factor = ((double)target_Duration_inFrames) / ((double)original_Duration_inFrames);
					printf("Ok. it's about resample %i frames (%f seconds) into %i frames (%f seconds => factor of %f on keys)\nLet's go\n", original_Duration_inFrames, ((double)original_Duration_inFrames)*(1.0 / 60.0), target_Duration_inFrames, ((double)target_Duration_inFrames)*(1.0 / 60.0), factor);

					animation->setFrameCount(target_Duration_inFrames);

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							size_t flag = keyframeAnim.getFlag();



							//first: we hae to create interpolation
							std::vector<InterpolatedKey> listInterpolKey;
							float x, y, z, w;
							for (size_t k = 0; k < target_Duration_inFrames; k++)
							{
								listNodes.at(i).getInterpolatedFrame((unsigned int)round(((double)k) / factor), flag, x, y, z, w);

								listInterpolKey.push_back(InterpolatedKey(x, y, z, w));
							}

							//next, we replace for the flag
							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy
							listKeyFrames.clear();

							for (size_t k = 0; k < target_Duration_inFrames; k++)				//change the frame number with the factor
							{
								InterpolatedKey &pos = listInterpolKey.at(k);
								listKeyFrames.push_back(LibXenoverse::EANKeyframe(k, pos.x, pos.y, pos.z, pos.w));
							}
						}
					}


				}
				printf("Done.\n");

			}else{
				printf("index %i or %i is not in ean file list or indexAnim %i or %i is not in list animations.\n", indexAnim_start, indexAnim_end, indexFile_src, indexFile_dest);
			}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else{
			printf("Unknow command *%s*.\n Type 'Help' to see valid commands\n", command.c_str());
		}
	}
	

	printf("Ok. goodbye. See you next time.\n");
	return 0;
}