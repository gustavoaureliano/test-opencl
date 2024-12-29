# test-opencl

Este projeto contém uma série de testes realizados para estudar computação paralela e OpenCL. Embora não tenha uma aplicação prática específica, serve como base para experimentos com paralelismo, utilizando a série de Leibniz para calcular π.

## Descrição

O projeto explora diferentes abordagens para adicionar paralelismo ao cálculo de π:

- **CPU com Threads**:  
  - `pithread.c` e `pithread2.c`: Implementações que utilizam threads para paralelizar o cálculo na CPU.

- **GPU com OpenCL**:  
  - `piopcl.c`: Implementação que utiliza OpenCL para executar o cálculo na GPU, empregando o kernel `pikernel.cl`. A função `atomic_add_f` utilizada no kernel foi extraída de [Atomic Float Addition in OpenCL](https://pipinspace.github.io/blog/atomic-float-addition-in-opencl.html).

Outros arquivos no projeto incluem:

- `opcl.c`: Minha primeira tentativa com OpenCL, baseada em um exemplo cuja fonte não me recordo.

- `baseopcl.c`: Desenvolvido com base nos exemplos do livro *Heterogeneous Computing with OpenCL 2.0* de Kaeli et al., 2015.

- `newpi.c`: Outra variação do cálculo de π para experimentação.

- `vector_add_kernel.cl` e `vector_add_kernel_f.cl`: Kernels para testes adicionais com OpenCL.

## Verificação de Compatibilidade com OpenCL

Antes de compilar e executar os programas, é recomendável verificar se o seu sistema possui dispositivos compatíveis com OpenCL. Para isso, utilize o utilitário `clinfo`, que exibe informações sobre as plataformas e dispositivos OpenCL disponíveis no sistema.

- **Instalação do clinfo**:

  - **Arch Linux**:
    ```bash
    sudo pacman -S clinfo
    ```

  - **Ubuntu / Debian**:
    ```bash
    sudo apt install clinfo
    ```

- **Uso**:
  ```bash
  clinfo -l

## Compilação

Para compilar os programas deste projeto, utilize o GCC com a biblioteca OpenCL. Por exemplo, para compilar o `piopcl.c`, execute:

```bash
gcc piopcl.c -lOpenCL -o piopcl
```

## Observações

Embora o programa funcione na GPU, a precisão limitada do tipo `float` representa um desafio. Trabalhar com `double` em GPUs pode ser mais complexo e não foi o foco principal deste estudo. O objetivo principal foi explorar técnicas de paralelismo, e não necessariamente obter um cálculo preciso de π.

## Referências

- **Livro**: *Heterogeneous Computing with OpenCL 2.0*, Morgan Kaufmann, 1ª edição, 2015. Autores: David R. Kaeli, Perhaad Mistry, Dana Schaa, Dong Zhang.

- **Artigo**: [Atomic Float Addition in OpenCL](https://pipinspace.github.io/blog/atomic-float-addition-in-opencl.html).
