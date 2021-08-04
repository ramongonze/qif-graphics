#include "guimenu.h"

GuiMenu::GuiMenu(){

}

GuiMenu::GuiMenu(int windowWidth, int windowHeight){
    // Data
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    for(int i = 0; i < 4; i++){
        dropdownBoxActive[i] = 0;
        dropdownEditMode[i] = false;
    }
    dropdownBoxActive[BUTTON_MODE] = BUTTON_MODE_OPTION_SINGLE;
    windowGettingStartedActive = false;

    // Text
    strcpy(buttonFileText, "File;Open file;Save;Save as...;Exit");
    strcpy(buttonModeText, "Mode;#112#Single channel;#000#Two channels;#000#Refinement");
    strcpy(buttonExamplesText, "Examples;Load channel that leaks everything;Load channel that leaks nothing");
    strcpy(buttonHelpText, "Help;Getting Started;About");
    
    fileName = (char*) malloc(10*CHAR_BUFFER_SIZE*sizeof(char));
    strcpy(fileName, "\0");

    // Define controls rectangles
#if !defined(PLATFORM_WEB)
    layoutRecsButtons[BUTTON_FILE] = (Rectangle){0, 0, 50, 25};
    layoutRecsButtons[BUTTON_MODE] = (Rectangle){51, 0, 50, 25};
    layoutRecsButtons[BUTTON_EXAMPLES] = (Rectangle){102, 0, 80, 25};
    layoutRecsButtons[BUTTON_HELP] = (Rectangle){183, 0, 50, 25};
#else
    layoutRecsButtons[BUTTON_MODE] = (Rectangle){0, 0, 50, 25};
    layoutRecsButtons[BUTTON_EXAMPLES] = (Rectangle){51, 0, 80, 25};
    layoutRecsButtons[BUTTON_HELP] = (Rectangle){132, 0, 50, 25};
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
    strcpy(gsDescriptionTexts[GS_HYPER], "If X is a finite set (of possible secret values), \u03C0 is the prior distribution on X, and C is a channel, a hyper-distribution [\u03C0\u203AC] resulted from C on \u03C0 is a distribution on distributions on X. Each output in an informational channel is a possible \"world\", and each possible world is a new distribution on the set of secrets. We call the possible worlds the inner distributions. Each possible world has a probability of occurring, and we call the distribution on the possible worlds as the outer distribution.");
    strcpy(gsDescriptionTexts[GS_VISUALIZATION], "");

    imgPadding[GS_PRIOR] = 50;
    imgPadding[GS_CHANNEL] = 110;
    imgPadding[GS_HYPER] = 210;
    imgPadding[GS_VISUALIZATION] = 10;

    loadGSImages();
}

int GuiMenu::readQIFFile(
    char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE],
    char channel[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE],
    int numSecrets[NUMBER_CHANNELS],
    int numOutputs[NUMBER_CHANNELS]
    ){

    /* File formats:
    
    Mode single channel:
    -----BEGIN OF FILE-----
    mode 1
    prior 3
    p1 p2 p3
    channel1 3 m
    p11 p12 ... p1m
    p21 p22 ... p2m
    p31 p32 ... p3m
    -----END OF FILE-----

    Mode two channels:
    -----BEGIN OF FILE-----
    mode 2
    prior 3
    p1 p2 p3
    channel1 3 m
    p11 p12 ... p1m
    p21 p22 ... p2m
    p31 p32 ... p3m
    channel2 3 m'
    p11 p12 ... p1m'
    p21 p22 ... p2m'
    p31 p32 ... p3m'
    -----END OF FILE-----

    Mode refinement:
    -----BEGIN OF FILE-----
    mode 3
    prior 3
    p1 p2 p3
    channel1 3 m
    p11 p12 ... p1m
    p21 p22 ... p2m
    p31 p32 ... p3m
    channel2 m m'
    p11 p12 ... p1m'
    p21 p22 ... p2m'
    ...
    pm1 pm2 ... pmm'
    -----END OF FILE-----

    If there is an error with file, returns the error flag.
    If there is no error, return the mode flag contained in file.
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
    string buffer;
    int mode, bufferInt;
    int newNumberSecrets[NUMBER_CHANNELS];
    int newNumberOutputs[NUMBER_CHANNELS];
    char newPrior[NUMBER_SECRETS][CHAR_BUFFER_SIZE];
    char newChannels[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];
    try{
        infile.open(fileName); 
        
        // Find mode
        infile >> buffer;
        if(buffer != "mode") throw exception();
        infile >> mode;
        if(mode != MODE_SINGLE && mode != MODE_TWO && mode != MODE_REF) throw exception();

        // Prior
        infile >> buffer;
        if(buffer != "prior") throw exception();
        infile >> bufferInt;
        if(bufferInt != NUMBER_SECRETS) throw exception();

        for(int i = 0; i < bufferInt; i++){
            infile >> buffer;
            strcpy(newPrior[i], buffer.c_str());
        }

        // Channel 1
        infile >> buffer;
        if(buffer != "channel1") throw exception();
        infile >> newNumberSecrets[CHANNEL_1];
        if(newNumberSecrets[CHANNEL_1] != NUMBER_SECRETS) throw exception();
        infile >> newNumberOutputs[CHANNEL_1];
        if(newNumberOutputs[CHANNEL_1] < 0 || newNumberOutputs[CHANNEL_1] > 50) throw exception();

        for(int i = 0; i < newNumberSecrets[CHANNEL_1]; i++){
            for(int j = 0; j < newNumberOutputs[CHANNEL_1]; j++){
                infile >> buffer;
                strcpy(newChannels[CHANNEL_1][i][j], buffer.c_str());
            }
        }

        // Channel 2
        if(mode == MODE_TWO || mode == MODE_REF){
            infile >> buffer;
            if(buffer != "channel2") throw exception();
            infile >> newNumberSecrets[CHANNEL_2];
            if(mode == MODE_TWO && newNumberSecrets[CHANNEL_2] != NUMBER_SECRETS) throw exception();
            if(mode == MODE_REF && newNumberSecrets[CHANNEL_2] != newNumberOutputs[CHANNEL_1]) throw exception();
            infile >> newNumberOutputs[CHANNEL_2];
            if(newNumberOutputs[CHANNEL_2] < 0 || newNumberOutputs[CHANNEL_2] > 50) throw exception();

            for(int i = 0; i < newNumberSecrets[CHANNEL_2]; i++){
                for(int j = 0; j < newNumberOutputs[CHANNEL_2]; j++){
                    infile >> buffer;
                    strcpy(newChannels[CHANNEL_2][i][j], buffer.c_str());
                }
            }
        }

        // If no exception was thrown until here, there was no error with the file
        infile.close();

        // Copy prior values
        GuiPrior::copyPrior(newPrior, prior);

        // Copy channel 1 values
        numSecrets[CHANNEL_1] = newNumberSecrets[CHANNEL_1];
        numOutputs[CHANNEL_1] = newNumberOutputs[CHANNEL_1];
        GuiChannel::copyChannelText(newChannels[CHANNEL_1], channel[CHANNEL_1], numSecrets[CHANNEL_1], numOutputs[CHANNEL_1]);

        // Copy channel 2 values
        if(mode == MODE_TWO || mode == MODE_REF){
            numSecrets[CHANNEL_2] = newNumberSecrets[CHANNEL_2];
            numOutputs[CHANNEL_2] = newNumberOutputs[CHANNEL_2];
            GuiChannel::copyChannelText(newChannels[CHANNEL_2], channel[CHANNEL_2], numSecrets[CHANNEL_2], numOutputs[CHANNEL_2]);
        }
        
        return mode;
    }catch(const exception& e){
        infile.close();
        return INVALID_QIF_FILE;
    }
#else
    return MODE_SINGLE;
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
    char imagesSrc[4][CHAR_BUFFER_SIZE] = {
        "images/gs-prior.png",
        "images/gs-channel.png",
        "images/gs-hyper.png",
        "images/gs-visualization.png"
    };

    for(int i = 0; i < 4; i++){
        Image img = LoadImage(imagesSrc[i]);
        ImageDrawTextEx(&img, GetFontDefault(), "", (Vector2){0,0}, (float)GetFontDefault().baseSize, 0.0f, BLACK);
        gsImages[i] = LoadTextureFromImage(img);  // Image converted to texture, uploaded to GPU memory (VRAM)
        UnloadImage(img);
    }
}