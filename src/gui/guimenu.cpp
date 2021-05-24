#include "guimenu.h"

GuiMenu::GuiMenu(){
    // Data
    dropdownBoxFileActive = 0;
    dropdownFileEditMode = false;

    // Text
    buttonFileText = (char*) malloc(128*sizeof(char));
    buttonExamplesText = (char*) malloc(128*sizeof(char));
    buttonHelpText = (char*) malloc(128*sizeof(char));
    fileName = (char*) malloc(2048*sizeof(char));

    strcpy(buttonFileText, "File;Open file;Save;Save as...;Exit");
    strcpy(buttonExamplesText, "Examples");
    strcpy(buttonHelpText, "Help");
    strcpy(fileName, "\0");

    // Define controls rectangles
    layoutRecsButtons = vector<Rectangle>(5);
    layoutRecsButtons[REC_BUTTON_FILE] = (Rectangle){0, 0, 50, 25};    // Button: buttonOpen
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){50, 0, 80, 25};    // Button: buttonExamples
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){130, 0, 50, 25};    // Button: buttonHelp
    
    layoutRecsMenu = (Rectangle){0, 0, 1130, 25};    // Line: LineMenu
}

int GuiMenu::readQIFFile(char* prior[NUMBER_SECRETS], vector<vector<char*>> &channel){
    // /* File format:
    // -----BEGIN OF FILE-----
    // prior
    // p1 p2 p3
    // channel
    // m
    // p11 p12 ... p1m
    // p21 p22 ... p2m
    // p31 pn2 ... pnm
    // -----END OF FILE-----

    // where n is the number of secrets (consequently the # elements in prior and # rows in channel),
    // m is the number of outputs in the channel (# of columns) and every two numbers are separated
    // by a white space.
    // */

    // FILE *file = popen("zenity --file-selection --title=Open --file-filter=*.qifg", "r");
    // fgets(fileName, 2048, file);
    // fclose(file);

    // // Remove \n from the end
    // string newFileName = string(fileName);
    // newFileName = newFileName.substr(0, newFileName.find_last_of("\n"));
    // strcpy(fileName, newFileName.c_str());

    // ifstream infile;
    // int numOutputs;
    // string buffer, probBuffer;
    // try{
    //     infile.open(fileName); 
        
    //     // Prior
    //     getline(infile, buffer);
    //     if(buffer != "prior") throw exception();

    //     vector<char*> newPrior(3, nullptr);
    //     for(int i = 0; i < 3; i++){
    //         newPrior[i] = (char*) malloc(128*sizeof(char));
    //         infile >> probBuffer;
    //         strcpy(newPrior[i], probBuffer.c_str());
    //     }
        
    //     // Channel
    //     getline(infile, buffer);        // Skip \n
    //     getline(infile, buffer);
    //     if(buffer != "channel") throw exception();

    //     infile >> numOutputs;
        
    //     vector<vector<char*>> newChannel = vector<vector<char*>>(numOutputs, vector<char*>(3, nullptr));
    //     for(int i = 0; i < numOutputs; i++){
    //         for(int j = 0; j < 3; j++){
    //             newChannel[i][j] = (char*) malloc(128*sizeof(char));
    //             strcpy(newChannel[i][j], "\0");
    //         }
    //     }
        
    //     getline(infile, buffer);        // Skip \n
    //     for(int j = 0; j < 3; j++){
    //         for(int i = 0; i < numOutputs; i++){
    //             infile >> probBuffer;
    //             strcpy(newChannel[i][j], probBuffer.c_str());
    //         }
    //         getline(infile, buffer);        // Skip \n
    //     }
        
    //     infile.close();

    //     // If no exception was thrown update prior and channel vectors        
    //     prior = newPrior;
    //     channel = newChannel;
    // }catch(const exception& e){
    //     cerr << "Invalid file: " << e.what() << '\n';        
    //     infile.close();
    //     return INVALID_QIF_FILE;
    // }

    return NO_ERROR;
}

void GuiMenu::saveQIFFile(char* prior[NUMBER_SECRETS], vector<vector<char*>> &channel, bool createNewFile){
    /* File format:
    -----BEGIN OF FILE-----
    prior
    p1 p2 p3
    channel
    m
    p11 p12 ... p1m
    p21 p22 ... p2m
    p31 pn2 ... pnm
    -----END OF FILE-----

    where n is the number of secrets (consequently the # elements in prior and # rows in channel),
    m is the number of outputs in the channel (# of columns) and every two numbers are separated
    by a white space.
    */

    // if(createNewFile){
    //     FILE *file = popen("zenity --file-selection --title=Save --save --filename=untitled.qifg --confirm-overwrite", "r");
    //     fgets(fileName, 2048, file);
    //     fclose(file);

    //     if(strcmp(fileName, "\0")){
    //         // Remove \n from the end
    //         string newFileName = string(fileName);
    //         newFileName = newFileName.substr(0, newFileName.find_last_of("\n"));
    //         strcpy(fileName, newFileName.c_str());

    //         // Fix file extension if it is not .qifg
    //         string fn = string(fileName);
    //         if(fn.find_last_of(".") == string::npos){
    //             string newFileName = fn + ".qifg";
    //             strcpy(fileName, newFileName.c_str());
    //         }else if(fn.substr(fn.find_last_of(".") + 1) != "qifg"){
    //             string newFileName = fn.substr(0, fn.find_last_of(".")) + ".qifg";
    //             strcpy(fileName, newFileName.c_str());
    //         }
    //     }
    // }

    // if(strcmp(fileName, "\0")){
    //     string output = "";
    //     output = output + "prior\n" + prior[0] + " " + prior[1] + " " + prior[2] + "\n";
    //     output = output + "channel\n" + to_string(channel.size()) + "\n";
    //     for(int j = 0; j < 3; j++){
    //         long unsigned int i = 0;
    //         while(i < channel.size()-1){
    //             output = output + channel[i][j] + " ";
    //             i++;
    //         }
    //         output = output + channel[i][j] + "\n";
    //     }

    //     ofstream outfile;
    //     outfile.open(fileName);
    //     outfile << output;
    //     outfile.close();
    // }
}