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

const GLuint WIDTH = 1000, HEIGHT = 1000;

string OBJECT_FILE = "Cube.obj";

float SPEED = 0.1; //o mover e escala vai aumentar nesse valor
float rotateX=0.0, rotateY=0.0, rotateZ=0.0;
float moveX=0.0, moveZ=0.0, moveY=0.0, scale=0.4;

struct Mesh {
    GLuint VAO;
	string meshFile;
	string textureFile;
	glm::vec3 position;
	float scale;
	int nVertices;
};

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modulo 2- Cubo - Helena!", nullptr, nullptr);
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

	Mesh mesh;
	mesh.meshFile = "./assets/Modelos3d/"+OBJECT_FILE;
	mesh.VAO = loadSimpleOBJ(mesh.meshFile, mesh.nVertices, mesh.textureFile);
	mesh.position = glm::vec3(0.0,0.0,0.0);
	mesh.scale = 0.5;

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

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1); 
		model = glm::translate(model, glm::vec3(mesh.position.x+moveX, mesh.position.y+moveY, mesh.position.z+moveZ));
		model = glm::scale(model, glm::vec3(mesh.scale+scale, mesh.scale+scale, mesh.scale+scale));
		if(rotateX != 0.0 || rotateY != 0.0 || rotateZ != 0.0) 
			model = glm::rotate(model, angle, glm::vec3(rotateX, rotateY, rotateZ));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(mesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &mesh.VAO);
	glfwTerminate();
	return 0;
}

// Função de callback de teclado -  É chamada sempre que uma tecla for pressionada
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//ROTAÇÃO
	else if (key == GLFW_KEY_X){
		rotateX = action != GLFW_RELEASE;
	} else if (key == GLFW_KEY_Y){
		rotateY = action != GLFW_RELEASE;
	} else if (key == GLFW_KEY_Z) {
		rotateZ = action != GLFW_RELEASE;
	} 


	//Controle de mover (transladar) WASD e IJ
	else if (key == GLFW_KEY_W){
		moveY += action != GLFW_RELEASE ? SPEED : 0;
	} else if (key == GLFW_KEY_S){
		moveY += action != GLFW_RELEASE ? -SPEED : 0;

	} else if (key == GLFW_KEY_D){
		moveX += action != GLFW_RELEASE ? SPEED : 0;
	} else if (key == GLFW_KEY_A){
		moveX += action != GLFW_RELEASE ? -SPEED : 0;

	} else if (key == GLFW_KEY_I){
		moveZ += action != GLFW_RELEASE ? SPEED : 0;
	} else if (key == GLFW_KEY_J){
		moveZ += action != GLFW_RELEASE ? -SPEED : 0;
	}

	//Controle de escala uniforme: [ para diminuir e ] para aumentar
	else if (key == GLFW_KEY_Q){
		scale += action != GLFW_RELEASE ? -SPEED : 0;
	} else if (key == GLFW_KEY_E){
		scale += action != GLFW_RELEASE ? SPEED : 0;
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

				//Cada linha vai pegar qual vertice é, qual textura, e qual normal
				//Ex.: f 2/1/1 é vertice 2, textura 1 e normal 1
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

	nVertices = vBuffer.size() / 8;  // x, y, z, nx, ny, nz, s, t (valores atualmente armazenados por vértice)

    return VAO;
}