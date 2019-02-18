// Standard inclues
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <map>
// Other includes
#include "./include/shader.hpp"
#include "./include/graphics.hpp"
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H 
// ImGui
#include "./imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_internal.h"

#define MAIN_V_SHADER "./shaders/main_v_shader.glsl"
#define MAIN_F_SHADER "./shaders/main_f_shader.glsl"
#define TEXT_V_SHADER "./shaders/text_v_shader.glsl"
#define TEXT_F_SHADER "./shaders/text_f_shader.glsl"
#define FREESANS_PATH "./fonts/FreeSans.ttf"

#define MAX_OUTPUTS 100 // Max number of outputs

// ImGui
#define WINDOW_COLOR 0.75f, 0.75f, 0.75f
#define TEXT_COLOR ImVec4(0.0f, 0.0f, 0.0f, 1.0f)
#define POPUP_COLOR ImVec4(0.65f, 0.65f, 0.65f, 1.0f)
#define FRAME_COLOR ImVec4(0.26f, 0.39f, 0.578f, 1.0f)
#define BUTTON_COLOR ImVec4(0.260f, 0.590f, 0.980f, 0.400f)

using namespace std;

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

/* A callbakc function used to reset the window coordinates when 
 * the user resizes the window. */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

/* This function is called in the main loop, and it threats
 * users' input (from keyboard, mouse, ...). */
void processInput(GLFWwindow *window, unsigned int *VAO, unsigned int *VBO, Hyper &hyper, bool *moveCircles, Point &oldMousePos);

/* Using the given hyper distribution, create the circles for prior and posterior distributions */
void buildCircles(unsigned int *VAO, unsigned int *VBO, Hyper &hyper);

/* Load the 128 character of ASCII. */
void loadCharacters(FT_Face &face);

/* Render the given text in the screen in the given coordinates. */
void RenderText(unsigned int VAO, unsigned int VBO, Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

static void ShowHelpMarker(const char* desc);

int main(){

    // Auxiliar variables
    Point p;

    unsigned int *VAO, *VBO;
    int window_width, window_height;

    // Initialize GLFW Library
    if(glfwInit() == GL_FALSE){
        fprintf(stderr, "Error initializing GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Tell to GLFW that we want to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell to GLFW that we want to use the OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window using GLFW
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "QIF Graphics", NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set the window position to the center of the monitor
    const GLFWvidmode *video = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, (video->width - WINDOW_WIDTH)/2, (video->height - WINDOW_HEIGHT)/2);

    // Makes the context of the specified window current
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    Shader textShader(TEXT_V_SHADER, TEXT_F_SHADER);
    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT);
	textShader.use();
   	textShader.setMat4("projection", projection); 

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, FREESANS_PATH, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; 

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    loadCharacters(face);

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

   	
    GLuint VAOText, VBOText;
    glGenVertexArrays(1, &VAOText);
    glGenBuffers(1, &VBOText);

    glBindVertexArray(VAOText);
        glBindBuffer(GL_ARRAY_BUFFER, VBOText);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);      


    /* Tell to GLFW we want to call the function "framebuffer_size_callback" 
     * when the user resizes the window */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(EXIT_FAILURE);
    }

    Shader mainShader(MAIN_V_SHADER, MAIN_F_SHADER);

    float mainTriangle[] = {
        0.0f+ORIGIN_X, 0.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f+ORIGIN_X, 1.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f+ORIGIN_X, 0.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f
    };

    // QIF ----------------
    Hyper hyper("prior", "channel");
    // --------------------
    
    VAO = (unsigned int *) malloc((hyper.channel->num_out + 2) * sizeof(unsigned int));
    VBO = (unsigned int *) malloc((hyper.channel->num_out + 2) * sizeof(unsigned int));

    /* VAO[0] -> Main triangle
     * VAO[1] -> Prior
     * VAO[2]...VAO[n] -> Posteriors */
    glGenVertexArrays(hyper.channel->num_out + 2, VAO);
    glGenBuffers(hyper.channel->num_out + 2, VBO);
    
    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO[0]);
        // 2. copy our vertices array in a buffer for OpenGL to use
        // Main Triangle
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mainTriangle), mainTriangle, GL_STATIC_DRAW);
        // 3. then set the vertex attributes pointers
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("./fonts/FreeSans.ttf", 20.0f);
    // ------------

    bool moveCircles = false;
    Point oldMousePos;
    buildCircles(VAO, VBO, hyper);
    double x1, x2, x3;
    x1 = x2 = x3 = 0.0f;
    int number_outputs = 0;
    bool prior_is_a_distribution = true;
    bool draw_circles = false;
    int error = 0;
    
    // Initialize the channel
    double channel[3][MAX_OUTPUTS];
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < MAX_OUTPUTS; j++)
            channel[i][j] = 0;
            
    while(!glfwWindowShouldClose(window)){
        // Input
        processInput(window, VAO, VBO, hyper, &moveCircles, oldMousePos);
        
        // Rendering commands here
        glClearColor(WINDOW_COLOR, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfwGL3_NewFrame();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        screenCoord2PixelCoord(mainTriangle[0], mainTriangle[1], p, WINDOW_WIDTH, WINDOW_HEIGHT);
        RenderText(VAOText, VBOText, textShader, "x2", p.x - 45, WINDOW_HEIGHT - p.y, 0.45f, glm::vec3(0.0f, 0.0f, 0.0f));

        screenCoord2PixelCoord(mainTriangle[6], mainTriangle[7], p, WINDOW_WIDTH, WINDOW_HEIGHT);
        RenderText(VAOText, VBOText, textShader, "x1", p.x - 5, WINDOW_HEIGHT - p.y + 20, 0.45f, glm::vec3(0.0f, 0.0f, 0.0f));
        
        screenCoord2PixelCoord(mainTriangle[12], mainTriangle[13], p, WINDOW_WIDTH, WINDOW_HEIGHT);
        RenderText(VAOText, VBOText, textShader, "x3", p.x + 10, WINDOW_HEIGHT - p.y, 0.45f, glm::vec3(0.0f, 0.0f, 0.0f));

		// Option bar		
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameBorderSize = 1.0f;
		style.GrabRounding = 0.0f;
		style.Colors[0] = TEXT_COLOR; // Window background color
		style.Colors[4] = POPUP_COLOR; // Pop up background color
        style.Colors[7] = FRAME_COLOR; // Frame background color
		style.WindowBorderSize = 0.0f; // No borders
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		
		ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			ImGui::Text("Enter the probability distribution on the set of secrets {x1,x2,x3}\n\n");
			
            ImGui::PushItemWidth(80);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("x1 "); ImGui::SameLine();
            ImGui::InputDouble("##x1", &x1, 0.0f, 0.0f, "%.4f", ImGuiInputTextFlags_CharsScientific | (draw_circles ? ImGuiInputTextFlags_ReadOnly : 0)); ImGui::SameLine();
            ShowHelpMarker("Probability of x1 occurs");
            ImGui::AlignTextToFramePadding();
            ImGui::Text("x2 "); ImGui::SameLine();
            ImGui::InputDouble("##x2", &x2, 0.0f, 0.0f, "%.4f", ImGuiInputTextFlags_CharsScientific | (draw_circles ? ImGuiInputTextFlags_ReadOnly : 0)); ImGui::SameLine();
            ShowHelpMarker("Probability of x2 occurs"); ImGui::SameLine();
            ImGui::Text("        "); ImGui::SameLine();
            if (ImGui::Button("Clean", ImVec2(80, 30))) x1 = x2 = x3 = 0.0f;                
            ImGui::AlignTextToFramePadding();
            ImGui::Text("x3 "); ImGui::SameLine();
            ImGui::InputDouble("##x3", &x3, 0.0f, 0.0f, "%.4f", ImGuiInputTextFlags_CharsScientific | (draw_circles ? ImGuiInputTextFlags_ReadOnly : 0)); ImGui::SameLine();
            ShowHelpMarker("Probability of x3 occurs");
            ImGui::Text("\n\n");
            ImGui::PopItemWidth();
            
            // Distribution
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Number of outputs"); ImGui::SameLine();
            ImGui::PushItemWidth(90);
            ImGui::InputInt("##num_out", &number_outputs);
            if(number_outputs < 0) number_outputs = 0;
            ImGui::PopItemWidth();

            ImGui::Text("\n\nEnter the channel\n");
            ImGui::PushItemWidth(50);
            style.FrameBorderSize = 0.0f;
            style.Colors[7] = ImVec4(WINDOW_COLOR, 1.0f);
            char h[7] = "p(y|x)";
            ImGui::InputText("##00", h, sizeof(h), ImGuiInputTextFlags_ReadOnly);

            for(int i = 0; i < number_outputs; i++){
                char buf1[64], buf2[64];
                sprintf(buf1, "##0%d", i+1);
                sprintf(buf2, "   y%d", i+1);

                ImGui::SameLine();
                ImGui::InputText(buf1, buf2, sizeof(buf2), ImGuiInputTextFlags_ReadOnly);
            }

            for(int i = 0; i < 3; i++){
                for(int j = 0; j <= number_outputs; j++){
                    char buf[64];
                    sprintf(buf, "##%d%d", i+1, j);

                    if(j == 0){
                        style.FrameBorderSize = 0.0f;
                        style.Colors[7] = ImVec4(WINDOW_COLOR, 1.0f);
                        char buf2[8];
                        sprintf(buf2, "    x%d", i+1);
                        ImGui::InputText(buf, buf2, sizeof(buf2), ImGuiInputTextFlags_ReadOnly);
                    }else{
                        style.FrameBorderSize = 1.0f;
                        style.Colors[7] = FRAME_COLOR;
                        ImGui::SameLine();
                        ImGui::InputDouble(buf, &(channel[i][j-1]), 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsScientific | (draw_circles ? ImGuiInputTextFlags_ReadOnly : 0));
                    }
                }
            }
            ImGui::PopItemWidth();
            
            ImGui::Text("\n\n");
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Click to draw"); ImGui::SameLine();
            if(draw_circles)
                style.Colors[24] = ImVec4(0.260f, 0.590f, 0.980f, 1.0f);
            if(ImGui::ButtonEx("Draw", ImVec2(80, 30), draw_circles ? ImGuiButtonFlags_Disabled : 0)){
                vector<long double> prob({x1,x2,x3}); // Prior
                vector<vector<long double> > ch(3, vector<long double>(number_outputs)); // Channel
                
                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < number_outputs; j++)
                        ch[i][j] = (long double) channel[i][j];

                if(!Distribution::isDistribution(prob)){
                    error = 1;
                }else if(!Channel::isChannel(ch)){
                    error = 2;
                }else{
                    error = 0;
                    hyper.prior->prob = prob;
                    hyper.channel = new Channel(*(hyper.prior), ch);
                    hyper.buildHyper(*(hyper.prior), *(hyper.channel));
                    buildCircles(VAO, VBO, hyper);
                    draw_circles = true;
                }
            }

            if(error == 1){
                ImGui::Text("The numbers in the prior are not a probability distribution!");
            }else if(error == 2){
                ImGui::Text("The matrix is not a channel!");
            }

            style.Colors[24] = BUTTON_COLOR;
            if(draw_circles){
                // Draw inners

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Click to set the probabilities again"); ImGui::SameLine();
                if (ImGui::Button("Set", ImVec2(80, 30)))
                    draw_circles = false;


                ImGui::Text("\n\nInner distributions\n");
                ImGui::PushItemWidth(50);
                style.FrameBorderSize = 0.0f;
                style.Colors[7] = ImVec4(WINDOW_COLOR, 1.0f);
                char h[7] = "p(x|y)";
                ImGui::InputText("##i00", h, sizeof(h), ImGuiInputTextFlags_ReadOnly);

                for(int i = 0; i < number_outputs; i++){
                    char buf1[64], buf2[64];
                    sprintf(buf1, "##i0%d", i+1);
                    sprintf(buf2, "   y%d", i+1);

                    ImGui::SameLine();
                    ImGui::InputText(buf1, buf2, sizeof(buf2), ImGuiInputTextFlags_ReadOnly);
                }

                for(int i = 0; i < 3; i++){
                    for(int j = 0; j <= number_outputs; j++){
                        char buf[64];
                        sprintf(buf, "##i%d%d", i+1, j);

                        if(j == 0){
                            style.FrameBorderSize = 0.0f;
                            style.Colors[7] = ImVec4(WINDOW_COLOR, 1.0f);
                            char buf2[8];
                            sprintf(buf2, "    x%d", i+1);
                            ImGui::InputText(buf, buf2, sizeof(buf2), ImGuiInputTextFlags_ReadOnly);
                        }else{
                            style.FrameBorderSize = 1.0f;
                            style.Colors[7] = FRAME_COLOR;
                            ImGui::SameLine();
                            char buf2[64];
                            sprintf(buf2, "%.3Lf", hyper.inners[i][j-1]);
                            ImGui::InputText(buf, buf2, sizeof(buf2), ImGuiInputTextFlags_ReadOnly);
                        }
                    }
                }
            }
        ImGui::End();

			

        // Draw ---------------------------------
            mainShader.use();
            
            // Main Triangle
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO[0]);          
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

        if(draw_circles){
			// Circles
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for(int i = 1; i <= hyper.channel->num_out+1; i++){
				glBindVertexArray(VAO[i]);
				glDrawArrays(GL_TRIANGLE_FAN, 0, (CIRCLES_SIDES+2));
				glBindVertexArray(0);
			}
			
			// Circles' names
			stringstream priorInfo;
			priorInfo << "Prior: " << fixed << setprecision(4) << hyper.prior->prob[0] << "  " << hyper.prior->prob[1] << "  " << hyper.prior->prob[2];
			// RenderText(VAOText, VBOText, textShader, priorInfo.str(), 20.0f, WINDOW_HEIGHT - 40.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
			
			// Prior
			dist2BaryCoord(*(hyper.prior), p);
			p.x += (ORIGIN_X - 0.05f);
			p.y += (ORIGIN_Y - 0.02f);
			screenCoord2PixelCoord(p.x, p.y, p, WINDOW_WIDTH, WINDOW_HEIGHT);
			RenderText(VAOText, VBOText, textShader, "prior", p.x, WINDOW_HEIGHT - p.y, 0.35f, glm::vec3(0.0f, 0.0f, 0.0f));
			
			// Inners
			for(int i = 0; i < hyper.channel->num_out; i++){
				stringstream info, circleName;
				circleName << "y" << i+1;
				
				long double x1 = hyper.inners[0][i];
				long double x2 = hyper.inners[1][i];
				long double x3 = hyper.inners[2][i];
				
				dist2BaryCoord(x1,x2,x3, p);
				p.x += (ORIGIN_X - 0.02f);
				p.y += (ORIGIN_Y - 0.02f);
				
				info << circleName.str() << ": " << fixed << setprecision(4) << hyper.outer.prob[i];
				// RenderText(VAOText, VBOText, textShader, info.str(), 20.0f, WINDOW_HEIGHT - (40.0f*(i+2)), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
				
				// Do not write the text if the output probability is 0
				if(hyper.outer.prob[i] == 0.0f)
					continue;
				
				screenCoord2PixelCoord(p.x, p.y, p, WINDOW_WIDTH, WINDOW_HEIGHT);
				RenderText(VAOText, VBOText, textShader, circleName.str(), p.x, WINDOW_HEIGHT - p.y, 0.35f, glm::vec3(0.0f, 0.0f, 0.0f));
			}
        }
        // --------------------------------------

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(hyper.channel->num_out+2, VAO);
    glDeleteBuffers(hyper.channel->num_out+2, VBO);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // glfwSetWindowAspectRatio(window, 1, 1);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, unsigned int *VAO, unsigned int *VBO, Hyper &hyper, bool *moveCircles, Point &oldMousePos){

    Point mousePos, priorPos, newPriorPos;

    // If the user presses the key ESC, close the window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // If the user presses the mouse left button
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        pixelCoord2ScreenCoord(xpos, ypos, mousePos, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Get prior position
        dist2BaryCoord(*(hyper.prior), priorPos);
        priorPos.x += ORIGIN_X;
        priorPos.y += ORIGIN_Y;

        if(*moveCircles == false && euclidianDistance(mousePos, priorPos) <= PRIOR_RADIUS){
            *moveCircles = true;
            oldMousePos.x = mousePos.x;
            oldMousePos.y = mousePos.y;
        }else{
            newPriorPos.x = mousePos.x - oldMousePos.x;
            newPriorPos.y = mousePos.y - oldMousePos.y;
            oldMousePos.x = mousePos.x;
            oldMousePos.y = mousePos.y;
        }
    }else{
        *moveCircles = false;
    }

    if(*moveCircles == true){
        vector<long double> prob(3);
        newPriorPos.x += priorPos.x;
        newPriorPos.y += priorPos.y;

        // Check if the new vector of 3 elements make up a probability distribution
        if(baryCoord2Dist(newPriorPos, prob)){
            baryCoord2Dist(newPriorPos, hyper.prior->prob);
            if(hyper.prior->prob[0] >= 0.985f){
            	hyper.prior->prob[0] = 1.0f;
            	hyper.prior->prob[1] = 0.0f;
            	hyper.prior->prob[2] = 0.0f;
            }else if(hyper.prior->prob[1] >= 0.985f){
            	hyper.prior->prob[1] = 1.0f;
            	hyper.prior->prob[0] = 0.0f;
            	hyper.prior->prob[2] = 0.0f;
            }else if(hyper.prior->prob[2] >= 0.985f){
            	hyper.prior->prob[2] = 1.0f;
            	hyper.prior->prob[0] = 0.0f;
            	hyper.prior->prob[1] = 0.0f;
            }
            hyper.channel = new Channel(*(hyper.prior), hyper.channel->matrix);
            hyper.buildHyper(*(hyper.prior), *(hyper.channel));

            buildCircles(VAO, VBO, hyper);
        }
    }
}

void buildCircles(unsigned int *VAO, unsigned int *VBO, Hyper &hyper){

    /* Used to draw circles
     * 6 = 3 position coordinates + 3 rgb color 
     * CIRCLES_SIDES + 2 = number of vertices in a circle */
    double circleVertices[6 * (CIRCLES_SIDES+2)];
    Point p;

    // Prior distribution buffer
    glBindVertexArray(VAO[1]);
        dist2BaryCoord(*(hyper.prior), p);
        createCircle(p.x + ORIGIN_X, p.y + ORIGIN_Y, PRIOR_RADIUS, 0.274f, 0.509f, 0.705, circleVertices);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
        // Position attribute
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Inners distributions buffers
    for(int i = 0; i < hyper.channel->num_out; i++){
        long double x1 = hyper.inners[0][i];
        long double x2 = hyper.inners[1][i];
        long double x3 = hyper.inners[2][i];
        long double radius = sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);

        dist2BaryCoord(x1,x2,x3,p);

        createCircle(p.x + ORIGIN_X, p.y + ORIGIN_Y, radius, 0.196f, 0.803f, 0.196f, circleVertices);

        glBindVertexArray(VAO[i+2]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i+2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
            // Position attribute
            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
            glEnableVertexAttribArray(0);
            // Color attribute
            glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
            glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
}

void RenderText(unsigned int VAO, unsigned int VBO, Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color){
    // Activate corresponding render state  
    s.use();
    s.setVec3("textColor", color);
    // glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadCharacters(FT_Face &face){

    for (GLubyte c = 0; c < 128; c++){
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
}

static void ShowHelpMarker(const char* desc){
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}