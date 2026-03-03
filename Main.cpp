#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <memory>
#include <filesystem>
#include "imgui.h"
#include "backends\imgui_impl_glfw.h"
#include "backends\imgui_impl_opengl3.h"
#include "Main.h"
#include "ImGuiMenus.h"
#include "GameManager.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "WorldObjects.h"
#include "Drawable.h"
#include "Player.h"
#include "PlayerItems.h"
#include "NPC.h"
#include "SaveLoad.h"

#ifdef _DEBUG
    Gamestate state = Gamestate::Playing;
#else
    Gamestate state = Gamestate::MainMenu;
#endif // _DEBUG



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    auto size = (std::pair<float, float>*)glfwGetWindowUserPointer(window);
    if (size) {
        size->first = (float)width;
        size->second = (float)height;
    }
}



Camera* gCamera = nullptr;

int main() {
    /*
    * main function - runs the main loop of the game
    */
    

    glfwInit(); //initalises glfw 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //sets the major (x) version 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //sets the minor (y) version y.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //sets the version to the core profile (doesnt use backwards compatilble features)

    auto windowSize = std::make_pair(800.f, 600.f);

    GLFWwindow* window = glfwCreateWindow((int)windowSize.first, (int)windowSize.second, "Fractued Realms", NULL, NULL);

    if (window == NULL) //checks to see if the window hasnt been created using glfw's enums
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //it inilises opengl here. 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //gets the size of the 
    glfwSetWindowUserPointer(window, &windowSize); //sets a pointer for all functions to refrence 
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //if glad (gpu functions) doesnt load throw an error
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION); //print the version of opengl being used 

    glViewport(0, 0, (int)windowSize.first, (int)windowSize.second); //tells opengl the window is size 800, 600 and starts at 0, 0 

    //initalise imgui 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; //create an io which can be used to place stuff elsewhere
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f)); //make a camera




    io.DisplaySize = ImVec2(windowSize.first, windowSize.second);


    static char bufferUsername[128];
    static char bufferPassword[128];
    std::string name = "";
    std::string password = "";
    std::string errorMessage = "";
    bool LogIn = false;


    while (true) {

        if (glfwWindowShouldClose(window)) abort(); //if the user quits crash the program

        glClear(GL_COLOR_BUFFER_BIT); //clear the screen

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame(); //reset glfw and opengl

        ImGui::NewFrame(); //reset imgui


        if (LogIn) {
            ImGui::Begin("Log In", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetCursorPos(ImVec2(100, 50));
            ImGui::Text(errorMessage.c_str()); //show error messages
            ImGui::SetCursorPos(ImVec2(100, 150));
            ImGui::InputText("press enter your name", bufferUsername, 36); //let user enter text
            ImGui::SetCursorPos(ImVec2(100, 200));
            ImGui::InputText("press enter your password", bufferPassword, 36); //let user enter text
            ImGui::SetCursorPos(ImVec2(100, 250));

            if (ImGui::Button("create an account", ImVec2(200, 50))) {
                errorMessage = ""; //remove any error messages
                LogIn = !LogIn; //flip the bool
                memset(bufferUsername, '\0', sizeof(bufferUsername)); //clear both buffers so that they are empty again 
                memset(bufferPassword, '\0', sizeof(bufferPassword)); //if they are not empty it could mean (depending on the imgui implementation)
                                                                      //that you write after the last guess - e.g.
                                                                      //username = paul, (wrong password) username = paulpaul etc if it just writes it after
            }

            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //draw it
            glfwSwapBuffers(window);
            glfwPollEvents();


            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) { //if the user presses enter try to log in
                try {
                    name = bufferUsername; //set the name
                    password = bufferPassword; //set the password
                    if (SaveLoad::logIn(name, password)) { //try log in
                        break; //if successful break out of the loop and enter the game
                    }
                }
                catch (std::runtime_error const& message) { //if a error message is thrown

                    if (!strcmp(message.what(), "1")) { //error 1 == wrong password
                        errorMessage = "Wrong password";
                    }
                    else if (!strcmp(message.what(), "2")) { //error 2 == wrong username
                        errorMessage = "Username not found";
                    }
                }
            }
        }
        else { //create account

            ImGui::Begin("create an account", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetCursorPos(ImVec2(100, 100));
            ImGui::Text(errorMessage.c_str()); //show error messages
            ImGui::SetCursorPos(ImVec2(100, 150));
            ImGui::InputText("create your name", bufferUsername, 36); //let user enter text
            ImGui::SetCursorPos(ImVec2(100, 200));
            ImGui::InputText("create your password", bufferPassword, 36); //let user enter text
            ImGui::SetCursorPos(ImVec2(100, 250));
            if (ImGui::Button("Sign in", ImVec2(200, 50))) {
                errorMessage = ""; //remove any error messages
                LogIn = !LogIn; //flip the bool
                memset(bufferUsername, '\0', sizeof(bufferUsername)); //clear both buffers so that they are empty again 
                memset(bufferPassword, '\0', sizeof(bufferPassword)); //if they are not empty it could mean (depending on the imgui implementation)
                                                                      //that you write after the last guess - e.g.
                                                                      //username = paul, (wrong password) username = paulpaul etc if it just writes it after
            }

            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //draw it
            glfwSwapBuffers(window);
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                try {
                    name = bufferUsername; //set the username and password
                    password = bufferPassword;

                    if (SaveLoad::createAccount(name, password)) { //try and create the account
                        break; //if succsessful exit the loop
                    }

                }
                catch (std::runtime_error const& message) { //if error
                    if (!strcmp(message.what(), "1")) {
                        errorMessage = "username already taken"; //display the correct message
                    }

                    if (!strcmp(message.what(), "2")) {
                        errorMessage = "illegal characters in name";
                    }

                    if (!strcmp(message.what(), "3")) {
                        errorMessage = "username or password cant be none";
                    }
                }
            }

        }
    }

    Manager manager(camera, name, LogIn); //make a game manager class which takes in the camera.

    manager.mainLoop(window); // run the main loop until the game needs to close 



    //close the game
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
