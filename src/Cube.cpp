#include <iostream>
#include <string>
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

const GLuint WIDTH = 1000, HEIGHT = 1000;

float SPEED = 0.1; //o mover e escala vai aumentar nesse valor
float rotateX=0.0, rotateY=0.0, rotateZ=0.0;
float moveX=0.0, moveZ=0.0, moveY=0.0, scale=0.4;

struct Cube {
    GLuint VAO;
	glm::vec3 position;
	float scale;
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

	//GLuint VAO = setupGeometry();
	Cube cubeBig;
	cubeBig.VAO = setupGeometry();
	cubeBig.position = glm::vec3(0.3,0.0,0.0);
	cubeBig.scale = 0.5;

	Cube cubeSmall;
	cubeSmall.VAO = setupGeometry();
	cubeSmall.position = glm::vec3(-0.65,0.0,0.0);
	cubeSmall.scale = 0.0;

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


		//CUBO 1
		model = glm::mat4(1); 
		model = glm::translate(model, glm::vec3(cubeBig.position.x+moveX, cubeBig.position.y+moveY, cubeBig.position.z+moveZ));
		model = glm::scale(model, glm::vec3(cubeBig.scale+scale, cubeBig.scale+scale, cubeBig.scale+scale));
		if(rotateX != 0.0 || rotateY != 0.0 || rotateZ != 0.0) 
			model = glm::rotate(model, angle, glm::vec3(rotateX, rotateY, rotateZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(cubeBig.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Quadrados nos vertices
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		//CUBO 2
		//TODO: Criar um Struct que vai armazenar vec3 position e float scale e seu VAO
		//Pra cada cubo chamar um model com os valores do struct + valor da interação do usuário
		model = glm::mat4(1); 
		model = glm::translate(model, glm::vec3(cubeSmall.position.x+moveX, cubeSmall.position.y+moveY, cubeSmall.position.z+moveZ));
		model = glm::scale(model, glm::vec3(cubeSmall.scale+scale, cubeSmall.scale+scale, cubeSmall.scale+scale));
		if(rotateX != 0.0 || rotateY != 0.0 || rotateZ != 0.0) 
			model = glm::rotate(model, angle, glm::vec3(rotateX, rotateY, rotateZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(cubeSmall.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Quadrados nos vertices
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &cubeBig.VAO);
	glDeleteVertexArrays(1, &cubeSmall.VAO);
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

// Criar os buffers que armazenam a geometria de um triângulo
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry(){
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		//x    y    z    r    g    b
		//Base OK
		-0.5, -0.5, -0.5, 58.0/255.0, 58.0/255.0, 58.0/255.0,
		-0.5, -0.5,  0.5, 58.0/255.0, 58.0/255.0, 58.0/255.0,
		0.5, -0.5, -0.5, 58.0/255.0, 58.0/255.0, 58.0/255.0,

		-0.5, -0.5, 0.5, 41/255.0, 41/255.0, 41/255.0,
		0.5, -0.5,  0.5, 41/255.0, 41/255.0, 41/255.0,
		0.5, -0.5, -0.5, 41/255.0, 41/255.0, 41/255.0,

		//Face de trás OK
		-0.5, -0.5, -0.5, 39/255.0, 78/255.0, 0.0,
		-0.5, 0.5, 0.5, 39/255.0, 78/255.0, 0.0,
		-0.5, -0.5, 0.5, 39/255.0, 78/255.0, 0.0,

		-0.5,-0.5, -0.5, 28/255.0, 56/255.0, 0.0,
		-0.5, 0.5, 0.5, 28/255.0, 56/255.0, 0.0,
		-0.5,0.5,-0.5, 28/255.0, 56/255.0, 0.0,

		//Face da esquerda OK
		-0.5,-0.5,0.5, 0, 51/255.0, 254/255.0,
		0.5,0.5,0.5, 0, 51/255.0, 254/255.0,
		0.5,-0.5,0.5, 0, 51/255.0, 254/255.0,
		
		-0.5,-0.5,0.5, 0, 36/255.0, 181/255.0,
		0.5,0.5,0.5, 0, 36/255.0, 181/255.0,
		-0.5,0.5,0.5, 0, 36/255.0, 181/255.0,

		//Face da frente 
		0.5,-0.5,0.5, 142/255.0, 0, 140/255.0,
		0.5,0.5,-0.5, 142/255.0, 0, 140/255.0,
		0.5,-0.5,-0.5, 142/255.0, 0, 140/255.0,

		0.5,-0.5,0.5, 101/255.0, 0, 100/255.0,
		0.5,0.5,-0.5, 101/255.0, 0, 100/255.0,
		0.5,0.5,0.5, 101/255.0, 0, 100/255.0,

		//Face da direita
		0.5,-0.5,-0.5, 139/255.0, 28/255.0, 0,
		-0.5,0.5,-0.5, 139/255.0, 28/255.0, 0,
		-0.5,-0.5,-0.5, 139/255.0, 28/255.0, 0,

		0.5,-0.5,-0.5, 99/255.0, 20/255.0, 0,
		-0.5,0.5,-0.5, 99/255.0, 20/255.0, 0,
		0.5,0.5,-0.5, 99/255.0, 20/255.0, 0,

		//Topo
		-0.5,0.5,0.5, 201/255.0, 201/255.0, 201/255.0,
		0.5,0.5,-0.5, 201/255.0, 201/255.0, 201/255.0,
		0.5,0.5,0.5, 201/255.0, 201/255.0, 201/255.0,

		-0.5,0.5,0.5, 167/255.0, 167/255.0, 167/255.0,
		0.5,0.5,-0.5, 167/255.0, 167/255.0, 167/255.0,
		-0.5,0.5,-0.5, 167/255.0, 167/255.0, 167/255.0,
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}