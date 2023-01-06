#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseClick_callback(GLFWwindow* windows, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
glm::vec3 worldPosFromViewPort(int winX, int winY) ;
unsigned int loadTexture(char const* path, bool gammaCorrection);
unsigned int loadCubemap(std::vector<std::string> faces);
void renderScene(const Shader& shader);
void renderCube();
void renderQuad();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool bloom = true;
bool bloomKeyPressed = false;
float exposure = 1.0f;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int colorBuffers[2];
unsigned int pingpongFBO[2];
unsigned int pingpongColorbuffers[2];
unsigned int rboDepth;


// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

//float cubeVertices[] = {
//     // positions          // normals
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//
//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
// };


//float planeVertices[] = {
//    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//
//     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
//};


//float planeVertices[] = {
//        // positions            // normals         // texcoords
//         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
//        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
//        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
//
//         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
//        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
//         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
//};

float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

float transparentVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

// transparent vegetation locations
// --------------------------------
std::vector<glm::vec3> vegetation
{
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3(1.5f, 0.0f, 0.51f),
    glm::vec3(0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(0.5f, 0.0f, -0.6f)
};


//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//    // positions   // texCoords
//    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//
//    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
//     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
//};



//float quadVertices[] = {
//        // positions     // colors
//        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
//
//        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
//    };


float quadVertices[] = {
    // positions        // texture Coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};


float skyboxVertices[] = {
      // positions
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
  };

std::vector<std::string> faces
   {
       "../../resources/skybox/right.jpg",
       "../../resources/skybox/left.jpg",
       "../../resources/skybox/top.jpg",
       "../../resources/skybox/bottom.jpg",
       "../../resources/skybox/front.jpg",
       "../../resources/skybox/back.jpg"
};


float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};


float _near = 0.1f, _far = 100.0f;
glm::mat4 projection;
glm::mat4 view;


bool LeftButtonHolding = false;
bool RightButtonHolding = false;

struct ModelInfo {
    Model* model_;
    glm::vec3 worldPos_;
    float pitch_;
    float yaw_;
    float roll_;
    bool bIsSelected_;
    std::string name_;
};
std::map<std::string, ModelInfo> gModelsMap_;


// meshes
unsigned int planeVAO;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseClick_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glEnable(GL_MULTISAMPLE);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader shaderLight("../../shaders/light.vs", "../../shaders/light.fs");
    
    Shader ourShader("../../shaders/lighting.vs", "../../shaders/lighting.fs");
    Shader shaderBlur("../../shaders/blur.vs",  "../../shaders/blur.fs");
    Shader shaderBloomFinal("../../shaders/shapes.vs",  "../../shaders/shapes.fs");
    
    // load textures
    // -------------
    unsigned int woodTexture = loadTexture("../../resources/textures/wood.png", true); // note that we're loading the texture as an SRGB texture
    unsigned int containerTexture = loadTexture("../../resources/textures/container2.png", true); // note that we're loading the texture as an SRGB texture
    
    // configure floating point framebuffer
    // ------------------------------------
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    ///create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    /// create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
  
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // ping-pong-framebuffer for blurring
       glGenFramebuffers(2, pingpongFBO);
       glGenTextures(2, pingpongColorbuffers);
       for (unsigned int i = 0; i < 2; i++)
       {
           glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
           glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
           glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
           // also check if framebuffers are complete (no need for depth buffer)
           if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
               std::cout << "Framebuffer not complete!" << std::endl;
       }
    

    // lighting info
   // -------------
   // positions
   std::vector<glm::vec3> lightPositions;
   lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
   lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
   lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
   lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
   // colors
   std::vector<glm::vec3> lightColors;
   lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
   lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
   lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
   lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

    // shader configuration
   // --------------------
   ourShader.use();
   ourShader.setInt("diffuseTexture", 0);
   shaderBlur.use();
   shaderBlur.setInt("image", 0);
   shaderBloomFinal.use();
   shaderBloomFinal.setInt("scene", 0);
   shaderBloomFinal.setInt("bloomBlur", 1);

   // render loop
   // -----------
   while (!glfwWindowShouldClose(window))
   {
       // per-frame time logic
       // --------------------
       float currentFrame = static_cast<float>(glfwGetTime());
       deltaTime = currentFrame - lastFrame;
       lastFrame = currentFrame;

       // input
       // -----
       processInput(window);

       // render
       // ------
       glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       // 1. render scene into floating point framebuffer
      // -----------------------------------------------
       glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
       glm::mat4 view = camera.GetViewMatrix();
       glm::mat4 model = glm::mat4(1.0f);
       ourShader.use();
       ourShader.setMat4("projection", projection);
       ourShader.setMat4("view", view);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, woodTexture);
       // set lighting uniforms
       for (unsigned int i = 0; i < lightPositions.size(); i++)
       {
           ourShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
           ourShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
       }
       ourShader.setVec3("viewPos", camera.position_);
       // create one large cube that acts as the floor
       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
       model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
       ourShader.setMat4("model", model);
       renderCube();
       // then create multiple cubes as the scenery
       glBindTexture(GL_TEXTURE_2D, containerTexture);
       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
       model = glm::scale(model, glm::vec3(0.5f));
       ourShader.setMat4("model", model);
       renderCube();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
       model = glm::scale(model, glm::vec3(0.5f));
       ourShader.setMat4("model", model);
       renderCube();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
       model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
       ourShader.setMat4("model", model);
       renderCube();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
       model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
       model = glm::scale(model, glm::vec3(1.25));
       ourShader.setMat4("model", model);
       renderCube();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
       model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
       ourShader.setMat4("model", model);
       renderCube();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
       model = glm::scale(model, glm::vec3(0.5f));
       ourShader.setMat4("model", model);
       renderCube();

       // finally show all the light sources as bright cubes
       shaderLight.use();
       shaderLight.setMat4("projection", projection);
       shaderLight.setMat4("view", view);

       for (unsigned int i = 0; i < lightPositions.size(); i++)
       {
           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(lightPositions[i]));
           model = glm::scale(model, glm::vec3(0.25f));
           shaderLight.setMat4("model", model);
           shaderLight.setVec3("lightColor", lightColors[i]);
           renderCube();
      }
       glBindFramebuffer(GL_FRAMEBUFFER, 0);

       // 2. blur bright fragments with two-pass Gaussian Blur
       // --------------------------------------------------
       bool horizontal = true, first_iteration = true;
       unsigned int amount = 10;
       shaderBlur.use();
       for (unsigned int i = 0; i < amount; i++)
       {
           glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
           shaderBlur.setInt("horizontal", horizontal);
           glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
           renderQuad();
           horizontal = !horizontal;
           if (first_iteration)
               first_iteration = false;
      }
       glBindFramebuffer(GL_FRAMEBUFFER, 0);

       // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
       // --------------------------------------------------------------------------------------------------------------------------
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       shaderBloomFinal.use();
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
       shaderBloomFinal.setInt("bloom", bloom);
       shaderBloomFinal.setFloat("exposure", exposure);
       renderQuad();

       std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;

       // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
       // -------------------------------------------------------------------------------
           glfwSwapBuffers(window);
           glfwPollEvents();
       }

       glfwTerminate();
       return 0;
   }

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_RIGHT, deltaTime);
    
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bloomKeyPressed)
       {
           bloom = !bloom;
           bloomKeyPressed = true;
       }
       if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
       {
           bloomKeyPressed = false;
       }

       if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
       {
           if (exposure > 0.0f)
               exposure -= 0.001f;
           else
               exposure = 0.0f;
       }
       else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
       {
           exposure += 0.001f;
       }
    
}

glm::vec3 worldPosFromViewPort(int winX, int winY) {
    float winZ;
    glReadPixels(
        winX,
        (int)SCR_HEIGHT - winY
        , 1, 1
        , GL_DEPTH_COMPONENT, GL_FLOAT
        , &winZ);

    float x = (2.0f * (float)winX) / SCR_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * (float)winY) / SCR_HEIGHT;
    float z = winZ * 2.0f - 1.0f;

    float w = (2.0 * _near * _far) / (_far + _near - z * (_far - _near));
//    float w = _near * _far / (_near * winZ - _far * winZ + _far);
    glm::vec4 wolrdPostion(x, y, z, 1);
    wolrdPostion *= w;
    wolrdPostion = glm::inverse(view) * glm::inverse(projection) * wolrdPostion;
    return wolrdPostion;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    }
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (LeftButtonHolding) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
    if (RightButtonHolding) {
        for (std::map<std::string, ModelInfo>::iterator modelsIter = gModelsMap_.begin(); modelsIter != gModelsMap_.end(); modelsIter++)
        {
            if (modelsIter->second.bIsSelected_) {
                modelsIter->second.worldPos_ = worldPosFromViewPort((int)xposIn,(int)yposIn);
                break;
            }
        }
    }
    
}

void mouseClick_callback(GLFWwindow* window, int button, int action, int mods){
    double winX, winY;
    glfwGetCursorPos(window, &winX, &winY);

    if ((action == GLFW_PRESS) && (button == GLFW_MOUSE_BUTTON_LEFT))
    {
        lastX = (float)winX;
        lastY = (float)winY;
        LeftButtonHolding = true;
    }
    else
        LeftButtonHolding = false;
    
    
    if ((action == GLFW_PRESS) && (button == GLFW_MOUSE_BUTTON_RIGHT))
    {
        RightButtonHolding = true;
        glm::vec3 worldPosition = worldPosFromViewPort((int)winX, (int)winY);
        
        for (std::map<std::string, ModelInfo>::iterator modelsIter = gModelsMap_.begin(); modelsIter != gModelsMap_.end(); modelsIter++)
        {
            float distance_ = glm::distance(modelsIter->second.worldPos_, glm::vec3(worldPosition));
            if (distance_ < 2.5) {
                std::cout << modelsIter->first << "模型被选中..." << std::endl;
                modelsIter->second.bIsSelected_ = true;
            }
            else
                modelsIter->second.bIsSelected_ = false;
        }
    }
    else
        RightButtonHolding = false;
    
}



// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}




// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}



// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}




// renders the 3D scene
// --------------------
void renderScene(const Shader& shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    renderCube();
}

