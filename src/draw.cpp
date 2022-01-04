#include "draw.h"

Draw::Draw(){
    controls = Controls();
}

void Draw::initStyle(){
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, -4);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 0);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 1);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    GuiSetStyle(DROPDOWNBOX, TEXT_INNER_PADDING, 5);
    GuiSetStyle(LISTVIEW, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(SCROLLBAR, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
    GuiSetStyle(SCROLLBAR, BORDER_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_DARK));
    GuiSetStyle(SCROLLBAR, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(SPINNER, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_RIGHT);
}

//------------------------------------------------------------------------------------
// Draw Functions Definitions
//------------------------------------------------------------------------------------
void Draw::drawGuiMenu(Gui &gui, Data &data, bool* closeWindow){
    DrawRectangleRec(gui.menu.recMenu, MENU_BASE_COLOR_NORMAL);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

#if !defined(PLATFORM_WEB)
    // Button File
    if(!gui.menu.ddEdit[BUTTON_FILE]) gui.menu.ddActive[BUTTON_FILE] = BUTTON_FILE_OPTION_FILE;        // Reset selection
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_FILE], 120, gui.menu.buttonFileText, &(gui.menu.ddActive[BUTTON_FILE]), gui.menu.ddEdit[BUTTON_FILE])) gui.menu.ddEdit[BUTTON_FILE] = !gui.menu.ddEdit[BUTTON_FILE];
#endif

    // Button Mode
    gui.menu.updateButtonModeString(gui.menu.ddActive[BUTTON_MODE]);
    
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_MODE], 330, gui.menu.buttonModeText, &(gui.menu.ddActive[BUTTON_MODE]), gui.menu.ddEdit[BUTTON_MODE])) gui.menu.ddEdit[BUTTON_MODE] = !gui.menu.ddEdit[BUTTON_MODE];

    // Button Examples
    if(!gui.menu.ddEdit[BUTTON_EXAMPLES]) gui.menu.ddActive[BUTTON_EXAMPLES] = BUTTON_EXAMPLES_OPTION_EXAMPLES;        // Reset selection
    if(GuiDropdownBox(gui.menu.recButtons[BUTTON_EXAMPLES], 320, gui.menu.buttonExamplesText, &(gui.menu.ddActive[BUTTON_EXAMPLES]), gui.menu.ddEdit[BUTTON_EXAMPLES])) gui.menu.ddEdit[BUTTON_EXAMPLES] = !gui.menu.ddEdit[BUTTON_EXAMPLES];
    
    // Button Guide
    if(GuiButton(gui.menu.recButtons[BUTTON_GUIDE], gui.menu.buttonGuideText)) controls.buttonGuide(gui);
}

void Draw::drawGuiPrior(Gui &gui, Data &data){
    // if(gui.drawing && gui.menu.dropdownBoxActive[BUTTON_MODE] != MODE_SINGLE)
    if(gui.drawing)
        drawContentPanel(gui.prior.recTitle, gui.prior.recContent, gui.prior.panPriorTxt, PRIOR_COLOR_L1, gui.defaultFont);
    else
        drawContentPanel(gui.prior.recTitle, gui.prior.recContent, gui.prior.panPriorTxt, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), gui.defaultFont);

    DrawRectangleRec(gui.prior.recPan, WHITE);
    DrawRectangleLinesEx(gui.prior.recPan, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    if(GuiButton(gui.prior.recButtonRand, gui.prior.buttonRandTxt)) controls.buttonRandomPrior(gui, data); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    for(int i = 0; i < NUMBER_SECRETS; i++){
        GuiLabel(gui.prior.recLabel[i], gui.prior.labelPriorTxt[i].c_str());
        if(GuiTextBox(gui.prior.recTBox[i], gui.prior.tBoxPriorTxt[i], CHAR_BUFFER_SIZE, gui.prior.tBoxPriorEdit[i])) gui.prior.tBoxPriorEdit[i] = !gui.prior.tBoxPriorEdit[i];
    }
}

void Draw::drawGuiChannel(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    int curChannel = gui.channel.curChannel;
    Color contentColor = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));

    if(mode == MODE_SINGLE){
        strcpy(gui.channel.panChannelTxt, "Channel C");
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        drawContentPanel(gui.channel.recTitle, gui.channel.recContent, gui.channel.panChannelTxt, contentColor, gui.defaultFont);
    }else if(mode == MODE_DP_SINGLE || mode == MODE_DP_POST_PROCESS){
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        DrawRectangleRec(gui.channel.recContent, contentColor);
    }else{
        strcpy(gui.channel.panChannelTxt, "");
        if(mode == MODE_TWO){
            strcpy(gui.channel.labelChannelTabs[CHANNEL_2], "Ch D");
        }else if(mode == MODE_REF){
            strcpy(gui.channel.labelChannelTabs[CHANNEL_2], "Ch R");
        }
        
        if(gui.drawing){
            if(curChannel == CHANNEL_1) contentColor = INNERS1_COLOR_D1;
            else if(curChannel == CHANNEL_3 || (curChannel == CHANNEL_2 && mode == MODE_TWO)) contentColor = INNERS2_COLOR;
        }
        drawContentPanel(gui.channel.recTitle, gui.channel.recContent, gui.channel.panChannelTxt, contentColor, gui.defaultFont);

        for(int i = 0; i < NUMBER_CHANNELS - (mode == MODE_TWO ? 1 : 0); i++)
            drawTab(gui, i, curChannel == i);

        initStyle();
    }

    if(mode != MODE_DP_SINGLE && curChannel != CHANNEL_3){
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
        if(GuiButton(gui.channel.recButtonRand, gui.channel.buttonRandTxt)) controls.buttonRandomChannel(gui, data); 
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    }

    if(mode != MODE_DP_SINGLE && curChannel != CHANNEL_3){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));

        if(gui.drawing && (curChannel == CHANNEL_1 || (curChannel == CHANNEL_3) || (mode == MODE_TWO && curChannel == CHANNEL_2))){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }

        if(GuiSpinner(gui.channel.recSpinner, gui.channel.labelOutTxt, &(gui.channel.spinChannelValue[curChannel]), 0, 50, gui.channel.spinChannelEdit)) gui.channel.spinChannelEdit = !gui.channel.spinChannelEdit;
        GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    }

    if(mode == MODE_DP_SINGLE){
        if(gui.drawing){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        }
        
        if(data.currentDPMechanism == MECH_KRR){
            GuiLabel(gui.channel.recTBoxEpsilon, gui.channel.labelEpsilonTxt);
            GuiLabel(gui.channel.recTBoxDelta, gui.channel.labelDeltaTxt);        
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxEpsilon, 80, 0, 0, 0), gui.channel.tBoxEpsilonValue, gui.defaultFont.baseSize, gui.channel.tBoxEpsilonEdit)) gui.channel.tBoxEpsilonEdit = !gui.channel.tBoxEpsilonEdit;
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxDelta, 80, 0, 0, 0), gui.channel.tBoxDeltaValue, gui.defaultFont.baseSize, gui.channel.tBoxDeltaEdit)) gui.channel.tBoxDeltaEdit = !gui.channel.tBoxDeltaEdit;

        }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
            GuiLabel(gui.channel.recTBoxAlpha, gui.channel.labelAlphaTxt);
            if(GuiTextBox(sum2Rec(gui.channel.recTBoxAlpha, 80, 0, 0, 0), gui.channel.tBoxAlphaValue, gui.defaultFont.baseSize, gui.channel.tBoxAlphaEdit)) gui.channel.tBoxAlphaEdit = !gui.channel.tBoxAlphaEdit;
        }

        if(gui.drawing){
            GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScroll = GuiScrollPanel(
        sum2Rec(gui.channel.recScrPan, 0, 0, -gui.channel.scrPanBOffset.x, -gui.channel.scrPanBOffset.y),
        (Rectangle){gui.channel.recScrPan.x, gui.channel.recScrPan.y, gui.channel.scrPanContent.x, gui.channel.scrPanContent.y},
        &(gui.channel.scrPanOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScroll.x, viewScroll.y, viewScroll.width, viewScroll.height);
        if(mode != MODE_DP_SINGLE)
            GuiLabel(sum2Rec(gui.channel.recLabelOut, gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelOutTxt);

        // Secrets
        for(int i = 0; i < gui.channel.numSecrets[curChannel]; i++){
            if(mode == MODE_SINGLE || mode == MODE_TWO || curChannel == CHANNEL_1 || curChannel == CHANNEL_3){
                GuiLabel(sum2Rec(gui.channel.recLabelX[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelXTxt[i].c_str());
            }else{
                GuiLabel(sum2Rec(gui.channel.recLabelX[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
            }

            for(int j = 0; j < gui.channel.numOutputs[curChannel]; j++){
                if(curChannel == CHANNEL_3 || mode == MODE_DP_SINGLE) GuiLock();
                if(GuiTextBox(sum2Rec(gui.channel.recTBoxChannel[i][j], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.tBoxChannelTxt[curChannel][i][j], CHAR_BUFFER_SIZE, gui.channel.tBoxChannelEdit[i][j])) gui.channel.tBoxChannelEdit[i][j] = !gui.channel.tBoxChannelEdit[i][j];
                if(curChannel == CHANNEL_3 || mode == MODE_DP_SINGLE) GuiUnlock();
            }
        }

        // Outputs labels
        if(curChannel == CHANNEL_1 || curChannel == CHANNEL_3){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
        }else if(curChannel == CHANNEL_2 && mode == MODE_TWO){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYPTxt[i].c_str());
        }else if(curChannel == CHANNEL_2 && mode == MODE_REF){
            for(int i = 0; i < gui.channel.numOutputs[curChannel]; i++)
                GuiLabel(sum2Rec(gui.channel.recLabelY[i], gui.channel.scrPanOffset.x, gui.channel.scrPanOffset.y, 0, 0), gui.channel.labelChannelYTxt[i].c_str());
        }
    EndScissorMode();

    // DP mechanisms
    if(mode == MODE_DP_SINGLE || mode == MODE_DP_POST_PROCESS){
        strcpy(gui.channel.panChannelTxt, "Channel C");
        if(gui.drawing) contentColor = INNERS1_COLOR_D1;
        drawContentDropBox(gui, gui.channel.recTitle, gui.channel.recContent, contentColor, gui.defaultFont);
    }
}

void Draw::drawGuiPosteriors(Gui &gui, Data &data){
    int mode = gui.menu.ddActive[BUTTON_MODE];
    int curChannel = gui.channel.curChannel;

    if(curChannel == CHANNEL_1){
        strcpy(gui.posteriors.gBoxPostTxt, "Hyper-distribution [\u03C0\u203AC]");
    }else if(curChannel == CHANNEL_2){
        if(mode == MODE_REF){
            strcpy(gui.posteriors.gBoxPostTxt, "");
        }else{
            strcpy(gui.posteriors.gBoxPostTxt, "Hyper-distribution [\u03C0\u203AD]");
        }
    }else if(curChannel == CHANNEL_3){
        strcpy(gui.posteriors.gBoxPostTxt, "Hyper-distribution [\u03C0\u203ACR]");
    }
    
    Color contentColor = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
    if(gui.drawing){
        if(curChannel == CHANNEL_1) contentColor = INNERS1_COLOR_D1;
        else if(curChannel == CHANNEL_3 || (curChannel == CHANNEL_2 && mode == MODE_TWO)) contentColor = INNERS2_COLOR;
    }
    drawContentPanel(gui.posteriors.recTitle, gui.posteriors.recContent, gui.posteriors.gBoxPostTxt, contentColor, gui.defaultFont);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_FOCUSED));
    Rectangle viewScrollPosteriors = GuiScrollPanel(
        sum2Rec(gui.posteriors.recScrPan, 0, 0, - gui.posteriors.scrPanPostBOffset.x, -gui.posteriors.scrPanPostBOffset.y),
        (Rectangle){gui.posteriors.recScrPan.x, gui.posteriors.recScrPan.y, gui.posteriors.scrPanPostContent.x, gui.posteriors.scrPanPostContent.y},
        &(gui.posteriors.scrPanPostOffset)
    );
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));

    BeginScissorMode(viewScrollPosteriors.x, viewScrollPosteriors.y, viewScrollPosteriors.width, viewScrollPosteriors.height);
        if(mode != MODE_REF || curChannel != CHANNEL_2){
            GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiLabel(sum2Rec(gui.posteriors.recLabelOuter, gui.posteriors.scrPanPostOffset.x, gui.posteriors.scrPanPostOffset.y, 0, 0), gui.posteriors.labelOuterTxt);

            for(int i = 0; i < gui.posteriors.numPost[curChannel]; i++){
                GuiLabel(sum2Rec(gui.posteriors.recLabelPost[i], gui.posteriors.scrPanPostOffset.x, gui.posteriors.scrPanPostOffset.y, 0, 0), gui.posteriors.labelPostTxt[curChannel][i].c_str());
                GuiTextBox(sum2Rec(gui.posteriors.recTBoxOuter[i], gui.posteriors.scrPanPostOffset.x, gui.posteriors.scrPanPostOffset.y, 0, 0), gui.posteriors.tBoxOuterTxt[i], CHAR_BUFFER_SIZE, gui.posteriors.tBoxOuterEdit[i]);
            }

            for(int i = 0; i < NUMBER_SECRETS; i++){
                GuiLabel(sum2Rec(gui.posteriors.recLabelX[i], gui.posteriors.scrPanPostOffset.x, gui.posteriors.scrPanPostOffset.y, 0, 0), gui.posteriors.labelPostXTxt[i].c_str());
                for(int j = 0; j < gui.posteriors.numPost[curChannel]; j++){
                    GuiTextBox(sum2Rec(gui.posteriors.recTBoxInners[i][j], gui.posteriors.scrPanPostOffset.x, gui.posteriors.scrPanPostOffset.y, 0, 0), gui.posteriors.tBoxInnersTxt[i][j], CHAR_BUFFER_SIZE, gui.posteriors.tBoxInnersEdit[i][j]);
                }
            }
        }
    EndScissorMode();
}

void Draw::drawGuiVisualization(Gui &gui, Data &data){
    drawContentPanel(gui.visualization.recTitle, gui.visualization.recContent, gui.visualization.gBoxVisTxt, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), gui.defaultFont);
    if(GuiButton(gui.visualization.recButtonDraw, gui.visualization.buttonDrawTxt)) controls.buttonDraw(gui, data);
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(data.error != NO_ERROR) DrawRectangleRec(gui.visualization.recTBoxStatus, WHITE);
    DrawRectangleRec(gui.visualization.recTBoxStatus, WHITE);
    
    if(strcmp(gui.visualization.tBoxStatusTxt, "Status")) GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(RED));
    GuiTextBox(gui.visualization.recTBoxStatus, gui.visualization.tBoxStatusTxt, CHAR_BUFFER_SIZE, gui.visualization.tBoxStatusEdit);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 0);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    
    GuiPanel(gui.visualization.recPanVis);

    if(gui.drawing){
        // Checkboxes
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(CHECKBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.showLabels = GuiCheckBox(gui.visualization.recCBoxShowLabels, gui.visualization.labelCBoxShowLabel, gui.showLabels);
        gui.showCHull = GuiCheckBox(gui.visualization.recCBoxShowCHull, gui.visualization.labelCBoxShowCHull, gui.showCHull);

        int mode = gui.menu.ddActive[BUTTON_MODE];
        
        // Sliders
        if(mode == MODE_DP_SINGLE){
            if(data.currentDPMechanism == MECH_KRR){
                GuiLabel(sum2Rec(gui.visualization.recSpinEpsilon, -gui.visualization.recSpinEpsilon.width-5, 0, 0, 0), "Epsilon (ln):");
                
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
                GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
                GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
                GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(BLACK));
                if(GuiSpinner(gui.visualization.recSpinEpsilon, NULL, &(gui.visualization.spinEpsilonValue), 1, 10000, gui.visualization.spinEpsilonEdit)) gui.visualization.spinEpsilonEdit = !gui.visualization.spinEpsilonEdit;
                GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
                GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

                GuiLabel(sum2Rec(gui.visualization.recSliderDelta, -gui.visualization.recSliderDelta.width+15, 0, 0, 0), "Delta:");
                GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_FOCUSED_TRANSP));
                gui.visualization.sliderDeltaValue = GuiSlider(gui.visualization.recSliderDelta, NULL, NULL, gui.visualization.sliderDeltaValue, 0, 1);
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui.visualization.sliderDeltaValue);
                GuiLabel(gui.visualization.recSliderDelta, buffer);
            }else if(data.currentDPMechanism == MECH_GEOMETRIC_TRUNCATED){
                GuiLabel(sum2Rec(gui.visualization.recSliderAlpha, -gui.visualization.recSliderAlpha.width+15, 0, 0, 0), "Alpha:");
                GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_FOCUSED_TRANSP));
                gui.visualization.sliderAlphaValue = GuiSlider(gui.visualization.recSliderAlpha, NULL, NULL, gui.visualization.sliderAlphaValue, 0, 1);
                char buffer[CHAR_BUFFER_SIZE];
                sprintf(buffer, "%.6f", gui.visualization.sliderAlphaValue);
                GuiLabel(gui.visualization.recSliderAlpha, buffer);
            }
        }

        // Triangle
        DrawTriangle(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BG_BASE_COLOR_LIGHT2);
        DrawTriangleLines(gui.visualization.trianglePoints[0], gui.visualization.trianglePoints[1], gui.visualization.trianglePoints[2], BLACK);
        for(int i = 0; i < NUMBER_SECRETS; i++){
            DrawTextEx(gui.defaultFontBig, &(gui.visualization.labelTriangleTxt[i][0]), (Vector2){gui.visualization.recLabelTriangle[i].x, gui.visualization.recLabelTriangle[i].y}, 32, 0, BLACK);
        }

        // Circles
        drawCirclePrior(gui, data);
        drawCirclesInners(gui, data, CHANNEL_1);

        if(mode == MODE_TWO)
            drawCirclesInners(gui, data, CHANNEL_2);
        else if(mode == MODE_REF)
            drawCirclesInners(gui, data, CHANNEL_3);
    }
}

void Draw::drawGettingStarted(Gui &gui){
    if(gui.menu.windowGSActive){
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BG_BASE_COLOR_DARK));
        GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(BLACK));
        gui.menu.windowGSActive = !GuiWindowBox(gui.menu.recGS, "Getting started");
        initStyle();

        // List of options
        GuiSetStyle(LISTVIEW, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
        GuiSetStyle(LISTVIEW, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        gui.menu.gsMenuActive = GuiListView(gui.menu.recGSMenu, gui.menu.gsMenuOptions, &gui.menu.gsMenuScrollIndex, gui.menu.gsMenuActive);

        // Visualization panel
        DrawRectangleRec(gui.menu.recGSPanel, WHITE);
        if(gui.menu.gsMenuActive > -1){
            drawGSContent(gui, gui.menu.recGSPanel, gui.menu.gsMenuActive, gui.menu.imgPadding[gui.menu.gsMenuActive]);
        }
    }
}

void Draw::drawCirclePrior(Gui &gui, Data &data){
    DrawCircle(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR);
    DrawCircleLines(data.priorCircle.center.x, data.priorCircle.center.y, data.priorCircle.radius, PRIOR_COLOR_LINES);
    if(gui.showLabels) DrawTextEx(gui.defaultFontBig, gui.visualization.labelPriorCircTxt, (Vector2) {gui.visualization.recLabelPriorCirc.x, gui.visualization.recLabelPriorCirc.y}, gui.defaultFontBig.baseSize, 1.0, BLACK);
}

void Draw::drawCirclesInners(Gui &gui, Data &data, int channel){
	for(int i = 0; i < data.hyper[channel].num_post; i++){
        Color colorFill, colorLines;
        if(channel == CHANNEL_1){
            colorFill = INNERS1_COLOR;
            colorLines = INNERS1_COLOR_LINES;
        }else{
            colorFill = INNERS2_COLOR;
            colorLines = INNERS2_COLOR_LINES;
        }

        DrawCircle(data.innersCircles[channel][i].center.x, data.innersCircles[channel][i].center.y, data.innersCircles[channel][i].radius, colorFill);
        DrawCircleLines(data.innersCircles[channel][i].center.x, data.innersCircles[channel][i].center.y, data.innersCircles[channel][i].radius, colorLines);
        
        // Decide to write label inside or outside the circle
        float threshold;
        if(channel == CHANNEL_1)
            threshold = 0.13f;
        else
            threshold = 0.20f;
        
        if(gui.showLabels){
            if(data.hyper[channel].outer.prob[i] < threshold)
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.labelPostTxt[channel][i][0]), (Vector2) {gui.visualization.recLabelInnersCirc[channel][i].x-25, gui.visualization.recLabelInnersCirc[channel][i].y-25}, 26, 1.0, BLACK);
            else
                DrawTextEx(gui.defaultFontBig, &(gui.posteriors.labelPostTxt[channel][i][0]), (Vector2) {gui.visualization.recLabelInnersCirc[channel][i].x-5, gui.visualization.recLabelInnersCirc[channel][i].y-5}, 26, 1.0, BLACK);
        }

        if(gui.showCHull){
            // Find convex hull using inners circles
            vector<pt> points = vector<pt>(gui.posteriors.numPost[channel]);
            for(int i = 0; i < gui.posteriors.numPost[channel]; i++){
                points[i].x = data.innersCircles[channel][i].center.x;
                points[i].y = data.innersCircles[channel][i].center.y;
            }

            convexHull(points);
            int n = (int) points.size();
            for(int i = 0; i < n-1; i++){
                if(channel == CHANNEL_1){
                    DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, INNERS1_COLOR_LINES);
                    DrawTriangle(
                        (Vector2){(float)points[0].x, (float)points[0].y},
                        (Vector2){(float)points[i].x, (float)points[i].y},
                        (Vector2){(float)points[i+1].x, (float)points[i+1].y},
                        CH1_COLOR
                    );
                }else{
                    DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, INNERS2_COLOR_LINES);
                    DrawTriangle(
                        (Vector2){(float)points[0].x, (float)points[0].y},
                        (Vector2){(float)points[i].x, (float)points[i].y},
                        (Vector2){(float)points[i+1].x, (float)points[i+1].y},
                        CH2_COLOR
                    );
                }

            }

            // Line from last point to the first
            if(channel == CHANNEL_1)
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS1_COLOR_LINES);
            else
                DrawLine(points[n-1].x, points[n-1].y, points[0].x, points[0].y, INNERS2_COLOR_LINES);
        }
	}
}

void Draw::drawContentPanel(Rectangle layoutTitle, Rectangle layoutContent, char *title, Color contentColor, Font font){
    DrawRectangleRec(layoutTitle, TITLES_BASE_COLOR);
    DrawRectangleRec(layoutContent, contentColor);
    DrawTextEx(font, title, (Vector2){layoutTitle.x + 10, layoutTitle.y}, font.baseSize, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}

void Draw::drawContentDropBox(Gui &gui, Rectangle layoutTitle, Rectangle layoutContent, Color contentColor, Font font){
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    if(GuiDropdownBox(layoutTitle, layoutTitle.width, gui.channel.ddDPTxt, &(gui.channel.ddDPActive), gui.channel.ddDPEdit)) gui.channel.ddDPEdit = !gui.channel.ddDPEdit;
    gui.channel.updateDDDP();
    initStyle();
}

void Draw::drawGSContent(Gui &gui, Rectangle panel, int option, int imgPadding){
    /* option: GS_PRIOR, GS_CHANNEL, GS_HYPER, GS_VISUALIZATION, GS_REFINEMENT
       imgPadding: Space between description and image
    */

    // Description
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, 0);
    
    gui.menu.scrPanContent.y = gui.menu.recGS.height + gui.menu.gsOptionYOffset[option];

    Rectangle viewScroll = GuiScrollPanel(
        sum2Rec(gui.menu.recScrPan, 0, 0, -gui.menu.scrPanBOffset.x, -gui.menu.scrPanBOffset.y),
        (Rectangle){gui.menu.recScrPan.x, gui.menu.recScrPan.y, gui.menu.scrPanContent.x, gui.menu.scrPanContent.y},
        &(gui.menu.scrPanOffset)
    );
    
    BeginScissorMode(viewScroll.x, viewScroll.y, viewScroll.width, viewScroll.height);
        GuiTextBoxMulti(
            (Rectangle){panel.x+10+gui.menu.scrPanOffset.x, panel.y+10+gui.menu.scrPanOffset.y, panel.width-30, (float)gui.menu.gsContentHeight[option]},
            gui.menu.gsDescriptionTexts[option],
            gui.defaultFont.baseSize,
            false
        );
        
        // Image
        if(strcmp(gui.menu.imagesSrc[option], "")){
            DrawTextureEx(gui.menu.gsImages[option], (Vector2){panel.x+10+gui.menu.scrPanOffset.x, panel.y+imgPadding+gui.menu.scrPanOffset.y}, 0.0f, 0.43f, WHITE);
        }
    EndScissorMode();


    initStyle();
}

void Draw::drawHelpMessage(Gui &gui, Rectangle rec, char message[CHAR_BUFFER_SIZE]){
    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, 5);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(BG_BASE_COLOR_LIGHT));
    int oldState = GuiGetState();
    GuiSetState(GUI_STATE_DISABLED);
    GuiTextBoxMulti(rec, message, gui.defaultFont.baseSize, false);
    GuiSetState(oldState);
}

void Draw::drawTab(Gui &gui, int channel, bool active){
    int oldState = GuiGetState();
    int mode = gui.menu.ddActive[BUTTON_MODE]; // Mode can be only MODE_TWO and MODE_REF

    if(gui.drawing){
        // Text color
        if(channel == CHANNEL_1 || channel == CHANNEL_3 || (channel == CHANNEL_2 && mode == MODE_TWO)){
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
        }else if(channel == CHANNEL_2 && mode == MODE_REF){
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        }

        // Button color
        if(channel == CHANNEL_1){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS1_COLOR_L1));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS1_COLOR_D2));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS1_COLOR_L1));
        }else if(channel == CHANNEL_3){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS2_COLOR_L2));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS2_COLOR_D1));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS2_COLOR_L2));
        }

        if(channel == CHANNEL_2 && mode == MODE_TWO){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(INNERS2_COLOR_L2));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(INNERS2_COLOR_D1));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(INNERS2_COLOR_L2));
        }else if(channel == CHANNEL_2 && mode == MODE_REF){
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BG_BASE_COLOR_LIGHT));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(BG_BASE_COLOR_DARK));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
        }
    }else{
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BG_BASE_COLOR_LIGHT));
        GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(TITLES_BASE_COLOR_DARKER));
    }
    
    if(active) GuiSetState(GUI_STATE_PRESSED);

    if(GuiButton(gui.channel.recTabs[channel], gui.channel.labelChannelTabs[channel])) controls.buttonsTabs(gui, channel);
    GuiSetState(oldState);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(MENU_BASE_COLOR_NORMAL));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(MENU_BASE_COLOR_PRESSED));
}
