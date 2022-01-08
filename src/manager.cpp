# include "manager.h"

Manager::Manager(){
    this->gui = Gui();
    this->data = Data();
    this->draw = Draw();
}

void Manager::checkButtonsMouseCollision(){
    // Active the button the mouse is over it
    int i = -1;
    Vector2 mousePoint = GetMousePosition();

    #if !defined(PLATFORM_WEB)
        if((gui.menu.ddEdit[BUTTON_FILE] || gui.menu.ddEdit[BUTTON_MODE] || gui.menu.ddEdit[BUTTON_EXAMPLES])
            &&
            (CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_FILE]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_MODE]) ||
            CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_EXAMPLES]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_GUIDE]))){
            i = 0; // File button is the first one
        }
    #else
        if((gui.menu.dropdownEditMode[BUTTON_MODE] || gui.menu.dropdownEditMode[BUTTON_EXAMPLES])
            &&
            (CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_MODE]) || CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_EXAMPLES]) ||
            CheckCollisionPointRec(mousePoint, gui.menu.recButtons[BUTTON_GUIDE]))){
            i = 1; // Mode button is the first one
        }
    #endif
    
    if(i >= 0){
        for(; i < 3; i++){
            if(i != BUTTON_MODE)
                gui.menu.ddActive[i] = 0;
                
            if(CheckCollisionPointRec(mousePoint, gui.menu.recButtons[i]))
                gui.menu.ddEdit[i] = true;
            else
                gui.menu.ddEdit[i] = false;
        }
    }
}

void Manager::checkHelpMessagesActive(Vector2 mousePosition){
    if(gui.helpMessagesActive[HELP_MSG_BUTTON_PRIOR])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 220, 60}, gui.helpMessages[HELP_MSG_BUTTON_PRIOR]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_CHANNEL])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 60}, gui.helpMessages[HELP_MSG_BUTTON_CHANNEL]);

    if(gui.helpMessagesActive[HELP_MSG_BUTTON_DRAW])
        draw.drawHelpMessage(gui, (Rectangle){mousePosition.x+10, mousePosition.y+10, 300, 90}, gui.helpMessages[HELP_MSG_BUTTON_DRAW]);
}

void Manager::checkPriorFlags(){
    if(data.compute[FLAG_PRIOR]){
        if(data.checkPriorText(gui.prior.tBoxPriorTxt) == NO_ERROR){
            if(Distribution::isDistribution(data.prior)){
                data.priorObj = Distribution(data.prior);
                data.ready[FLAG_PRIOR] = true;
            }else{
                data.error = INVALID_PRIOR;
                data.ready[FLAG_PRIOR] = false;
            }
        }else{
            data.error = INVALID_VALUE_PRIOR;
            data.ready[FLAG_PRIOR] = false;
        }
        
        data.compute[FLAG_PRIOR] = false;

        if(data.error == INVALID_PRIOR || data.error == INVALID_VALUE_PRIOR){
            for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
                data.ready[FLAG_HYPER_1+channel] = false;
                gui.posteriors.resetPosterior(channel);
                
                if(data.ready[FLAG_CHANNEL_1+channel]){
                    data.ready[FLAG_CHANNEL_1+channel] = false;
                    data.compute[FLAG_CHANNEL_1+channel] = true;
                }
            }
        }
    }else if(!data.ready[FLAG_PRIOR]){
        data.error = INVALID_PRIOR;
    }
}

void Manager::checkChannelsFlags(){
    // If prior is not ready, there is no reason to compute channel
    if(!data.ready[FLAG_PRIOR])
        return;

    int mode = gui.menu.ddActive[BUTTON_MODE];
    
    if(mode == MODE_DP_SINGLE){
        if(data.compute[FLAG_CHANNEL_1]){
            int ret = NO_ERROR;
            if(data.currentDPMechanism == MECH_KRR)
                ret = data.checkEpsilonDeltaText(gui.channel.tBoxEpsilonValue, gui.channel.tBoxDeltaValue);
            else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED)
                ret = data.checkAlphaText(gui.channel.tBoxAlphaValue);

            if(ret == NO_ERROR){
                if(data.currentDPMechanism == MECH_KRR){
                    random_response rr;
                    data.channel[CHANNEL_1] = rr.get_channel(3, log(data.epsilon), data.delta);
                }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
                    truncated_geometric tg;
                    data.channel[CHANNEL_1] = tg.get_channel(2, data.alpha);
                }

                data.channelObj[CHANNEL_1] = Channel(data.priorObj, data.channel[CHANNEL_1]);
                gui.updateChannelTBoxes(data.channelObj[CHANNEL_1], CHANNEL_1);
                data.ready[FLAG_CHANNEL_1] = true;
                data.compute[FLAG_HYPER_1] = true;
            }else{
                data.error = ret;
                gui.posteriors.resetPosterior(CHANNEL_1);
                gui.channel.resetChannel(CHANNEL_1);
                data.ready[FLAG_HYPER_1] = false;
            }
            data.compute[FLAG_CHANNEL_1] = false;
        }else if(!data.ready[FLAG_CHANNEL_1]){
            if(data.currentDPMechanism == MECH_KRR)
                data.error = INVALID_EPSILON_OR_DELTA;
            else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED)
                data.error = INVALID_ALPHA;
        }
    }else{
        for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
            if(data.compute[FLAG_CHANNEL_1+channel]){
                if(channel == CHANNEL_3){
                    // Multiply channels C and R
                    data.channelObj[CHANNEL_3] = composeChannels(data.channelObj[CHANNEL_1], data.channelObj[CHANNEL_2]);
                    data.channel[CHANNEL_3] = data.channelObj[CHANNEL_3].matrix;
                    gui.updateChannelTBoxes(data.channelObj[CHANNEL_3], CHANNEL_3);
                    data.ready[FLAG_CHANNEL_3] = true;
                    data.compute[FLAG_HYPER_3] = true; // Set hyper to be computed
                }else if(data.checkChannelText(gui.channel.tBoxChannelTxt[channel], channel, gui.channel.numSecrets[channel], gui.channel.numOutputs[channel]) == NO_ERROR){    
                    if(Channel::isChannel(data.channel[channel])){
                        if(channel == CHANNEL_2 && gui.menu.ddActive[BUTTON_MODE] == MODE_REF){
                            data.fakePrior = Distribution(gui.channel.numSecrets[CHANNEL_2], "uniform");
                            data.channelObj[CHANNEL_2] = Channel(data.fakePrior, data.channel[CHANNEL_2]);
                            if(data.ready[FLAG_CHANNEL_1])
                                data.compute[FLAG_CHANNEL_3] = true;
                        }else{
                            data.channelObj[channel] = Channel(data.priorObj, data.channel[channel]);
                            data.compute[FLAG_HYPER_1+channel] = true; // Set hyper to be computed
                            if(channel == CHANNEL_1 && gui.menu.ddActive[BUTTON_MODE] == MODE_REF)
                                data.compute[FLAG_CHANNEL_2] = true;
                        }
                        data.ready[FLAG_CHANNEL_1+channel] = true;
                    }else{
                        data.error = INVALID_CHANNEL_1 + channel;   
                    }
                }else{
                    data.error = INVALID_VALUE_CHANNEL_1 + channel;
                }

                data.compute[FLAG_CHANNEL_1+channel] = false;

                if(data.error == INVALID_CHANNEL_1+channel || data.error == INVALID_VALUE_CHANNEL_1+channel){
                    data.ready[FLAG_CHANNEL_1+channel] = false;
                    data.ready[FLAG_HYPER_1+channel] = false;
                    data.compute[FLAG_HYPER_1+channel] = false;
                    gui.posteriors.resetPosterior(channel);

                    if(mode == MODE_REF){
                        data.ready[FLAG_CHANNEL_3] = false;
                        data.compute[FLAG_CHANNEL_3] = false;
                        data.ready[FLAG_HYPER_3] = false;
                        data.compute[FLAG_HYPER_3] = false;
                        gui.posteriors.resetPosterior(CHANNEL_3);
                        gui.channel.resetChannel(CHANNEL_3);
                    }
                }
            }else{
                if(channel == CHANNEL_1 && !data.ready[FLAG_CHANNEL_1]){
                    data.error = INVALID_CHANNEL_1;
                }else if(channel == CHANNEL_2 && data.ready[FLAG_CHANNEL_1] && !data.ready[FLAG_CHANNEL_2] && gui.menu.ddActive[BUTTON_MODE] != MODE_SINGLE){
                    if(gui.menu.ddActive[BUTTON_MODE] == MODE_TWO) data.error = INVALID_CHANNEL_2_D;
                    else data.error = INVALID_CHANNEL_2_R;                
                }
            }
        }
    }    
}

void Manager::checkHypersFlags(){
    // If prior is not ready, there is no reason to compute the hyper
    if(!data.ready[FLAG_PRIOR])
        return;

    for(int channel = 0; channel < NUMBER_CHANNELS; channel++){
        // If channel is not ready, there is no reason to compute the hyper
        if(!data.ready[FLAG_CHANNEL_1+channel]){
            data.ready[FLAG_HYPER_1+channel] = false;
            continue;
        }

        if(data.compute[FLAG_HYPER_1+channel]){
            data.hyper[channel] = Hyper(data.channelObj[channel]);
            gui.posteriors.numPost[channel] = data.hyper[channel].num_post;
            data.ready[FLAG_HYPER_1+channel] = true;
            data.compute[FLAG_HYPER_1+channel] = false;
            
            if(gui.channel.curChannel == channel)
                gui.updateHyperTBoxes(data.hyper[channel], channel, data.ready[FLAG_HYPER_1+channel]);
        }
    }
}
