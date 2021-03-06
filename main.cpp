/*
 * Hack Assembler
 * Created by Francois W. Nel on 27 Jun 2016.
 *
 * Description:
 *  The assembler converts Hack assembly instructions
 *  into machine code for the Hack computer architecture.
 *  Assembly instructions should be provided in a .asm source file,
 *  and the machine code will be stored in a .hack file
 *  with either the same filename, or using the provided
 *  filename and extension.
 *
 * Usage:
 *  $ chasm <inputfilename.asm> <(optional) outputfilename.hack>
 *
 * Limitations:
 *  Please ensure that there is a blank line at the end
 *  of your .asm source file.
 */

#include <iostream>
#include <fstream>
#include <bitset>
#include "Parser.h"
#include "CodeTranslator.h"
#include "SymbolTable.h"
#include "Parser.cpp"
#include "CodeTranslator.cpp"
#include "SymbolTable.cpp"
#include "ErrorHandler.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    string inputFileName, outputFileName;
    int lineNumberROM, newAddress;
    unsigned long lineNumberSource;
    ofstream fout;
    ErrorHandler handler;
    // Get the input and output file names, and provide usage instructions
    //  if too few or too many arguments are provided.

        cout << "enter input file name" << "\n";
        cin >> inputFileName;
        outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + ".hack";



    // Create the output file
    fout.open(outputFileName);

    if (fout.fail()) {
        cout << "Failed to create output file." << endl;
        exit(1);
    }

    /*
     * First pass: Generate symbol table
     *  This is done by reading the input file line by line using the symbolSource parser,
     *  and looking for pseudo instructions. If an A-instruction or C-instruction is encountered,
     *  lineNumberROM is incremented, which ultimately corresponds to the program counter address
     *  of the instructions in the ROM / .hack output file. If an L-instruction is encountered
     *  and it does not already exist in the symbol table, the symbol and the current
     *  program counter address is stored in the symbol table.
     */

    Parser symbolSource(inputFileName);
    SymbolTable symbolTable;

    lineNumberSource = 0;
    lineNumberROM = 0;
    while (true) {
        symbolSource.advance(lineNumberSource);

        if (!symbolSource.hasMoreCommands()) {
            break;
        }

        if (symbolSource.commandType(lineNumberSource) != 'A' && symbolSource.commandType(lineNumberSource) != 'C' && symbolSource.commandType(lineNumberSource) != 'L') {
            handler.WriteErrors(9,lineNumberSource);
        }
        else if (symbolSource.commandType(lineNumberSource) == 'A' || symbolSource.commandType(lineNumberSource) == 'C') {
            lineNumberROM++;
        }

        if (symbolTable.contains(symbolSource.symbol()) == true && symbolSource.commandType(lineNumberSource) == 'L'){
          handler.WriteErrors(6,lineNumberSource);
        }

        if (symbolSource.commandType(lineNumberSource) == 'L' && !symbolTable.contains(symbolSource.symbol())) {

            if (symbolSource.getFirstChar() == '.') {
                
                symbolTable.addEQUEntry(symbolSource.symbol(), symbolSource.getEQUValue());
            }

            else {
                symbolTable.addEntry(symbolSource.symbol(), lineNumberROM);
            }
        }
        
            
    }

    /*
     * Second pass: Assemble machine code
     *  With the symbol table generated, we read the file line by line once more,
     *  this time using the assemblySource parser. Since the L-instructions have been dealt with,
     *  we are only looking for A-instructions and C-instructions.
     *
     *  If an A-instruction is encountered, the address can either be a number,
     *  a predefined symbol, or a user defined variable. If the address is a number,
     *  the string is converted to its numeric representation in decimal,
     *  which is then converted to binary and output to the file.
     *  If the address is a symbol, the corresponding address is either
     *  retrieved if it is a predefined symbol, or generated if it is a user defined variable,
     *  and this address is then converted into binary and output to the file.
     *
     *  If a C-instruction is encountered, the destination, computation and jump mnemonics
     *  are converted into binary code using the translator, and the resulting bit string
     *  is output to the file.
     */

    Parser assemblySource(inputFileName);
    CodeTranslator translator;

    lineNumberSource = 0;   // Reset the line number for the error handling.
    newAddress = 16;        // Predefined symbols occupy addresses 0-15.

    while (true) {
        assemblySource.advance(lineNumberSource);

        if (!assemblySource.hasMoreCommands()) {
            break;
        }

        if (assemblySource.commandType(lineNumberSource) == 'A') {
            fout << '0';    // A-instructions always start with '0'.

            // Check if the symbol is a number.
            if (assemblySource.symbol().find_first_not_of("0123456789") == string::npos ) 
            {
                // Convert the string to a decimal number, convert the decimal number to a binary number.
                if (assemblySource.getBitSet() == " "){
                  handler.WriteErrors(8,lineNumberSource);
                }
                else
                  fout << bitset<15>(stoull(assemblySource.getBitSet(), nullptr)).to_string();
            }
            else if(assemblySource.symbol().find_first_not_of("0123456789bBxX") == string::npos)
            {

                fout << bitset<15>(stoull(assemblySource.getBitSet(),nullptr)).to_string();
            }
            else {
                // Check if the symbol is a variable.
                if (!symbolTable.contains(assemblySource.symbol())) {
                    symbolTable.addEntry(assemblySource.symbol(), newAddress++);
                }
                // Retrieve the address, and convert the decimal number to a binary number.
                fout << bitset<15>(static_cast<unsigned long long>(symbolTable.getAddress(assemblySource.symbol()))).to_string();
            }
            fout << endl;
        }
        else if (assemblySource.commandType(lineNumberSource) == 'C') {
            fout << "111";  // C-instructions always start with "111".

            if (translator.comp(assemblySource.compM(), lineNumberSource) == " ") {
                  handler.WriteErrors(3,lineNumberSource);
            }
            fout << translator.comp(assemblySource.compM(), lineNumberSource);

            if (translator.dest(assemblySource.destM(), lineNumberSource) == " ") {
                  handler.WriteErrors(2,lineNumberSource);
            }
            fout << translator.dest(assemblySource.destM(), lineNumberSource);
           
            if (translator.jump(assemblySource.jumpM(), lineNumberSource) == " ") {
                handler.WriteErrors(4,lineNumberSource);
            }
            fout << translator.jump(assemblySource.jumpM(), lineNumberSource);
            fout << endl;
        }
    }
    symbolTable.PrintTable();
    fout.close();
    return 0;
}
