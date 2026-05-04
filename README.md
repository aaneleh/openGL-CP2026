# Computação Gráfica - Híbrido

## Tarefa 1  `Hello3D.cpp`

  
![Local](RESULT.png)

## Tarefa 2  `Cube.cpp`  

- Alterado a geometria para um cubo
- Alterado cores: cada face tem uma cor e cada triangulo uma variação de tom
- Alterada rotação: trocado para float para poder rotacionar até nos 3 eixo ao mesmo tempo
- Adicionado translação: A e D move no eixo X, W e S no eixo Y e I e J no eixo Z
- Adicionado controle de escala: Q diminui a escala e E aumenta
- Adicionado outro cubo na cena: criado um Struct Cube, cada cubo tem sua instância com VAO, posição e escala inicial

![Local](RESULT2.png)

## Tarefa 3  `Mesh.cpp` 

- Carregado arquivo .obj pelo loadSimpleOBJ
- Adicionado atributos de normais e coordenadas de textura (s t) ao VAO
- Renderizado objeto carregado
- Lido nome do arquivo de textura do material (.mtl) do objeto

![Local](RESULT3.png)

## Tarefa 4  `Phong.cpp` 

- Carregado as informações dos vetores normais dos vértices no arquivo .OBJ (vn). 
- 🟨 Recuperar os coeficientes de iluminação ambiente, difusa e especular do arquivo de materiais (.mtl), que serão enviados pela aplicação para o fragment shader, onde calcularemos sua contribuição para a cor do pixel.