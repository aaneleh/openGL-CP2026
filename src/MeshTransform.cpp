#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();
int loadSimpleOBJ(string filePATH, int &nVertices, string &textureFile);
void loadSimpleMTL(string filePATH, string &textureImage);

const GLuint WIDTH = 1000, HEIGHT = 1000;

string OBJECT_FILE = "Suzanne.obj";

float SPEED = 0.1; //o mover e escala vai aumentar nesse valor
float rotateX=0.0, rotateY=0.0, rotateZ=0.0;
float moveX=0.0, moveZ=0.0, moveY=0.0, scale=0.4;
int selectedObject = -1;

struct Mesh {
    GLuint VAO;
	string meshFile;
	string textureFile;
	string textureImage;
	glm::vec3 position;
	glm::vec3 rotate;
	glm::vec3 scale;
	/* float scale; */
	int nVertices;
};

vector<Mesh> objects;
Mesh createMesh(string objPath, glm::vec3 position, float scale);
void render(glm::mat4 model, GLint modelLoc, Mesh object);

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

int main(){
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modulo 3 - Mesh - Helena!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

    objects.push_back(createMesh("../assets/Modelos3d/"+OBJECT_FILE, glm::vec3(1.0,1.0,1.0), 0.2));
    objects.push_back(createMesh("../assets/Modelos3d/"+OBJECT_FILE, glm::vec3(0.0,0.0,0.0), 0.2));

    glUseProgram(shaderID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)){
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

        for(int i = 0; i < objects.size(); i ++){
            render(model, modelLoc, objects[i]);
        }

		glfwSwapBuffers(window);
	}
    for(int i = 0; i < objects.size(); i ++){
        glDeleteVertexArrays(1, &objects[i].VAO);
    }
	glfwTerminate();
	return 0;
}

// Função de callback de teclado -  É chamada sempre que uma tecla for pressionada
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

    else if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        selectedObject = 0;
        std::cout << "Objeto selecionado: 1" << std::endl;

    }  else if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        selectedObject = 1;
        std::cout << "Objeto selecionado: 2" << std::endl;
    } 

    if(selectedObject > -1){
        //ROTAÇÃO
        if (key == GLFW_KEY_X){
            objects[selectedObject].rotate.x = action != GLFW_RELEASE;
        } else if (key == GLFW_KEY_Y){
            objects[selectedObject].rotate.y = action != GLFW_RELEASE;
        } else if (key == GLFW_KEY_Z) {
            objects[selectedObject].rotate.z = action != GLFW_RELEASE;
        } 


        //Controle de mover (transladar) WASD e IJ
        else if (key == GLFW_KEY_W){
            objects[selectedObject].position.y += action != GLFW_RELEASE ? SPEED : 0;
        } else if (key == GLFW_KEY_S){
            objects[selectedObject].position.y += action != GLFW_RELEASE ? -SPEED : 0;

        } else if (key == GLFW_KEY_D){
            objects[selectedObject].position.x += action != GLFW_RELEASE ? SPEED : 0;
        } else if (key == GLFW_KEY_A){
            objects[selectedObject].position.x += action != GLFW_RELEASE ? -SPEED : 0;

        } else if (key == GLFW_KEY_I){
            objects[selectedObject].position.z += action != GLFW_RELEASE ? SPEED : 0;
        } else if (key == GLFW_KEY_J){
            objects[selectedObject].position.z += action != GLFW_RELEASE ? -SPEED : 0;
        }

        //Controle de escala uniforme: [ para diminuir e ] para aumentar
        if (key == GLFW_KEY_Q){
            objects[selectedObject].scale.x += action != GLFW_RELEASE ? -SPEED : 0;
            objects[selectedObject].scale.y += action != GLFW_RELEASE ? -SPEED : 0;
            objects[selectedObject].scale.z += action != GLFW_RELEASE ? -SPEED : 0;
        } else if (key == GLFW_KEY_E){
            objects[selectedObject].scale.x += action != GLFW_RELEASE ? SPEED : 0;
            objects[selectedObject].scale.y += action != GLFW_RELEASE ? SPEED : 0;
            objects[selectedObject].scale.z += action != GLFW_RELEASE ? SPEED : 0;
        } else if(key == GLFW_KEY_J){
            objects[selectedObject].scale.x += action != GLFW_RELEASE ? SPEED : 0;
        } else if(key == GLFW_KEY_I){
            objects[selectedObject].scale.y += action != GLFW_RELEASE ? SPEED : 0;
        } else if(key == GLFW_KEY_K){
            objects[selectedObject].scale.z += action != GLFW_RELEASE ? SPEED : 0;
        } 
    }

}

// Compilar e "buildar" um programa de shader simples e único neste exemplo de código
// Retorna o identificador do programa de shader
int setupShader(){
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int loadSimpleOBJ(string filePATH, int &nVertices, string &textureFile) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<GLfloat> vBuffer;
    glm::vec3 color = glm::vec3(1.0, 0.0, 0.0); //não é mais usado mas sem isso não builda

    std::ifstream arqEntrada(filePATH.c_str());
    if (!arqEntrada.is_open()) {
        std::cerr << "Erro ao tentar ler o arquivo " << filePATH << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(arqEntrada, line)) {
        std::istringstream ssline(line);
        std::string word;
        ssline >> word;

        if (word == "mtllib") {
            ssline >> textureFile;

		} else if (word == "v") {
            glm::vec3 vertice;
            ssline >> vertice.x >> vertice.y >> vertice.z;
            vertices.push_back(vertice);

        } else if (word == "vn") {
            glm::vec3 normal;
            ssline >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);

		} else if (word == "vt") {
            glm::vec2 vt;
            ssline >> vt.s >> vt.t;
            texCoords.push_back(vt);
			
        } else if (word == "f") {
            while (ssline >> word) {
                int vi = 0, ti = 0, ni = 0;
                std::istringstream ss(word);
                std::string index;

                if (std::getline(ss, index, '/')) vi = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index, '/')) ti = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index)) ni = !index.empty() ? std::stoi(index) - 1 : 0;

                vBuffer.push_back(vertices[vi].x);
                vBuffer.push_back(vertices[vi].y);
                vBuffer.push_back(vertices[vi].z);
                vBuffer.push_back(normals[ni].x); //normal
                vBuffer.push_back(normals[ni].y); //normal
                vBuffer.push_back(normals[ni].z); //normal
                vBuffer.push_back(texCoords[ti].s); //s
                vBuffer.push_back(texCoords[ti].t); //t
            }
        }
    }

    arqEntrada.close();

    std::cout << "Gerando o buffer de geometria..." << std::endl;
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	nVertices = vBuffer.size() / 8;  // x, y, z, normais (x, y, z), s, t

    return VAO;
}

void loadSimpleMTL(string filePATH, string &textureImage) {

    std::ifstream arqEntrada(filePATH.c_str());

    if (!arqEntrada.is_open()) {
        std::cerr << "Erro ao tentar ler o arquivo " << filePATH << std::endl;
    }

    std::string line;
    while (std::getline(arqEntrada, line)) {
        std::istringstream ssline(line);
        std::string word;
        ssline >> word;

        if (word == "map_Kd") 
            ssline >> textureImage;
	}

    arqEntrada.close();
}

Mesh createMesh(string objPath, glm::vec3 position, float scale) {
    Mesh mesh;
	mesh.meshFile = objPath;
	mesh.VAO = loadSimpleOBJ(mesh.meshFile, mesh.nVertices, mesh.textureFile);
	mesh.position = position;
	mesh.rotate = glm::vec3(0.0,0.0,0.0);
	mesh.scale = glm::vec3(scale,scale,scale);
	loadSimpleMTL("../assets/Modelos3d/"+mesh.textureFile,mesh.textureImage);

    return mesh;
}

void render(glm::mat4 model, GLint modelLoc, Mesh object){
    float angle = (GLfloat)glfwGetTime();

    model = glm::mat4(1); 
    model = glm::translate(model, glm::vec3(object.position.x, object.position.y, object.position.z));
    model = glm::scale(model, glm::vec3(object.scale.x, object.scale.y, object.scale.z));

    if(object.rotate.x != 0.0 || object.rotate.y != 0.0 || object.rotate.z != 0.0) 
        model = glm::rotate(model, angle, object.rotate);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(object.VAO);
    glDrawArrays(GL_TRIANGLES, 0, object.nVertices);
}