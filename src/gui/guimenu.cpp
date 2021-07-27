#include "guimenu.h"

GuiMenu::GuiMenu(){

}

GuiMenu::GuiMenu(int windowWidth, int windowHeight){
    // Data
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    dropdownBoxFileActive = 0;
    dropdownFileEditMode = false;
    dropdownBoxHelpActive = 0;
    dropdownHelpEditMode = false;
    dropdownBoxExamplesActive = 0;
    dropdownExamplesEditMode = false;
    windowGettingStartedActive = false;

    // Text
    strcpy(buttonFileText, "File;Open file;Save;Save as...;Exit");
    strcpy(buttonExamplesText, "Examples;Load channel that leaks everything;Load channel that leaks nothing");
    strcpy(buttonHelpText, "Help;Getting Started;About");
    
    fileName = (char*) malloc(10*CHAR_BUFFER_SIZE*sizeof(char));
    strcpy(fileName, "\0");

    // Define controls rectangles
#if !defined(PLATFORM_WEB)
    layoutRecsButtons[REC_BUTTON_FILE] = (Rectangle){0, 0, 50, 25};
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){50, 0, 80, 25};
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){130, 0, 50, 25};
#else
    layoutRecsButtons[REC_BUTTON_EXAMPLES] = (Rectangle){0, 0, 80, 25};
    layoutRecsButtons[REC_BUTTON_HELP] = (Rectangle){80, 0, 50, 25};
#endif
    
    layoutRecsMenu = (Rectangle){0, 0, 1130, 25};
    layoutRecsGettingStarted = (Rectangle){(float)(windowWidth*0.1), (float)(windowHeight*0.1), (float)(windowWidth*0.8), (float)(windowHeight*0.8)};

    // Getting started
    int windowsStatusBarHeight = 22;
    layoutRecsGettingStartedMenu = (Rectangle){(float)(layoutRecsGettingStarted.x+10), (float)(layoutRecsGettingStarted.y+10+windowsStatusBarHeight), (float)(layoutRecsGettingStarted.width*0.2), (float)(layoutRecsGettingStarted.height-windowsStatusBarHeight-20)};
    layoutRecsGettingStartedPanel = (Rectangle){(float)(layoutRecsGettingStartedMenu.x+layoutRecsGettingStartedMenu.width+10), (float)(layoutRecsGettingStarted.y+10+windowsStatusBarHeight), (float)(layoutRecsGettingStarted.width*0.8-30), (float)(layoutRecsGettingStarted.height-windowsStatusBarHeight-20)};
    
    strcpy(gettingStartedMenuOptions, "Prior distribution;Channel;Hyper-distribution;Visualization;Refinement");
    gettingStartedMenuScrollIndex = 0;
    gettingStartedMenuActive = -1;
    
    // GS description texts
    strcpy(gsDescriptionTexts[GS_PRIOR], "Prior distribution on the set of secrets X = {X1,X2,X3}.");
    strcpy(gsDescriptionTexts[GS_CHANNEL], "A channel is a system that takes as input a secret Xi, whose possible values come from a finite set X, and whose only observable behavior is to produce an output Yi, whose possible values come from a finite set Y.");

    imgPadding[GS_PRIOR] = 50;
    imgPadding[GS_CHANNEL] = 110;

    loadGSImages();
}

int GuiMenu::readQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int* newNumOutputs){
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
    Parameter newNumOutputs returns the number of outputs read from file
    */

#if !defined(PLATFORM_WEB)
    FILE *file = popen("zenity --file-selection --title=Open --file-filter=*.qifg", "r");
    fgets(fileName, 2048, file);
    fclose(file);

    // Remove \n from the end
    string newFileName = string(fileName);
    newFileName = newFileName.substr(0, newFileName.find_last_of("\n"));
    strcpy(fileName, newFileName.c_str());

    ifstream infile;
    int numOutputs;
    string buffer, probBuffer;
    char newPrior[NUMBER_SECRETS][CHAR_BUFFER_SIZE];
    char newChannel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];
    try{
        infile.open(fileName); 
        
        // Prior
        getline(infile, buffer);
        if(buffer != "prior") throw exception();
        
        for(int i = 0; i < NUMBER_SECRETS; i++){
            infile >> probBuffer;
            strcpy(newPrior[i], probBuffer.c_str());
        }
        
        // Channel
        getline(infile, buffer);        // Skip \n
        getline(infile, buffer);
        if(buffer != "channel") throw exception();

        infile >> numOutputs;
        *newNumOutputs = numOutputs;      // Return the number of outputs read from file
    
        getline(infile, buffer);        // Skip \n
        for(int i = 0; i < NUMBER_SECRETS; i++){
            for(int j = 0; j < numOutputs; j++){
                infile >> probBuffer;
                strcpy(newChannel[i][j], probBuffer.c_str());
            }
            getline(infile, buffer);        // Skip \n
        }
        infile.close();

        // If no exception was thrown update prior and channel vectors        
        GuiPrior::copyPrior(newPrior, prior);
        GuiChannel::copyChannelText(newChannel, channel, numOutputs);
    }catch(const exception& e){
        cerr << "Invalid file. " << e.what() << '\n';        
        infile.close();
        return INVALID_QIF_FILE;
    }
    return NO_ERROR;
#else
    return NO_ERROR;
#endif
}

void GuiMenu::saveQIFFile(char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE], char channel[NUMBER_SECRETS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE], int numOutputs, bool createNewFile){
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

#if !defined(PLATFORM_WEB)
    if(createNewFile){
        FILE *file = popen("zenity --file-selection --title=Save --save --filename=untitled.qifg --confirm-overwrite", "r");
        fgets(fileName, 2048, file);
        fclose(file);

        if(strcmp(fileName, "\0")){
            // Remove \n from the end
            string newFileName = string(fileName);
            newFileName = newFileName.substr(0, newFileName.find_last_of("\n"));
            strcpy(fileName, newFileName.c_str());

            // Fix file extension if it is not .qifg
            string fn = string(fileName);
            if(fn.find_last_of(".") == string::npos){
                string newFileName = fn + ".qifg";
                strcpy(fileName, newFileName.c_str());
            }else if(fn.substr(fn.find_last_of(".") + 1) != "qifg"){
                string newFileName = fn.substr(0, fn.find_last_of(".")) + ".qifg";
                strcpy(fileName, newFileName.c_str());
            }
        }
    }

    if(strcmp(fileName, "\0")){
        string output = "";
        output = output + "prior\n" + prior[0] + " " + prior[1] + " " + prior[2] + "\n";
        output = output + "channel\n" + to_string(numOutputs) + "\n";
        for(int i = 0; i < NUMBER_SECRETS; i++){
            int j = 0;
            while(j < numOutputs-1){
                output = output + channel[i][j] + " ";
                j++;
            }
            output = output + channel[i][j] + "\n";
        }

        ofstream outfile;
        outfile.open(fileName);
        outfile << output;
        outfile.close();
    }
#endif
}

void GuiMenu::loadGSImages(){
    char imagesSrc[2][CHAR_BUFFER_SIZE] = {
        "images/gs-prior.png",
        "images/gs-channel.png"
    };

    for(int i = 0; i < 2; i++){
        Image img = LoadImage(imagesSrc[i]);
        ImageDrawTextEx(&img, GetFontDefault(), "", (Vector2){0,0}, (float)GetFontDefault().baseSize, 0.0f, BLACK);
        gsImages[i] = LoadTextureFromImage(img);  // Image converted to texture, uploaded to GPU memory (VRAM)
        UnloadImage(img);
    }
}