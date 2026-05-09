# Computação Gráfica - Híbrido

## Tarefa 1  `Hello3D.cpp`

  
![Tarefa 1](RESULT.png)

## Tarefa 2  `Cube.cpp`  

- Alterado a geometria para um cubo
- Alterado cores: cada face tem uma cor e cada triangulo uma variação de tom
- Alterada rotação: trocado para float para poder rotacionar até nos 3 eixo ao mesmo tempo
- Adicionado translação: A e D move no eixo X, W e S no eixo Y e I e J no eixo Z
- Adicionado controle de escala: Q diminui a escala e E aumenta
- Adicionado outro cubo na cena: criado um Struct Cube, cada cubo tem sua instância com VAO, posição e escala inicial

![Tarefa 2](RESULT2.png)

## Vivencial 1 `MeshTransform.cpp`

- 🟩 Leitura de arquivos .OBJ (recomenda-se testar com os modelos que estão em:
https://github.com/guilhermechagaskurtz/CGCCHibrido/tree/main/assets/Modelos3D)
- 🟩 Exibir mais de um objeto na tela
- 🟩 Permitir a seleção dos objetos, a partir de uma tecla 
    - Foram instanciados 2 objetos, seleciona qual o objeto transformar nas teclas 1 e 2
- 🟩 Permitir a aplicação de transformações no objeto selecionado:
    - 🟩 Rotacionar (R) nos eixos x, y e z
        - Rotaciona em enquanto pressionando X, Y e Z
    - 🟩 Transladar (T) nos eixos x, y e z (pode-se usar as setas ou WASD)
        - Move ao clicar em WASD
    - 🟩 Aplicar escala (S) nos eixos x, y e z (pode-se ter a opção de escala uniforme também)
        - Modifica a escala de forma uniforme usando Q e E
        - Modifica a escala por eixo usando J I K para os eixos x, y e z, respectivamente

## Tarefa 3  `Mesh.cpp` 

- Carregado arquivo .obj pelo loadSimpleOBJ
- Adicionado atributos de normais e coordenadas de textura (s t) ao VAO
- Renderizado objeto carregado
- Lido nome do arquivo de textura do material (.mtl) do objeto

![Tarefa 3](RESULT3.png)

## Tarefa 4  `Phong.cpp` 

- Carregado as informações dos vetores normais dos vértices no arquivo .OBJ (vn). 
- 🟨 Recuperar os coeficientes de iluminação ambiente, difusa e especular do arquivo de materiais (.mtl), que serão enviados pela aplicação para o fragment shader, onde calcularemos sua contribuição para a cor do pixel.
