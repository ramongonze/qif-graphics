#include "guimenu.h"

GuiMenu::GuiMenu(){

}

GuiMenu::GuiMenu(int windowWidth, int windowHeight){
    // Data
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    for(int i = 0; i < 4; i++){
        ddActive[i] = 0;
        ddEdit[i] = false;
    }
    ddActive[BUTTON_MODE] = BUTTON_MODE_OPTION_SINGLE;
    windowGSActive = false;

    // Text
    strcpy(buttonFileText, "File;Open file;Save;Save as...;Exit");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_MODE], "Mode");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_SINGLE], "Single channel");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_TWO], "Two channels");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_REF], "Refinement");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_DP_SINGLE], "Differential privacy - Single mechanism");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_DP_TWO], "Differential privacy - Two mechanisms");
    strcpy(buttonModeOptions[BUTTON_MODE_OPTION_DP_POST_PROCESS], "Differential privacy - Post-processing");
    updateButtonModeString(BUTTON_MODE_OPTION_SINGLE);
    
    strcpy(buttonExamplesText, "Examples;Load channel that leaks everything;Load channel that leaks nothing");
    strcpy(buttonGuideText, "Guide");
    
    fileName = (char*) malloc(10*CHAR_BUFFER_SIZE*sizeof(char));
    strcpy(fileName, "\0");

    // Define controls rectangles
    #if !defined(PLATFORM_WEB)
        recButtons[BUTTON_FILE] = (Rectangle){0, 0, 50, 25};
        recButtons[BUTTON_MODE] = (Rectangle){51, 0, 50, 25};
        recButtons[BUTTON_EXAMPLES] = (Rectangle){102, 0, 80, 25};
        recButtons[BUTTON_GUIDE] = (Rectangle){183, 0, 60, 25};
    #else
        recButtons[BUTTON_MODE] = (Rectangle){0, 0, 50, 25};
        recButtons[BUTTON_EXAMPLES] = (Rectangle){51, 0, 80, 25};
        recButtons[BUTTON_GUIDE] = (Rectangle){132, 0, 60, 25};
    #endif
    
    int windowsStatusBarHeight = 22;
    recMenu = (Rectangle){0, 0, 1130, 25};

    // Getting started
    scrPanOffset = {0, 0};
    scrPanBOffset = {0, 0};

    recGS = (Rectangle){(float)(windowWidth*0.1), (float)(windowHeight*0.1), (float)(windowWidth*0.8), (float)(windowHeight*0.8)};
    recGSMenu = sum2Rec(recGS, 10, 10+windowsStatusBarHeight, -recGS.width*0.8, -windowsStatusBarHeight-20);
    recGSPanel = sum2Rec(recGS, recGSMenu.width+10, 10+windowsStatusBarHeight, -(recGS.width*0.2)-30, -windowsStatusBarHeight-20);
    recScrPan = recGSPanel;
    scrPanContent.y = recGS.height + gsOptionYOffset[GS_OPTION_QIF]; // Default value is GS_OPTION_QIF
    scrPanContent.x = recScrPan.width - 20; 

    gsOptionYOffset[GS_OPTION_QIF] = 300;
    gsContentHeight[GS_OPTION_QIF] = recGSPanel.height + 400;
    for(int option = GS_OPTION_PRIOR; option <= GS_OPTION_HYPER; option++){
        gsOptionYOffset[option] = -60;
        gsContentHeight[option] = recGSPanel.height;
    }

    for(int option = GS_OPTION_MODE_SINGLE; option <= GS_OPTION_MODE_TWO; option++){
        gsOptionYOffset[option] = 460;
        gsContentHeight[option] = recGSPanel.height + 560;
    }
    gsOptionYOffset[GS_OPTION_MODE_REF] = 520;
    gsContentHeight[GS_OPTION_MODE_REF] = recGSPanel.height + 620;

    strcpy(gsMenuOptions, "QIF Graphics;Prior distribution;Channels;Hyper-distribution;Mode single channel;Mode two channels;Mode refinement");
    gsMenuScrollIndex = 0;
    gsMenuActive = GS_OPTION_QIF;
    
    // GS description texts
    string s = "";
    s = string("QIF Graphics is a graphical tool that can be used to see geometrically the information leakage caused by channels wrt. to a set of secrets ")+
        string("using Quantitative Information Flow (QIF). Prior and hyper-distributions can be seen geometrically as circles such that the sum of the mass of inners ")+
        string("circles correspond to the mass of prior circle and considering the space of probability distributions over the set of three secrets, the prior ")+
        string("corresponds to the center of mass of all inners.")+
        string("\nThe tool has 3 modes of usage that can be selected in the menu bar:")+
        string("\n\n- Single channel: Given a prior distribution on the set of secrets and a channel that maps these secrets ")+
        string("to outputs, the tool draws in baricentric coordinates the prior and hyper distributions.")+
        string("\n\n- Two channels: Given a prior distribution on the set of secrets and two channels that maps each one to a set of ")+
        string("outputs, the tool draws in baricentirc coordinates the prior distribution and the inners from both channels.")+
        string("\n\n- Mode refinement: Given a prior distribution on the set of secrets, a channel C that maps these secrets ")+
        string("to a set of outputs and a channel R that post-processes the outputs of C, the tool builds a channel CR that ")+
        string("refines C and it also draws in baricentric coordinates the prior distribution and the hypers from C and CR, ")+
        string("in the way that is possible to see what refinement means geometrically.")+
        string("\n\nThe tool also has the option to show the convex hull of hyper-distributions, which indicates the refinement region.")+
        string("\n\nMore information about QIF and the geometric interpretation of prior, channels and hyper-distributions can be ")+
        string("found on\n\nAlvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S. The Science of Quantitative ")+
        string("Information Flow. Springer, 1 edition, 2019.");
    strcpy(gsDescriptionTexts[GS_OPTION_QIF], s.c_str());

    s = string("Prior distribution on the set of secrets X = {X1,X2,X3}. The box under the label Xi must be filled with a value that represents the ")+
        string("prior probability of the secret being Xi.")+
        string("\n\nAll probabilities can be written as fractions or as decimal numbers.");
    strcpy(gsDescriptionTexts[GS_OPTION_PRIOR], s.c_str());

    s = string("A channel is a system that takes as input a secret Xi, whose possible values come from a finite set of secrets X, ")+
        string("and whose only observable behavior is to produce an output Yi, whose possible values come from a finite set of outputs Y.")+
        string("\nThe box on row i and column j represents the conditional probability p(Yj|Xi) of the channel output Yj given that the secret is Xi.")+
        string("\n\nAll probabilities can be written as fractions or as decimal numbers.");
    strcpy(gsDescriptionTexts[GS_OPTION_CHANNELS], s.c_str());

    s = string("If X is a finite set (of possible secret values), \u03C0 is the prior distribution on X, and C is a channel, ")+
        string("a hyper-distribution [\u03C0\u203AC] is a distribution on distributions on X. Each ")+
        string("inner distribution \u03B4i is a possible \"world\", and each possible world is a new distribution on the ")+
        string("set of secrets. We call the possible worlds the inner distributions. Each possible world has a probability of ")+
        string("occurring, and we call the distribution on the possible worlds as the outer distribution.");
    strcpy(gsDescriptionTexts[GS_OPTION_HYPER], s.c_str());
    
    s = string("The single channel mode allows to define a prior distribution \u03C0 on the set of secrets X = {X1,X2,X3} and a channel C that maps X ")+
        string("to a set of outputs Y. The channel must have at least 1 output and at most 50. Each row in the channel must be a probability distribution.")+
        string("\n\nAll probabilities in both prior and channel can be written as fractions or as decimal numbers.")+
        string("\n\nAfter setting the prior \u03C0 and channel C, the hyper-distribution will be generated automatically, and clicking on the button \"Draw\", the geometric ")+
        string("representation of prior and inner distributions will be generated. It is possible, using the mouse pointer, to move the prior distribution ")+
        string("circle around the hyperplane of probability distributions (the triangle). Also it is possible to mark the checkbox \"Show convex hull\" ")+
        string("to see the convex hull of inner distributions.");
    strcpy(gsDescriptionTexts[GS_OPTION_MODE_SINGLE], s.c_str());
    
    s = string("The two channels mode allows to define a prior distribution \u03C0 on the set of secrets X = {X1,X2,X3} and two channels C and D that maps X ")+
        string("to sets of outputs Y and Y'. The channels must have at least 1 output and at most 50. Each row in the channels must be a probability distribution.")+
        string("\n\nAll probabilities in both prior and channel can be written as fractions or as decimal numbers.")+
        string("\n\nAfter setting the prior \u03C0 and channels C and D, the hyper-distributions will be generated automatically, and clicking on the button \"Draw\", the geometric ")+
        string("representation of prior and inner distributions will be generated. It is possible, using the mouse pointer, to move the prior distribution ")+
        string("circle around the hyperplane of probability distributions (the triangle). Also it is possible to mark the checkbox \"Show convex hull\" ")+
        string("to see the convex hull of inner distributions.");
    strcpy(gsDescriptionTexts[GS_OPTION_MODE_TWO], s.c_str());

    s = string("The refinement mode allows to define a prior distribution \u03C0 on the set of secrets X = {X1,X2,X3}, a channel C that maps X to a set of outputs Y and ")+
        string("a channel R that maps outputs of C (the set Y) to a new set o outputs Z. The channel R can be viewd as a post-processing of outputs of C. ")+
        string("The channels must have at least 1 output and at most 50. Each row in the channels must be a probability distribution.")+
        string("\n\nAll probabilities in both prior and channel can be written as fractions or as decimal numbers.")+
        string("\n\nAfter setting the prior \u03C0 and channels C and R, the tool will generate a new channel \"CR\" that is a composition of C and R.")+
        string("We have the property that CR refines the channel C. Then the hyper-distributions wrt. to channels ")+
        string("C and CR will be generated automatically, and clicking on the button \"Draw\", the geometric ")+
        string("representation of prior and inner distributions will be generated. It is possible, using the mouse pointer, to move the prior distribution ")+
        string("circle around the hyperplane of probability distributions (the triangle). Also it is possible to mark the checkbox \"Show convex hull\" ")+
        string("to see the convex hull of inner distributions.");
    strcpy(gsDescriptionTexts[GS_OPTION_MODE_REF], s.c_str());

    // GS images

    strcpy(imagesSrc[GS_OPTION_QIF], "");
    strcpy(imagesSrc[GS_OPTION_PRIOR], "images/gs-prior.png");
    strcpy(imagesSrc[GS_OPTION_CHANNELS], "images/gs-channel.png");
    strcpy(imagesSrc[GS_OPTION_HYPER], "images/gs-hyper.png");
    strcpy(imagesSrc[GS_OPTION_MODE_SINGLE], "images/gs-single.png");
    strcpy(imagesSrc[GS_OPTION_MODE_TWO], "images/gs-two.png");
    strcpy(imagesSrc[GS_OPTION_MODE_REF], "images/gs-ref.png");

    imgPadding[GS_OPTION_QIF] = 0;
    imgPadding[GS_OPTION_PRIOR] = 150;
    imgPadding[GS_OPTION_CHANNELS] = 250;
    imgPadding[GS_OPTION_HYPER] = 210;
    imgPadding[GS_OPTION_MODE_SINGLE] = 450;
    imgPadding[GS_OPTION_MODE_TWO] = 450;
    imgPadding[GS_OPTION_MODE_REF] = 530;

    loadGSImages();
}

void GuiMenu::updateButtonModeString(int mode){
    string newText = string(buttonModeOptions[BUTTON_MODE_OPTION_MODE]);
    for(int i = 1; i < NUM_BUTTON_MODE; i++){
        if(i == mode)
            newText = newText + ";#112#" + string(buttonModeOptions[i]);
        else
            newText = newText + ";#000#" + string(buttonModeOptions[i]);
    }
    strcpy(buttonModeText, newText.c_str());
}

int GuiMenu::readQIFFile(
    char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE],
    char channel[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE],
    int numSecrets[NUMBER_CHANNELS],
    int numOutputs[NUMBER_CHANNELS]
    ){

    /* File formats:
    
    Mode single channel:     |  Mode two channels:       |  Mode refinement:
    -----BEGIN OF FILE-----  |  -----BEGIN OF FILE-----  |  -----BEGIN OF FILE-----
    mode 1                   |  mode 2                   |  mode 3
    prior 3                  |  prior 3                  |  prior 3
    p1 p2 p3                 |  p1 p2 p3                 |  p1 p2 p3
    channel1 3 m             |  channel1 3 m             |  channel1 3 m
    p11 p12 ... p1m          |  p11 p12 ... p1m          |  p11 p12 ... p1m
    p21 p22 ... p2m          |  p21 p22 ... p2m          |  p21 p22 ... p2m
    p31 p32 ... p3m          |  p31 p32 ... p3m          |  p31 p32 ... p3m
    -----END OF FILE-----    |  channel2 3 m'            |  channel2 m m'
                             |  p11 p12 ... p1m'         |  p11 p12 ... p1m'
                             |  p21 p22 ... p2m'         |  p21 p22 ... p2m'
                             |  p31 p32 ... p3m'         |  ...
                             |  -----END OF FILE-----    |  pm1 pm2 ... pmm'
                             |                           |  -----END OF FILE-----

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

void GuiMenu::saveQIFFile(
    char prior[NUMBER_SECRETS][CHAR_BUFFER_SIZE],
    char channel[NUMBER_CHANNELS][MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE],
    int numSecrets[NUMBER_CHANNELS],
    int numOutputs[NUMBER_CHANNELS],
    int mode,
    bool createNewFile
    ){
    /* File formats:
    
    Mode single channel:     |  Mode two channels:       |  Mode refinement:
    -----BEGIN OF FILE-----  |  -----BEGIN OF FILE-----  |  -----BEGIN OF FILE-----
    mode 1                   |  mode 2                   |  mode 3
    prior 3                  |  prior 3                  |  prior 3
    p1 p2 p3                 |  p1 p2 p3                 |  p1 p2 p3
    channel1 3 m             |  channel1 3 m             |  channel1 3 m
    p11 p12 ... p1m          |  p11 p12 ... p1m          |  p11 p12 ... p1m
    p21 p22 ... p2m          |  p21 p22 ... p2m          |  p21 p22 ... p2m
    p31 p32 ... p3m          |  p31 p32 ... p3m          |  p31 p32 ... p3m
    -----END OF FILE-----    |  channel2 3 m'            |  channel2 m m'
                             |  p11 p12 ... p1m'         |  p11 p12 ... p1m'
                             |  p21 p22 ... p2m'         |  p21 p22 ... p2m'
                             |  p31 p32 ... p3m'         |  ...
                             |  -----END OF FILE-----    |  pm1 pm2 ... pmm'
                             |                           |  -----END OF FILE-----

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
            // Channel 1
            string output = "";
            output = output + "mode " + to_string(mode) + "\n";
            output = output + "prior 3\n" + prior[0] + " " + prior[1] + " " + prior[2] + "\n";
            output = output + "channel1 " + to_string(numSecrets[CHANNEL_1]) + " " + to_string(numOutputs[CHANNEL_1]) + "\n";
            for(int i = 0; i < numSecrets[CHANNEL_1]; i++){
                int j = 0;
                while(j < numOutputs[CHANNEL_1]-1){
                    output = output + channel[CHANNEL_1][i][j] + " ";
                    j++;
                }
                output = output + channel[CHANNEL_1][i][j] + "\n";
            }

            if(mode == MODE_TWO || mode == MODE_REF){
                // Channel 2
                output = output + "channel2 " + to_string(numSecrets[CHANNEL_2]) + " " + to_string(numOutputs[CHANNEL_2]) + "\n";
                for(int i = 0; i < numSecrets[CHANNEL_2]; i++){
                    int j = 0;
                    while(j < numOutputs[CHANNEL_2]-1){
                        output = output + channel[CHANNEL_2][i][j] + " ";
                        j++;
                    }
                    output = output + channel[CHANNEL_2][i][j] + "\n";
                }
            }

            ofstream outfile;
            outfile.open(fileName);
            outfile << output;
            outfile.close();
        }
    #endif
    }

    void GuiMenu::loadGSImages(){
        for(int i = 0; i < 7; i++){
            if(!strcmp(imagesSrc[i], ""))
                continue;

            Image img = LoadImage(imagesSrc[i]);
            ImageDrawTextEx(&img, GetFontDefault(), "", (Vector2){0,0}, (float)GetFontDefault().baseSize, 0.0f, BLACK);
            gsImages[i] = LoadTextureFromImage(img);  // Image converted to texture, uploaded to GPU memory (VRAM)
            UnloadImage(img);
        }
}
