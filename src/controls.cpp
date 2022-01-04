#include "controls.h"

Controls::Controls(){}

void Controls::buttonFile(Gui &gui, Data &data, bool* closeWindow){
#if !defined(PLATFORM_WEB)
    int option = gui.menu.ddActive[BUTTON_FILE];

    if(option == BUTTON_FILE_OPTION_OPEN){
        gui.drawing = false;
        int retRead = gui.menu.readQIFFile(gui.prior.tBoxPriorTxt, gui.channel.tBoxChannelTxt, gui.channel.numSecrets, gui.channel.numOutputs);

        if(retRead == INVALID_QIF_FILE){
            // Open a dialog error
            system("zenity --error --no-wrap --text=\"Invalid QIF graphics file\"");
        }else{
            // Update channels spinners
            gui.channel.spinChannelValue[CHANNEL_1] = gui.channel.numOutputs[CHANNEL_1];
            gui.channel.spinChannelValue[CHANNEL_2] = gui.channel.numOutputs[CHANNEL_2];
            
            // Reset prior
            data.ready[FLAG_PRIOR] = false;
            data.compute[FLAG_PRIOR] = true;

            // Reset channels
            data.resetAllExceptComputeChannel1();

            for(int channel = 0; channel < NUMBER_CHANNELS; channel++)
                gui.posteriors.resetPosterior(FLAG_CHANNEL_1+channel);

            // Update current mode
            gui.channel.checkModeAndSizes(retRead);
            gui.menu.ddActive[BUTTON_MODE] = retRead;

            if(retRead == MODE_TWO) data.compute[FLAG_CHANNEL_2] = true;

            updateStatusBar(NO_ERROR, gui.visualization);
        }
    }else if(option == BUTTON_FILE_OPTION_SAVE){
        gui.menu.saveQIFFile(
            gui.prior.tBoxPriorTxt,
            gui.channel.tBoxChannelTxt,
            gui.channel.numSecrets,
            gui.channel.numOutputs,
            gui.menu.ddActive[BUTTON_MODE],
            strcmp(gui.menu.fileName, "\0") == 0 ? true : false
        );
        if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;   
    }else if(option == BUTTON_FILE_OPTION_SAVEAS){
        gui.menu.saveQIFFile(
            gui.prior.tBoxPriorTxt,
            gui.channel.tBoxChannelTxt,
            gui.channel.numSecrets,
            gui.channel.numOutputs,
            gui.menu.ddActive[BUTTON_MODE],
            true
        );
        if(strcmp(gui.menu.fileName, "\0")) data.fileSaved = true;
    }else if(option == BUTTON_FILE_OPTION_EXIT){
        if(data.fileSaved){
            *closeWindow = true;
        }else{
            int ret;
            FILE *file = popen("zenity --no-wrap --title=\"QIF Graphics\" --question --text=\"Do you want to save changes you made?\"", "r");
            ret = WEXITSTATUS(pclose(file));        // Get the user input Yes=0 or No=1

            if(ret == 0){
                // Yes
                gui.menu.saveQIFFile(
                    gui.prior.tBoxPriorTxt,
                    gui.channel.tBoxChannelTxt,
                    gui.channel.numSecrets,
                    gui.channel.numOutputs,
                    gui.menu.ddActive[BUTTON_MODE],
                    strcmp(gui.menu.fileName, "\0") == 0 ? true : false
                );
                if(strcmp(gui.menu.fileName, "\0")){
                    data.fileSaved = true;
                    *closeWindow = true;  
                }
            }else{
                // No
                *closeWindow = true;
            }
        }
    }
#endif
    gui.menu.ddActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;
}

void Controls::buttonMode(Gui &gui, Data &data, int* prevMode){
    int curMode = gui.menu.ddActive[BUTTON_MODE];

    if(*prevMode != curMode){
        gui.channel.checkModeAndSizes(curMode);

        // Note: Channel 1 doesn't need to be changed in any mode
        gui.drawing = false;
        updateStatusBar(NO_ERROR, gui.visualization);

        // Set prior to be computed
        data.ready[FLAG_PRIOR] = false;
        data.compute[FLAG_PRIOR] = true;

        data.resetAllExceptComputeChannel1();

        if(curMode == MODE_SINGLE || curMode == MODE_DP_SINGLE){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x, gui.channel.anchorChannel.y, 350, 20};
        }else if(curMode == MODE_TWO){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x+112, gui.channel.anchorChannel.y, 350-112, 20};
            data.compute[FLAG_CHANNEL_2] = true;
        }else if(curMode == MODE_REF){
            gui.channel.recTitle = (Rectangle){gui.channel.anchorChannel.x+168, gui.channel.anchorChannel.y, 350-168, 20};
        }
    }

    *prevMode = curMode;
}

void Controls::buttonExamples(Gui &gui, Data &data){
    int example = gui.menu.ddActive[BUTTON_EXAMPLES];
    
    // If none of the options were seleted, skip.
    if(example != BUTTON_EXAMPLES_OPTION_CH_0 && example != BUTTON_EXAMPLES_OPTION_CH_1)
        return;

    int curChannel = gui.channel.curChannel;

    // If some option was selected when the current channel is CR, skip.
    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF && curChannel == CHANNEL_3)
        return;

    char newChannel[MAX_CHANNEL_OUTPUTS][MAX_CHANNEL_OUTPUTS][CHAR_BUFFER_SIZE];

    gui.channel.spinChannelValue[curChannel] = (example == BUTTON_EXAMPLES_OPTION_CH_0) ? gui.channel.numSecrets[curChannel] : 1;    
    gui.channel.updateChannelBySpinner(gui.channel.curChannel, gui.menu.ddActive[BUTTON_MODE]);

    if(example == BUTTON_EXAMPLES_OPTION_CH_0){
        // Set identity matrix
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            for(int j = 0; j < gui.channel.numOutputs[curChannel]; j++){
                if(i == j) strcpy(newChannel[i][j], "1");
                else strcpy(newChannel[i][j], "0");
            }
        }
    }

    if(example == BUTTON_EXAMPLES_OPTION_CH_1){
        // Set channel that leaks nothing
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            strcpy(newChannel[i][0], "1");
        }
    }

    GuiChannel::copyChannelText(newChannel, gui.channel.tBoxChannelTxt[curChannel], gui.channel.numSecrets[curChannel], gui.channel.numOutputs[curChannel]);
    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+curChannel] = true;
        data.ready[FLAG_CHANNEL_1+curChannel] = false;
    }
    gui.drawing = false;
}

void Controls::buttonGuide(Gui &gui){
    gui.menu.windowGSActive = true;
}

void Controls::buttonRandomPrior(Gui &gui, Data &data){
    data.newRandomPrior();
    gui.drawing = false;
    data.fileSaved = false;
    data.compute[FLAG_PRIOR] = false;
    data.ready[FLAG_PRIOR] = true;
    gui.updatePriorTBoxes(data.priorObj);
    
    int mode = gui.menu.ddActive[BUTTON_MODE];
    
    // Reset channel 1 and set it to be computed
    data.ready[FLAG_CHANNEL_1] = false;
    data.compute[FLAG_CHANNEL_1] = true;
    data.ready[FLAG_HYPER_1] = false;
    data.compute[FLAG_HYPER_1] = false;

    if(mode == MODE_TWO || mode == MODE_REF){
        // Reset channel 2 and set it to be computed
        data.ready[FLAG_CHANNEL_2] = false;
        data.compute[FLAG_CHANNEL_2] = true;
        data.ready[FLAG_HYPER_2] = false;
        data.compute[FLAG_HYPER_2] = false;
    }

    if(mode == MODE_REF){
        // Reset channel 3
        data.ready[FLAG_CHANNEL_3] = false;
        data.compute[FLAG_CHANNEL_3] = false;
        data.ready[FLAG_HYPER_3] = false;
        data.compute[FLAG_HYPER_3] = false;
    }
}

void Controls::buttonsTabs(Gui &gui, int channel){
    gui.channel.curChannel = channel;
    updateStatusBar(NO_ERROR, gui.visualization);
}

void Controls::buttonRandomChannel(Gui &gui, Data &data){
    if(data.error == INVALID_CHANNEL_1 || data.error == INVALID_CHANNEL_2_D || data.error == INVALID_CHANNEL_2_R)
        updateStatusBar(NO_ERROR, gui.visualization);
    
    data.newRandomChannel(gui.channel.curChannel, gui.channel.numSecrets[gui.channel.curChannel], gui.channel.numOutputs[gui.channel.curChannel]);
    gui.drawing = false;
    data.fileSaved = false;
    gui.channel.updateChannelTextBoxes(data.channel[gui.channel.curChannel]);

    if(gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
        data.resetAllExceptComputeChannel1();
    }else{
        data.compute[FLAG_CHANNEL_1+gui.channel.curChannel] = true;
        data.ready[FLAG_CHANNEL_1+gui.channel.curChannel] = false;
    }
}

void Controls::buttonDraw(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    if(mode == MODE_SINGLE || mode == MODE_DP_SINGLE){
        if(!data.ready[FLAG_HYPER_1]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = STEPS;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRecPriorCircLabel(data.priorCircle);
        gui.updatePriorTBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRecInnersCircLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        if(data.currentDPMechanism == MECH_KRR){
            gui.visualization.spinEpsilonValue = stoi(gui.channel.tBoxEpsilonValue);
            gui.visualization.sliderDeltaValue = stof(gui.channel.tBoxDeltaValue);
        }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            gui.visualization.sliderAlphaValue = stof(gui.channel.tBoxAlphaValue);
        }
    }else if(mode == MODE_TWO){
        if(!data.ready[FLAG_HYPER_1] || !data.ready[FLAG_HYPER_2]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = 2*STEPS+1;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRecPriorCircLabel(data.priorCircle);
        gui.updatePriorTBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRecInnersCircLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_2, mode);
        gui.updateRecInnersCircLabel(CHANNEL_2, data.innersCircles[CHANNEL_2]);
    }else if(mode == MODE_REF){
        if(!data.ready[FLAG_HYPER_1] || !data.ready[FLAG_HYPER_3]){
            updateStatusBar(data.error, gui.visualization);
            return;
        }

        gui.drawing = true;
        data.animationRunning = true;
        data.animation = 2*STEPS+1;

        data.buildPriorCircle(gui.visualization.trianglePoints);
        gui.updateRecPriorCircLabel(data.priorCircle);
        gui.updatePriorTBoxes(data.priorObj);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_1, mode);
        gui.updateRecInnersCircLabel(CHANNEL_1, data.innersCircles[CHANNEL_1]);

        data.buildInnerCircles(gui.visualization.trianglePoints, CHANNEL_3, mode);
        gui.updateRecInnersCircLabel(CHANNEL_3, data.innersCircles[CHANNEL_3]);
    }
}

void Controls::updateStatusBar(int error, GuiVisualization &visualization){
	switch(error){
		case INVALID_VALUE_PRIOR:
			strcpy(visualization.tBoxStatusTxt, "Some character in prior distribution is not valid");
			break;
        case INVALID_VALUE_CHANNEL_1:
			strcpy(visualization.tBoxStatusTxt, "Some character in channel C is not valid");
			break;
        case INVALID_VALUE_CHANNEL_2:
			strcpy(visualization.tBoxStatusTxt, "Some character in channel R is not valid");
			break;
		case INVALID_PRIOR:
			strcpy(visualization.tBoxStatusTxt, "The values in prior distribution do not make up a probability distribution");
			break;
		case INVALID_CHANNEL_1:
			strcpy(visualization.tBoxStatusTxt, "Some row in channel C is not a probability distribution");
			break;
        case INVALID_CHANNEL_2_D:
			strcpy(visualization.tBoxStatusTxt, "Some row in channel D is not a probability distribution");
            break;
        case INVALID_CHANNEL_2_R:
			strcpy(visualization.tBoxStatusTxt, "Some row in channel R is not a probability distribution");
			break;
        case INVALID_EPSILON_OR_DELTA:
			strcpy(visualization.tBoxStatusTxt, "Invalid epsilon or delta");
			break;
        case INVALID_ALPHA:
			strcpy(visualization.tBoxStatusTxt, "Invalid alpha");
			break;
		case NO_ERROR:
			strcpy(visualization.tBoxStatusTxt, "Status");
	}
}
