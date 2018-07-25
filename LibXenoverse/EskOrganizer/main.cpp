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

	while (c != '\n')
	{
		c = getchar();
		if (c != '\n'){
			line += string((char*)&c);
		}
		else{
			line += string("\0");
		}
	}
	return line;
}

int main(int argc, char** argv)
{

	LibXenoverse::initializeDebuggingLog();

	printf("Welcome to ESK Organizer v0.6.3kx\n\n");


	string help = "Commands:\n\
	<Required Argument> [Optional Argument]\n\n\
	'Help [Command]' e.g. 'Help Load' for help with Load command\n\
	'Load <path\\filenameESKFile> [file2.ext] ...'\n\
	'Save <indexESK> <path\\filename>'\n\
	'GetESKFileList'\n\
	'GetBoneList <indexESK> [useTree]'\n\
	'Rename <indexESK> <indexBone> <newName> [renameAlsoIntoEMDFile]'\n\
	'Remove <indexESK> <indexBone> [recursive]'\n\
	'Copy <indexESK> <indexBone> [recursive]'\n\
	'Paste <indexESK> <indexBone> [alsoCopyAnimation] [matchDuration]'\n\
	'CalculTransformMatrixFromSkinningMatrix <indexESK> <indexBone>'\n\
	'ClearTransformMatrix <indexESK> <indexBone>'\n\
	'CalculSkinningMatrixFromTransformMatrix <indexESK> <indexBone>'\n\
	'ClearSkinningMatrix <indexESK> <indexBone>'\n\
	'getJson <indexESK>'\n\
	'GetBonePosition <indexESK> <indexBone> [absolute]'\n\
	'GetBoneOrientation <indexESK> <indexBone> [absolute]'\n\
	'GetBoneScale <indexESK> <indexBone> [absolute]'\n\
	'SetBonePosition <indexESK> <indexBone> <posX> <posY> <posZ> [absolute]'\n\
	'SetBoneOrientation <indexESK> <indexBone> <quatX> <quatY> <quatZ> <quatW> [absolute]'\n\
	'SetBoneScale <indexESK> <indexBone> <scaleX> <scaleY> <scaleZ> [absolute]'\n\
	'Merge <indexESK_src> <indexESK_dest>'\n\
	'EMD_Load <path\\filenameEMDFile> [<path\\filenameEMDFile2> <path\\filenameEMDFile3> ...]'\n\
	'EMD_Save <indexEMD> <path\\filename>'\n\
	'EMD_GetEMDFileList'\n\
	'EMD_GetBoneList <indexEMD>'\n\
	'EMD_Rename <indexEMD> <oldBoneName> <newBoneName>'\n\
	'Quit'\n";
	
	//'GetBoneRotation <indexESK> <indexBone> <absolute>'\n\
	//'SetBoneRotation <indexESK> <indexBone> <RotY> <RotZ> <RotX> <absolute>'\n\

	std::map<string, string> helptext;
	helptext["HELP"] = "Help [Command]\nList all commands\n\n";
	helptext["LOAD"] = "Load <path\\filenameESKFile> [file2.ext] ...\nLoad a skeleton from an ESK or EAN file\n\n";
	helptext["SAVE"] = "Save <indexESK> <path\\filename>\nSave your modified ESK or EAN file\n\n";
	helptext["GETESKFILELIST"] = "GetESKFileList\nGet list of loaded ESK files\n\n";
	helptext["GETBONELIST"] = "GetBoneList <indexESK> [useTree]\nGet list of bones within a specified ESK file, use the argument 'true' to show parent-child relations\n\n";	
	helptext["RENAME"] = "Rename <indexESK> <indexBone> <newName> [renameAlsoIntoEMDFile]\nRename a specified bone, use [renameAlsoIntoEMDFile] to rename this in the EMF dile as well\n\n";
	helptext["REMOVE"] = "Remove <indexESK> <indexBone> [recursive]\nDelete a specified bone, also delete children if [recursive] is true\n\n";
	helptext["COPY"] = "Copy <indexESK> <indexBone> [recursive]\nCopy on the specified bone to clipboard, also copy children if [recursive] is true\n\n";
	helptext["PASTE"] = "Paste <indexESK> <indexBone> [alsoCopyAnimation] [matchDuration]\nPaste clipboard bone OVER destination bone, set [alsoCopyAnimation] and/or [matchDuration] arguments to true/false if desired\n\n";
	helptext["CALCULTRANSFORMMATRIXFROMSKINNINGMATRIX"] = "CalculTransformMatrixFromSkinningMatrix <indexESK> <indexBone>\nConvert the skinningMatrix to transformMatrix\nEAN have internal skeletons without transformMatrix, so this function is used to create an ESK from an EAN\nif indexESK==-1 it will edit all bones\n\n";
	helptext["CLEARTRANSFORMMATRIX"] = "ClearTransformMatrix <indexESK> <indexBone>\nClear the transformMatrix\n\n";
	helptext["CALCULSKINNINGMATRIXFROMTRANSFORMMATRIX"] = "CalculSkinningMatrixFromTransformMatrix <indexESK> <indexBone>\nConvert the transformMatrix to skinningMatrix\nEAN have internal skeletons without transformMatrix, so this function is used to create an EAN from an ESK\nif indexESK==-1 it will edit all bones\n\n";
	helptext["CLEARSKINNINGMATRIX"] = "ClearSkinningMatrix <indexESK> <indexBone>\nClear the skinningMatrix\n\n";
	helptext["GETJSON"] = "getJson <indexESK>\nFor test comparisons\n\n";
	helptext["GETBONEPOSITION"] = "GetBonePosition <indexESK> <indexBone> [absolute]\nGet the bone position as absolute (from transformMatrix) if [absolute] is true, or as releative (from skinningMatrix) if false\n\n";
	helptext["GETBONEORIENTATION"] = "GetBoneOrientation <indexESK> <indexBone> [absolute]\nGet the bone quaternion orientation, absolute if true (else relative)\n\n";
	helptext["GETBONESCALE"] = "GetBoneScale <indexESK> <indexBone> [absolute]\nGet the bone position, absolute if true (else relative)\n\n";
	helptext["SETBONEPOSITION"] = "SetBonePosition <indexESK> <indexBone> <posX> <posY> <posZ> [absolute]\nSet bone position, absolute if true (else relative)\n\n";
	helptext["SETBONESCALE"] = "SetBoneScale <indexESK> <indexBone> <scaleX> <scaleY> <scaleZ> [absolute]\nSet bone scale, absolute if true (else relative)\n\n";
	helptext["MERGE"] = "Merge <indexESK_src> <indexESK_dest>\nMerge two ESK files, similar to EMDFBX process\n\n";
	helptext["EMD_LOAD"] = "EMD_Load <path\\filenameEMDFile> [<path\\filenameEMDFile2> <path\\filenameEMDFile3> ...]\nLoad EMF file(/s)\n\n";
	helptext["EMD_SAVE"] = "EMD_Save <indexEMD> <path\\filename>\nSave your modified EMD file\n\n";
	helptext["EMD_GETEMDFILELIST"] = "EMD_GetEMDFileList\nGet list of loaded EMD files\n\n";
	helptext["EMD_GETBONELIST"] = "EMD_GetBoneList <indexEMD>\nGet list of bones within a specified EMD file\n\n";
	helptext["EMD_RENAME"] = "EMD_Rename <indexEMD> <oldBoneName> <newBoneName>\nRename specified bone within EMD file\n\n";
	helptext["QUIT"] = "Quit\nExit the program\n\n";	
	//helptext["GETBONEROTATION"] = "GetBoneRotation <indexESK> <indexBone> [absolute]\nGet the bone taitBryan angles (know also as EulerAngle, with the diference to have this order : RotY (Yaw) -> RotZ (pitch) -> RotX (Roll) like a plane) information of the orientation, absolute if true (else relative)\n\n";
	//helptext["SETBONEROTATION"] = "SetBoneRotation <indexESK> <indexBone> <RotY> <RotZ> <RotX> [absolute]\nSet rotation of specified bone\n\n";
	printf((string("You can load ESK, EAN, and EMD file and list their bones.\nDon't rename c_Base or root bone. Keep unique names at the end!\n\n") + help +"\n").c_str());

	struct EanOrEskFile
	{
		LibXenoverse::EAN* mEan;
		LibXenoverse::ESK* mEsk;
		string mFilename;

		EanOrEskFile(string filename)
		{
			mEan = NULL;
			mEsk = NULL;
			mFilename = filename;
		}
	};

	struct EskOrganizer_Copy
	{
		LibXenoverse::EskTreeNode* mTreeNode;
		LibXenoverse::EskTreeNode* mRootToClean;
		EanOrEskFile* mSourceFile;

		EskOrganizer_Copy(LibXenoverse::EskTreeNode* treeNode, EanOrEskFile* sourceFile, LibXenoverse::EskTreeNode* rootToClean)
		{
			mTreeNode = treeNode;
			mRootToClean = rootToClean;
			mSourceFile = sourceFile;
		}
	};

	EskOrganizer_Copy eskOrganizer_Copy(nullptr, nullptr, nullptr);
	vector<EanOrEskFile*> listEskFile;
	vector<LibXenoverse::EMD*> listEmdFile;


	string line = "";
	std::vector<string> arguments;
	std::vector<string> arg_tmp;



	while ((arguments.size() == 0) || (arguments.at(0) != "Quit"))
	{
		printf("So, what do you want to do next?\n");
		line = readLine();
		



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
				}
				else {
					str2_tmp = str_tmp.substr(1, str_tmp.length() - 2);
					arguments.push_back(str2_tmp);
					str2_tmp = "";
				}
				continue;
			}else if (str2_tmp.length() != 0) {

				str2_tmp += " " + str_tmp;

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
					printf("\n\nCommand doesn't exist. Use 'Help' command for the full list of commands\n");
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


				if (extension == "ean")
				{
					EanOrEskFile *eanOrEskFile = new EanOrEskFile(filename);
					eanOrEskFile->mEan = new LibXenoverse::EAN();
					if (eanOrEskFile->mEan->load(filename))
					{
						listEskFile.push_back(eanOrEskFile);
						eanOrEskFile->mEsk = eanOrEskFile->mEan->getSkeleton();
						printf("EAN Loaded\n");
					}else {
						printf("Error: Failed to load %s\n", filename.c_str());
						delete eanOrEskFile;
					}

				}else if (extension == "esk") {

					EanOrEskFile *eanOrEskFile = new EanOrEskFile(filename);
					eanOrEskFile->mEsk = new LibXenoverse::ESK();
					if (eanOrEskFile->mEsk->load(filename))
					{
						listEskFile.push_back(eanOrEskFile);
						printf("ESK Loaded\n");
					}else {
						printf("Error: Failed to load %s\n", filename.c_str());
						delete eanOrEskFile;
					}

				}else {
					printf("%s is not an EAN or ESK file\n", filename.c_str());
				}
			}
				



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "SAVE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help Save'\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			string filename = arguments.at(1);
			string extension = LibXenoverse::extensionFromFilename(filename, true);

			if (index < listEskFile.size())
			{
				if ((listEskFile.at(index)->mEan) && (extension == "ean"))
				{
					listEskFile.at(index)->mEan->save(filename);
					printf("EAN file saved at %s\n", filename.c_str());

				}else if (extension == "esk") {
					listEskFile.at(index)->mEsk->save(filename);
					printf("ESK file saved at %s\n", filename.c_str());

				}else{
					printf("Extension is not correct (.ean or .esk): %s\n", filename.c_str());
				}
			}else{
				printf("There is no ESK at index %i\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETESKFILELIST"){

			size_t nbFile = listEskFile.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listEskFile.at(i)->mFilename.c_str());





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETBONELIST"){

			if (nbArg < 1)
			{
				printf("Missing argument <indexESK>\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			bool useTree = (nbArg > 1) ? (arguments.at(1) == "true") : false;
				


			if (index < listEskFile.size())
			{
				LibXenoverse::ESK *esk = listEskFile.at(index)->mEsk;

				if (esk)
				{
					std::vector<LibXenoverse::ESKBone*> &bones = esk->getBones();
					size_t nbBone = bones.size();

					if (useTree == false)												//just a list of index
					{
						for (size_t i = 0; i < nbBone; i++)
							printf("%i : %s\n", i, bones.at(i)->getName().c_str());
					}else{
						LibXenoverse::EskTreeNode *treeNode = esk->getTreeOrganisation();
						printf("%s\n", treeNode->getDisplayStr().c_str());
					}
				}else{
					printf("No skeleton definition in this file\n");
				}
			}else{
				printf("There is no EAN at index %i\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "RENAME"){

			if (nbArg < 3)
			{
				printf("Missing arguments. Try 'Help Rename'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			string newName = arguments.at(2);
			bool renameEmd = (nbArg > 3) ? (arguments.at(3) == "true") : false;
				

			if(indexFile < listEskFile.size())
			{				
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));
				
				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					string oldName = listEskFile.at(indexFile)->mEsk->getBones().at(boneIndex)->getName();
					
					listEskFile.at(indexFile)->mEsk->getBones().at(boneIndex)->setName(newName);
					printf("Name changed\n");

					if (renameEmd)
					{
						size_t nbEmdFile = listEmdFile.size();
						for (size_t i = 0; i < nbEmdFile; i++)
							listEmdFile.at(i)->replaceBonesNames(oldName, newName);

						printf("EMD bone also renamed\n");
					}
				}else{
					printf("There is no bone at index %i\n", boneIndex);
				}
			}else{
				printf("There is no file at index %i\n", indexFile);
			}
		

		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "REMOVE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help Remove'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool recursive = (nbArg>2) ? (arguments.at(2) == "true") : true;
				


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<std::vector<size_t>> listMovingindex = listEskFile.at(indexFile)->mEsk->remove(boneIndex, recursive);

					//if we have a ean, we have to deal with offset on bone's index.
					if (listEskFile.at(indexFile)->mEan)
						listEskFile.at(indexFile)->mEan->changeBoneIndexes(listMovingindex);

					printf("Done.\nATTENTION: index of bone might have changed\n");
				}else{
					printf("There is no bone at index %i\n", boneIndex);
				}
			}else{
				printf("There is no file at index %i\n", indexFile);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "COPY"){

			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help Copy'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool recursive = (nbArg>2) ? (arguments.at(2) == "true") : true;
				


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					if (eskOrganizer_Copy.mRootToClean != nullptr)
					{
						delete eskOrganizer_Copy.mRootToClean;

						eskOrganizer_Copy.mRootToClean = nullptr;
						eskOrganizer_Copy.mTreeNode = nullptr;
						eskOrganizer_Copy.mSourceFile = nullptr;
					}

					LibXenoverse::EskTreeNode* rootNode = listEskFile.at(indexFile)->mEsk->getTreeOrganisation();					
					eskOrganizer_Copy.mSourceFile = listEskFile.at(indexFile);
					eskOrganizer_Copy.mTreeNode = rootNode->getBoneWithIndex(boneIndex);
					eskOrganizer_Copy.mRootToClean = rootNode;

					printf("Copied\n");
				}else{
					printf("There is no bone at index %i\n", boneIndex);
				}
			}else{
				printf("There is no file at index %i\n", indexFile);
			}






		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "PASTE"){

			if (eskOrganizer_Copy.mRootToClean == nullptr)
			{
				printf("Nothing Copied. please use 'Copy' command before. try 'Help' command\n");
				continue;
			}

			if (nbArg < 2)
			{
				printf("Missing arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool copyAnimation = (nbArg>2) ? (arguments.at(2) == "true") : true;
			bool matchDuration = (nbArg>3) ? (arguments.at(3) == "true") : true;


			

			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<std::vector<std::vector<size_t>>> listMovingindex = listEskFile.at(indexFile)->mEsk->addTree(eskOrganizer_Copy.mTreeNode, boneIndex);

					std::vector<std::vector<size_t>> &listNewBonesAdded = listMovingindex.at(0);
					std::vector<std::vector<size_t>> &listMovingindex_forPrevious = listMovingindex.at(1);
					
					
					if (listEskFile.at(indexFile)->mEan)
					{
						//if the destination and source file are ean, and if copyAnimation, we have to do it with help of listNewBonesAdded
						if ((eskOrganizer_Copy.mSourceFile->mEan) && (copyAnimation))
							listEskFile.at(indexFile)->mEan->addBoneAnimationFromAnotherEan(eskOrganizer_Copy.mSourceFile->mEan, listNewBonesAdded, matchDuration);


						//if we have a Ean, we have to deal with offset on bone's index.
						listEskFile.at(indexFile)->mEan->changeBoneIndexes(listMovingindex_forPrevious);
					}

					printf("Pasted.\nATTENTION: index of bone could have changed\n");
				}else{
					printf("There is no bone at index %i\n", boneIndex);
				}
			}else{
				printf("There is no file at index %i\n", indexFile);
			}







		///////////////////////////////////////////////////////////////////////////////////////////////////	
		}else if ((command == "CLEARTRANSFORMMATRIX") || (command == "CLEARSKINNINGMATRIX")){

			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help'\n");
				continue;
			}
			bool isSkinningMatrix = (command == "CLEARSKINNINGMATRIX");
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);



			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((boneIndex != (size_t)-1) && (boneIndex != i))
						continue;

					bone = bones.at(i);

					if (!isSkinningMatrix)
					{
						//matrix4x4 iddentity
						bone->transform_matrix[0] = 1; bone->transform_matrix[1] = 0; bone->transform_matrix[2] = 0; bone->transform_matrix[3] = 0;
						bone->transform_matrix[4] = 0; bone->transform_matrix[5] = 1; bone->transform_matrix[6] = 0; bone->transform_matrix[7] = 0;
						bone->transform_matrix[8] = 0; bone->transform_matrix[9] = 0; bone->transform_matrix[10] = 1; bone->transform_matrix[11] = 0;
						bone->transform_matrix[12] = 0; bone->transform_matrix[13] = 0; bone->transform_matrix[14] = 0; bone->transform_matrix[15] = 1;
						bone->haveTransformMatrix = false;
					}else{
						//matrix3x4 iddentity
						bone->skinning_matrix[0] = 0; bone->skinning_matrix[1] = 0; bone->skinning_matrix[2] = 0; bone->skinning_matrix[3] = 1;
						bone->skinning_matrix[4] = 1; bone->skinning_matrix[5] = 0; bone->skinning_matrix[6] = 0; bone->skinning_matrix[7] = 0;
						bone->skinning_matrix[8] = 1; bone->skinning_matrix[9] = 1; bone->skinning_matrix[10] = 1; bone->skinning_matrix[11] = 1;
					}
					if (boneIndex != (size_t)-1)
						break;
				}

				printf("Done\n");
			}else{
				printf("There is no file at index %i\n", indexFile);
			}





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if ((command == "CALCULTRANSFORMMATRIXFROMSKINNINGMATRIX") || (command == "CALCULSKINNINGMATRIXFROMTRANSFORMMATRIX")){

			
			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help'\n");
				continue;
			}
			bool isSkinningMatrix = (command == "CALCULSKINNINGMATRIXFROMTRANSFORMMATRIX");
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);

			

			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((boneIndex != (size_t)-1) && (boneIndex != i))
						continue;

					bone = bones.at(i);

					if (!isSkinningMatrix)
						bone->calculTransformMatrixFromSkinningMatrix(bones, true);
					else
						bone->calculSkinningMatrixFromTransformMatrix(bones, true);

					if (boneIndex != (size_t)-1)
						break;
				}

				printf("Done\n");
			}
			else{
				printf("There is no file at index %i\n", indexFile);
			}



		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if ((command == "GETBONEPOSITION") || (command == "SETBONEPOSITION")
			|| (command == "GETBONEORIENTATION") || (command == "SETBONEORIENTATION")
			|| (command == "GETBONEROTATION") || (command == "SETBONEROTATION")
			|| (command == "GETBONESCALE") || (command == "SETBONESCALE")
			){

			size_t nbMinimumArg = 1;
			if      (command == "SETBONEPOSITION")		nbMinimumArg = 5;
			else if (command == "GETBONEORIENTATION")	nbMinimumArg = 2;
			else if (command == "SETBONEORIENTATION")	nbMinimumArg = 6;
			else if (command == "GETBONEROTATION")		nbMinimumArg = 2;
			else if (command == "SETBONEROTATION")		nbMinimumArg = 5;
			else if (command == "GETBONESCALE")			nbMinimumArg = 2;
			else if (command == "SETBONESCALE")			nbMinimumArg = 5;

			if (nbArg < nbMinimumArg)
			{
				printf("Missing arguments. Try 'Help'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);

			std::vector<float> vector;
			if (nbMinimumArg >= 5)
			{
				vector.push_back(std::stof(arguments.at(2)));
				vector.push_back(std::stof(arguments.at(3)));
				vector.push_back(std::stof(arguments.at(4)));
				if (nbMinimumArg >= 6)
					vector.push_back(std::stof(arguments.at(5)));
			}

			bool absolute = (nbArg > nbMinimumArg) ? (arguments.at(nbMinimumArg) == "true") : false;
			

			


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
					LibXenoverse::ESKBone* bone = nullptr;

					size_t nbBones = bones.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						if(boneIndex != i)
							continue;

						bone = bones.at(i);

						float skinning_matrix[12];
						if (!absolute)
						{
							for (size_t i = 0; i < 12; i++)
								skinning_matrix[i] = bone->skinning_matrix[i];
						}else{

							double resultTransformMatrix[16];

							for (size_t i = 0; i < 16; i++)
								resultTransformMatrix[i] = bone->transform_matrix[i];

							double tmpTransformMatrix[16];
							LibXenoverse::ESKBone::inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);

							LibXenoverse::ESKBone::transpose4x4(&tmpTransformMatrix[0]);		//come back to Ogre matrice way
							double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
							LibXenoverse::ESKBone::decomposition4x4(&tmpTransformMatrix[0], &skinning_matrix_b[0]);

							for (size_t i = 0; i < 12; i++)
								skinning_matrix[i] = (float)skinning_matrix_b[i];
						}

						bool isASet = false;
						if (command == "GETBONEPOSITION")
						{
							printf("Position : %f, %f, %f\n", skinning_matrix[0], skinning_matrix[1], skinning_matrix[2]);
						}else if (command == "GETBONEORIENTATION"){
							printf("Orientation (xyzw): %f, %f, %f, %f\n", skinning_matrix[4], skinning_matrix[5], skinning_matrix[6], skinning_matrix[7]);
						}else if (command == "GETBONEROTATION"){
							printf("TODO. sorry\n");
						}else if (command == "GETBONESCALE"){
							printf("Scale : %f, %f, %f\n", skinning_matrix[8], skinning_matrix[9], skinning_matrix[10]);
						}else if (command == "SETBONEPOSITION"){
							skinning_matrix[0] = vector.at(0);
							skinning_matrix[1] = vector.at(1);
							skinning_matrix[2] = vector.at(2);
							isASet = true;
						}else if (command == "SETBONEORIENTATION"){
							skinning_matrix[4] = vector.at(0);
							skinning_matrix[5] = vector.at(1);
							skinning_matrix[6] = vector.at(2);
							skinning_matrix[7] = vector.at(3);
							isASet = true;
						}else if (command == "SETBONEROTATION"){
							printf("TODO. sorry\n");
						}else if (command == "SETBONESCALE"){
							skinning_matrix[8] = vector.at(0);
							skinning_matrix[9] = vector.at(1);
							skinning_matrix[10] = vector.at(2);
							isASet = true;
						}

						if (isASet)
						{
							if (!absolute)
							{
								for (size_t i = 0; i < 12; i++)
									bone->skinning_matrix[i] = skinning_matrix[i];

								printf("Done. Please consider using 'CalculTransformMatrixFromSkinningMatrix %i -1' to update TransformMatrix\n", indexFile);
							}else{

								double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
								for (size_t i = 0; i < 12; i++)
									skinning_matrix_b[i] = skinning_matrix[i];


								double resultTransformMatrix[16];
								LibXenoverse::ESKBone::makeTransform4x4(&skinning_matrix_b[0], &resultTransformMatrix[0]);

								LibXenoverse::ESKBone::transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way

								double tmpTransformMatrix[16];
								LibXenoverse::ESKBone::inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);

								for (size_t i = 0; i < 16; i++)
									bone->transform_matrix[i] = (float)tmpTransformMatrix[i];

								printf("Done. Please consider using 'CalculSkinningMatrixFromTransformMatrix %i -1' to update SkinningMatrix\n", indexFile);
							}
						}
						break;
					}
				}else{
					printf("There is no bone at index %i\n", boneIndex);
				}
			}else{
				printf("There is no file at index %i\n", indexFile);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GETJSON"){

			if (nbArg < 1)
			{
				printf("Missing arguments. Try 'Help getJson'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));

			if (indexFile < listEskFile.size())
				printf("\n\n%s\n\n", listEskFile.at(indexFile)->mEsk->getTreeOrganisation()->getJsonStr().c_str());
			else
				printf("There is no file at index %i\n", indexFile);





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "TEST"){

			if (nbArg < 1)
			{
				printf("Missing arguments. Try 'Help'\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));


			if (indexFile < listEskFile.size())
			{
				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					bone = bones.at(i);
					bone->test_calculTransformMatrixFromSkinningMatrix(bones, false);
				}

				printf("Done\n");
			}else{
				printf("There is no file at index %i\n", indexFile);
			}

		


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "MERGE"){

			if (nbArg < 2)
			{
				printf("Missing arguments. Try 'Help Merge'\n");
				continue;
			}
			size_t indexFile_src = std::stoi(arguments.at(0));
			size_t indexFile_dest = std::stoi(arguments.at(1));


			if ((indexFile_src < listEskFile.size()) && (indexFile_dest < listEskFile.size()))
			{
				if ((listEskFile.at(indexFile_dest)->mEan) && (listEskFile.at(indexFile_src)->mEan))
				{
					listEskFile.at(indexFile_dest)->mEan->merge(listEskFile.at(indexFile_src)->mEan);
				}else{
					listEskFile.at(indexFile_dest)->mEsk->merge(listEskFile.at(indexFile_src)->mEsk);
				}

				printf("Done\n");
			}else{
				if ((indexFile_src < listEskFile.size()) && (indexFile_dest < listEskFile.size()))
					printf("indexFile_src %i or  %i is not in file list\n", indexFile_src, indexFile_dest);
			}
			



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_LOAD"){


			for (size_t i = 0; i < nbArg; i++)
			{
				string filename = arguments.at(i);
				string extension = LibXenoverse::extensionFromFilename(filename, true);

				if (extension != "emd")
				{
					printf("%s not a EMD file\n", filename.c_str());
					continue;
				}

				LibXenoverse::EMD* emd = new LibXenoverse::EMD();
				if (emd->load(filename))
				{
					listEmdFile.push_back(emd);
					printf("File %s loaded\n", filename.c_str());
				}else{
					printf("Error: fail on loading %s\n", filename.c_str());
					delete emd;
				}
			}
			


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_SAVE"){

			if (arguments.at(0) == "")
			{
				arguments.erase(arguments.begin());
				nbArg--;
			}
			size_t index = (nbArg>0) ? std::stoi(arguments.at(0)) : (size_t)-1;


			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{	
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					string filename = listEmdFile.at(i)->getName();

					if (index == (size_t)-1)
					{
						filename = filename + "_modified.emd";
					}else{
						if (nbArg>1)
							filename = arguments.at(1);
					}

					listEmdFile.at(i)->save(filename);
					printf("file saved at %s\n", filename.c_str());

					if (index != (size_t)-1)
						break;
				}
			}else{
				printf("There is no EMD at index %i\n", index);
			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_GETEMDFILELIST"){

			size_t nbFile = listEmdFile.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listEmdFile.at(i)->getName().c_str());




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_GETBONELIST"){

			
			if (arguments.at(0) == "")
			{
				arguments.erase(arguments.begin());
				nbArg--;
			}
			size_t index = (nbArg>0) ? std::stoi(arguments.at(0)) : (size_t)-1;


			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					std::vector<string> listBones = listEmdFile.at(i)->getBonesNames();

					printf("*** file %s :\n", listEmdFile.at(i)->getName().c_str());

					size_t nbBones = listBones.size();
					for (size_t i = 0; i < nbBones; i++)
						printf("%s\n", listBones.at(i).c_str());

					if (index != (size_t)-1)
						break;
				}
			}else{
				printf("There is no EMD at index %i\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_RENAME"){

			if (nbArg < 3)
			{
				printf("Missing arguments. Try 'Help Rename'\n");
				continue;
			}
			size_t index = (arguments.size()>0) ? std::stoi(arguments.at(0)) : (size_t)-1;
			string oldName = arguments.at(1);
			string newName = arguments.at(2);

			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					listEmdFile.at(i)->replaceBonesNames(oldName, newName);

					if (index != (size_t)-1)
						break;
				}

				printf("Done\n");
			}else{
				printf("There is no EMD at index %i\n", index);
			}

			/*
			//TODO 'Rename <indexEsk> <indexBone> <newName> <renameAlsoIntoEmdFile>' notice you could use the name of the bone for indexBone. if you use renameAlsoIntoEmdFile at true, it will also rename bone in all Emd File loaded.\n\
			*/			



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else{
			printf("Unknown command *%s*.\n Type 'Help' for a list of valid commands\n", line.c_str());
		}
	}
	

	printf("Goodbye. See you soon!\n");
	return 0;
}
