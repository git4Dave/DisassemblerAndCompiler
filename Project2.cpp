//============================================================================
// Author      : David Machin
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <sstream>
#include <stdbool.h>
using namespace std;

void parseFileContents(string fileLine);
void categoryOneInstruction(string opcode, string file);
void categoryTwoInstruction(string opcode, string file);
void categoryThreeInstruction(string opcode, string file);
long long int binaryToDecimal(string number);
long long int twosComplement32(string file);
int twosComplement16(string file);
string stringConversion(int value);
void simulation();

ofstream writeFile;
ofstream simulationWrite;
int pcAddress = 60;
//vector<long long int> dataStorage(250);
//vector<int> PCstorage(250);
//vector<string> outputs(250);
int dataPCIncrement = 0, outputIncrement = 0;
map<int, string> outputStorage;
map<int, long long int> dataStorage;

int main() {
	  string fileContents, fileName;
	  cin>>fileName;
	  ifstream textFile;
	  textFile.open(fileName.c_str());
	  writeFile.open("disassembly.txt");
	  if (textFile.is_open()){ //Think about removing this later as not needed
	    while(getline(textFile,fileContents)){
	    	cout << fileContents << '\n';
	    	parseFileContents(fileContents);
	    }
	  }
	  textFile.close();
	  writeFile.close();

	 simulationWrite.open("simulation.txt");
	 simulation();
	 simulationWrite.close();

	return 0;
}

void parseFileContents(string fileLine){
	string categoryString, opcodeString;
	long long int value;
	for(int iterate = 0; iterate<6; iterate++){
		while(iterate < 3){
			categoryString.push_back(fileLine[iterate]);
			iterate++;
		}
		if(iterate == 3 && categoryString == "001"){
			while(iterate<6){
				opcodeString.push_back(fileLine[iterate]);
				iterate++;
			}
			categoryOneInstruction(opcodeString, fileLine);
		}
		else if(categoryString == "010" && iterate == 3){
			while(iterate<6){
				opcodeString.push_back(fileLine[iterate]);
				iterate++;
			}
			categoryTwoInstruction(opcodeString, fileLine);
		}
		else if(categoryString == "100" && iterate == 3){
			while(iterate<6){
				opcodeString.push_back(fileLine[iterate]);
				iterate++;
			}
			categoryThreeInstruction(opcodeString, fileLine);
		}
		else{
			pcAddress = pcAddress + 4;
			value = twosComplement32(fileLine);
			writeFile<<fileLine<<"\t"<<pcAddress<<"\t"<<value<<"\n";
			dataStorage[pcAddress] = value;
			//dataStorage[dataPCIncrement] = value;
			//PCstorage[dataPCIncrement] = pcAddress;
			//dataPCIncrement++;
			//cout<<"This is " << dataPCIncrement<<endl;
			iterate++;
			break;
		}
		categoryString = "";
		opcodeString = "";
	}
}

void categoryOneInstruction(string opcode, string file){
	string instructionName = "", targetS = "", pcAddressS = "", jInstruction = "", binaryNumber = "", sourceRegister1 = "", sourceRegister2 = "", offset = "", outputtedString = "";
	int offsetValue = 0, registerNum = 0, registerNum2 = 0, baseNum = 0, target = 0;
	string register1 = "R", registerNumS = "", registerNum2S = "", offsetValueS = "";
	string immediateValue = "#";
	string base = "R";
	unsigned int sizee = file.length();
	for(unsigned int i = 6; i < sizee;i++){
		if(opcode == "000"){
			instructionName = "NOP";
			pcAddress = pcAddress + 4;
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<"\n";
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + "\n";
			//outputs[outputIncrement] = outputtedString;
			//outputIncrement++;
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "001"){
			instructionName = "J";
			pcAddress = pcAddress + 4;
			//binaryNumber = decimalToBinary(pcAddress);
			while(i<sizee){ //LOOK into target
				jInstruction.push_back(file[i]);
				i++;
			}
			jInstruction.erase(0,2);
			jInstruction.push_back('0');
			jInstruction.push_back('0');
			//target.append(binaryToDecimal(jInstruction));
			target = binaryToDecimal(jInstruction);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<immediateValue<<target<<"\n";
			//cout<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<immediateValue<<target<<"\n";
			targetS = stringConversion(target);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + immediateValue + targetS + "\n";
			//outputs[outputIncrement] = outputtedString;
			//outputIncrement++;
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "010"){
			instructionName = "BEQ";
			pcAddress = pcAddress + 4;
			while(i<sizee){
				if(i < 11){
					sourceRegister1.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					sourceRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 16){
					offset.push_back(file[i]);
					i++;
				}
			}
			registerNum = binaryToDecimal(sourceRegister1);
			registerNum2 = binaryToDecimal(sourceRegister2);
			offsetValue = twosComplement16(offset);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<register1<<registerNum<<", "<<register1<<registerNum2<<", "<<immediateValue<<offsetValue<<"\n";
			registerNumS = stringConversion(registerNum);
			registerNum2S = stringConversion(registerNum2);
			offsetValueS = stringConversion(offsetValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + register1 + registerNumS + ", " + register1 + registerNum2S + ", " +  immediateValue + offsetValueS + "\n";
			//outputs[outputIncrement] = outputtedString;
			//outputIncrement++;
			outputStorage[pcAddress] = outputtedString;

			// register1.append(registerNum);
			// register2.append(registerNum2);
			// immediateValue.append(offsetValue);
			break;
		}
		else if(opcode == "011"){
			instructionName = "BNE";
				pcAddress = pcAddress + 4;
				while(i<sizee){
					if(i < 11){
						sourceRegister1.push_back(file[i]);
						i++;
					}
					else if( i >= 11 && i < 16){
						sourceRegister2.push_back(file[i]);
						i++;
					}
					else if(i >= 16){
						offset.push_back(file[i]);
						i++;
					}
				}
				registerNum = binaryToDecimal(sourceRegister1);
				registerNum2 = binaryToDecimal(sourceRegister2);
				offsetValue = twosComplement16(offset);
				writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<register1<<registerNum<<", "<<register1<<registerNum2<<", "<<immediateValue<<offsetValue<<"\n";
				registerNumS = stringConversion(registerNum);
				registerNum2S = stringConversion(registerNum2);
				offsetValueS = stringConversion(offsetValue);
				pcAddressS = stringConversion(pcAddress);
				outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + register1 + registerNumS + ", " + register1 + registerNum2S + ", " +  immediateValue + offsetValueS + "\n";
				outputStorage[pcAddress] = outputtedString;
				break;
		}
		else if(opcode == "100"){
			instructionName = "BGTZ";
				pcAddress = pcAddress + 4;
				while(i<sizee){
					if(i < 11){
						sourceRegister1.push_back(file[i]);
						i++;
					}
					else if( i >= 11 && i < 16){
						sourceRegister2.push_back('0');
						i++;
					}
					else if(i >= 16){
						offset.push_back(file[i]);
						i++;
					}
				}
				registerNum = binaryToDecimal(sourceRegister1);
				registerNum2 = binaryToDecimal(sourceRegister2);
				offsetValue = twosComplement16(offset);
				writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<register1<<registerNum<<", "<<immediateValue<<offsetValue<<"\n";
				registerNumS = stringConversion(registerNum);
				registerNum2S = stringConversion(registerNum2);
				offsetValueS = stringConversion(offsetValue);
				pcAddressS = stringConversion(pcAddress);
				outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + register1 + registerNumS + ", " + register1 + registerNum2S + ", " +  immediateValue + offsetValueS + "\n";
				outputStorage[pcAddress] = outputtedString;
				break;
		}
		else if(opcode == "101"){
			instructionName = "SW";
			pcAddress = pcAddress + 4;
			while(i<file.length()){
				if(i < 11){
					base.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					sourceRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16){
					offset.push_back(file[i]);
					i++;
				}
			}
			baseNum = binaryToDecimal(base);
			registerNum = binaryToDecimal(sourceRegister1);
			offsetValue = twosComplement16(offset);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<register1<<registerNum<<", "<<offsetValue<<"("<<register1<<baseNum<<")"<<"\n";
			registerNumS = stringConversion(registerNum);
			registerNum2S = stringConversion(registerNum2);
			offsetValueS = stringConversion(offsetValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + register1 + registerNumS + ", " + register1 + registerNum2S + ", " +  immediateValue + offsetValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			// register1.append(registerNum);
			// immediateValue.append(offsetValue);
			break;
		}
		else if(opcode == "110"){
			instructionName = "LW";
			pcAddress = pcAddress + 4;
			while(i<sizee){
				if(i < 11){
					base.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
						sourceRegister1.push_back(file[i]);
						i++;
				}
				else if(i >= 16){
						offset.push_back(file[i]);
						i++;
				}
			}
			baseNum = binaryToDecimal(base);
			registerNum = binaryToDecimal(sourceRegister1);
			offsetValue = twosComplement16(offset);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<" "<<register1<<registerNum<<", "<<offsetValue<<"("<<register1<<baseNum<<")"<<"\n";
			registerNumS = stringConversion(registerNum);
			registerNum2S = stringConversion(registerNum2);
			offsetValueS = stringConversion(offsetValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + " " + register1 + registerNumS + ", " + register1 + registerNum2S + ", " +  immediateValue + offsetValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "111"){ //not sure whether anything else if needed for break
			instructionName = "BREAK";
			pcAddress = pcAddress + 4;
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName<<"\n";
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName + "\n";
			outputStorage[pcAddress] = outputtedString;
		}
	}
}

void categoryTwoInstruction(string opcode, string file){
	string instructionName2 = "", srcRegister1 = "", srcRegister2 = "", pcAddressS = "", destinationRegister = "", outputtedString = "", reg1ContentsS = "";
	string sourceReg1 = "R", reg2ContentsS = "", destContentsS = "";
	int destContents = 0, reg1Contents = 0, reg2Contents = 0;
	unsigned int len = file.length();
	for(unsigned int i = 6; i < len;i++){
		if(opcode == "000"){
			instructionName2 = "XOR";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "001"){
			instructionName2 = "MUL";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "010"){
			instructionName2 = "ADD";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "011"){
			instructionName2 = "SUB";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "100"){
			instructionName2 = "AND";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "101"){
			instructionName2 = "OR";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "110"){
			instructionName2 = "ADDU";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		else if(opcode == "111"){
			instructionName2 = "SUBU";
			pcAddress = pcAddress + 4;
			while(i<len){
				if(i < 11){
					destinationRegister.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					srcRegister1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 && i < 21){
					srcRegister2.push_back(file[i]);
					i++;
				}
				else if(i >= 21){ //Not Sure if these bits will be needed

				}
			}
			destContents = binaryToDecimal(destinationRegister);
			reg1Contents = binaryToDecimal(srcRegister1);
			reg2Contents = binaryToDecimal(srcRegister2);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName2<<" "<<sourceReg1<<destContents<<", "<<sourceReg1<<reg1Contents<<", "<<sourceReg1<<reg2Contents<<"\n";
			destContentsS = stringConversion(destContents);
			reg1ContentsS = stringConversion(reg1Contents);
			reg2ContentsS = stringConversion(reg2Contents);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName2 + " " + sourceReg1 + destContentsS + ", " + sourceReg1 + reg1ContentsS + ", " + sourceReg1 + reg2ContentsS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
	}
}

void categoryThreeInstruction(string opcode, string file){
	string instructionName3 = "", src1 = "", destinationRegister2 = "", pcAddressS = "", iVal = "", outputtedString = "";
	string srcReg1 = "R", destReg2 = "R", immValue = "#", destContents2S = "", reg1Contents2S = "", immediateValueS = "";
	int destContents2 = 0, reg1Contents2 = 0, immediateValue = 0;
	unsigned int lengthh = file.length();
	for(unsigned int i = 6; i < lengthh;i++){
		if(opcode == "000"){ //NOT sure if remaining bits are needed
			instructionName3 = "ORI";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "001"){ //NOT sure if remaining bits are needed
			instructionName3 = "XORI";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "010"){ //NOT sure if remaining bits are needed
			instructionName3 = "ADDI";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 16 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = twosComplement16(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "011"){ //NOT sure if remaining bits are needed
			instructionName3 = "SUBI";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 27 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = twosComplement16(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "100"){ //NOT sure if remaining bits are needed
			instructionName3 = "ANDI";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 16){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "101"){ //NOT sure if remaining bits are needed
			instructionName3 = "SRL";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 27 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "110"){ //NOT sure if remaining bits are needed
			instructionName3 = "SRA";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 27 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString = "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
		if(opcode == "111"){ //NOT sure if remaining bits are needed
			instructionName3 = "SLL";
			pcAddress = pcAddress + 4;
			while(i<lengthh){
				if(i < 11){
					destinationRegister2.push_back(file[i]);
					i++;
				}
				else if( i >= 11 && i < 16){
					src1.push_back(file[i]);
					i++;
				}
				else if(i >= 27 ){
					iVal.push_back(file[i]);
					i++;
				}
			}
			destContents2 = binaryToDecimal(destinationRegister2);
			reg1Contents2 = binaryToDecimal(src1);
			immediateValue = binaryToDecimal(iVal);
			writeFile<<file<<"\t"<<pcAddress<<"\t"<<instructionName3<<" "<<srcReg1<<destContents2<<", "<<srcReg1<<reg1Contents2<<", "<<immValue<<immediateValue<<"\n";
			destContents2S = stringConversion(destContents2);
			reg1Contents2S = stringConversion(reg1Contents2);
			immediateValueS = stringConversion(immediateValue);
			pcAddressS = stringConversion(pcAddress);
			outputtedString =  "\t" + pcAddressS + "\t" + instructionName3 + " " + srcReg1 + destContents2S + ", " + srcReg1 + reg1Contents2S + ", " + immValue + immediateValueS + "\n";
			outputStorage[pcAddress] = outputtedString;
			break;
		}
	}
}

string stringConversion(int value){
	stringstream stream;
    stream << value;
    return stream.str();
}
void simulation(){
	/*int register0 = 0, register1 = 0, register2 = 0, register3 = 0, register4 = 0, register5 = 0, register6 = 0, register7 = 0, register8 = 0, register9 = 0;
	int register10 = 0, register11 = 0, register12 = 0, register13 = 0, register14 = 0, register15 = 0, register16 = 0, register17 = 0, register18 = 0;
	int register19 = 0, register20 = 0, register21 = 0, register22 = 0, register23 = 0, register24 = 0, register25 = 0, register26 = 0, register27 = 0;
	int register28 = 0, register29 = 0, register30 = 0, register31 = 0;*/
	map<int, string> :: iterator it;
	map<int, long long int> :: iterator it2;
	int registers[32];

	for(int i = 0; i < 32; i++){
		registers[i] = 0;
	}

	string currentString = "";
	int t = 1, temp = 0, offsetValue, baseValue, regNumber;
	for(it = outputStorage.begin(); it != outputStorage.end(); it++){
		currentString = outputStorage[it->first];
		bool comma = false, parenthesis = false;
		string valueString = "";
		if(currentString.find("NOP") != string::npos){
			cout<<"--------------------"<<"\n";
			cout<<"Cycle: "<<t<<outputStorage[it->first]<<"\n";
			cout<<"Registers"<<"\n";
			cout<<"R00:"<<"\t"<<registers[0]<<"\t"<<registers[1]<<"\t"<<registers[2]<<"\t"<<registers[3]<<"\t"<<registers[4]<<"\t"<<registers[5]<<"\t"<<registers[6]<<"\t"<<registers[7]<<"\n";
			cout<<"R08:"<<"\t"<<registers[8]<<"\t"<<registers[9]<<"\t"<<registers[10]<<"\t"<<registers[11]<<"\t"<<registers[12]<<"\t"<<registers[13]<<"\t"<<registers[14]<<"\t"<<registers[15]<<"\n";
			cout<<"R16:"<<"\t"<<registers[16]<<"\t"<<registers[17]<<"\t"<<registers[18]<<"\t"<<registers[19]<<"\t"<<registers[20]<<"\t"<<registers[21]<<"\t"<<registers[22]<<"\t"<<registers[23]<<"\n";
			cout<<"R24:"<<"\t"<<registers[24]<<"\t"<<registers[25]<<"\t"<<registers[26]<<"\t"<<registers[27]<<"\t"<<registers[28]<<"\t"<<registers[29]<<"\t"<<registers[30]<<"\t"<<registers[31]<<"\n";


			simulationWrite<<"--------------------"<<"\n";
			simulationWrite<<"Cycle: "<<t<<outputStorage[it->first]<<"\n";
			simulationWrite<<"Registers"<<"\n";
			simulationWrite<<"R00:"<<"\t"<<registers[0]<<"\t"<<registers[1]<<"\t"<<registers[2]<<"\t"<<registers[3]<<"\t"<<registers[4]<<"\t"<<registers[5]<<"\t"<<registers[6]<<"\t"<<registers[7]<<"\n";
			simulationWrite<<"R08:"<<"\t"<<registers[8]<<"\t"<<registers[9]<<"\t"<<registers[10]<<"\t"<<registers[11]<<"\t"<<registers[12]<<"\t"<<registers[13]<<"\t"<<registers[14]<<"\t"<<registers[15]<<"\n";
			simulationWrite<<"R16:"<<"\t"<<registers[16]<<"\t"<<registers[17]<<"\t"<<registers[18]<<"\t"<<registers[19]<<"\t"<<registers[20]<<"\t"<<registers[21]<<"\t"<<registers[22]<<"\t"<<registers[23]<<"\n";
			simulationWrite<<"R24:"<<"\t"<<registers[24]<<"\t"<<registers[25]<<"\t"<<registers[26]<<"\t"<<registers[27]<<"\t"<<registers[28]<<"\t"<<registers[29]<<"\t"<<registers[30]<<"\t"<<registers[31]<<"\n";
			simulationWrite<<"\n";
			t++;
		}
		else if(currentString.find("J ") != string::npos){
			cout<<"--------------------"<<"\n";
			cout<<"Cycle "<<t<<"\t"<<outputStorage[it->first]<<"\n";
			t++;
		}
		else if(currentString.find("LW ") != string::npos){
			temp = 0;
			while(temp < currentString.length()){
				temp++;
				if(currentString[temp] == 'R' && comma == false){
					while(isdigit(currentString[temp + 1])){
						valueString.push_back(currentString[temp + 1]);
						temp++;
					}
					regNumber = atoi(valueString.c_str());
					valueString = "";
				}
				if(currentString[temp] == ','){
					comma = true;
					temp++;
				}
				if(comma == true && isdigit(currentString[temp]) && parenthesis == false){
					while(isdigit(currentString[temp])){
						valueString.push_back(currentString[temp + 1]);
						temp++;
					}
					offsetValue = atoi(valueString.c_str());
					valueString = "";
					temp--;
				}
				if(comma == true && currentString[temp] == '('){
					temp++;
					parenthesis = true;
				}
				if(parenthesis == true && currentString[temp] == 'R'){
					while(isdigit(currentString[temp + 1])){
						valueString.push_back(currentString[temp + 1]);
						temp++;
					}
					baseValue = atoi(valueString.c_str());
					valueString = "";
					temp--;
					break;
				}
			}
			registers[regNumber] = registers[baseValue] + dataStorage[offsetValue];
			cout<<"--------------------"<<"\n";
			cout<<"Cycle "<<t<<"\t"<<outputStorage[it->first]<<"\n";
			t++;
		}

		int count = 0;
		bool access = false;
		for(it2 = dataStorage.begin();it2 != dataStorage.end(); it2++){
			if(it2 -> first == dataStorage.begin() -> first){
				simulationWrite<<"Data";
				simulationWrite<<"\n";
				simulationWrite<<it2->first<<":"<<"\t";
			}
			if(count + 1 % 8 == 0){
				simulationWrite<<dataStorage[it2->first];
				count++;
				access = false;
			}
			else if(count % 8 != 0  || access == true || count == 0){
				simulationWrite<<dataStorage[it2->first]<<"\t";
				access = false;
				count++;
			}
			else if(count % 8 == 0){
				simulationWrite<<"\n";
				simulationWrite<<it2->first<<":"<<"\t";
				simulationWrite<<dataStorage[it2->first]<<"\t";
				count++;
				access = true;
			}
		}

	}

}
long long int binaryToDecimal(string number){
	double num = 0;
	long long int num2 = 0;
	//string finalValue;
	unsigned int size = number.length();
	for(unsigned int k = 0; k < size; k++){
		if(number[k] == '1'){
			num = num + pow(2, size - (1+k));
		}
	}
	num2 = num;
	//finalValue = to_string(num2);
	return num2;
}

long long int twosComplement32(string file){
	double number = binaryToDecimal(file);
	long long int number2;
	cout<<number<<endl;
	if(number > 2147483647){
		cout<<"h";
		number2 = number - 4294967296;
		return number2;
	}
	else{
		cout<<"b";
		number2 = number;
		return number2;

	}
}

int twosComplement16(string file){
	int number = binaryToDecimal(file);
	if(number > 32767){
		return number - 65536;
	}
	else{
		return number;
	}
}
//string decimalToBinary(int address){
//	string binaryNumber;
//	int result;
//	    if(address <= 1) {
//	        cout << address;
//	        return -2;
//	    }
//	    result = address % 2;
//	    decimalToBinary(address >> 1);
//	    binaryNumber.push_back((char) result);
//	    return binaryNumber;
//}
