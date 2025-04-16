# test-opencl

Este projeto contém uma série de testes realizados para estudar computação paralela e OpenCL. Embora não tenha uma aplicação prática específica, serve como base para experimentos com paralelismo, utilizando a série de Leibniz para calcular π.

---

## Índice

- [Descrição](#descrição)
  - [Abordagens de Cálculo de π](#abordagens-de-cálculo-de-π)
  - [Outros Arquivos do Projeto](#outros-arquivos-do-projeto)
- [Verificação de Compatibilidade com OpenCL](#verificação-de-compatibilidade-com-opencl)
- [Compilação](#compilação)
- [Rodando OpenCL no Android com Termux](#rodando-opencl-no-android-com-termux)
  - [Pré-requisitos e Configuração](#pré-requisitos-e-configuração)
  - [Compilando e Executando](#compilando-e-executando)
- [Observações](#observações)
- [Fontes e Referências](#fontes-e-referências)

---

## Descrição

O projeto explora diversas abordagens para aplicar paralelismo no cálculo de π:

### Abordagens de Cálculo de π

- **CPU com Threads**  
  - **pithread.c** e **pithread2.c**  
    Implementações que utilizam threads para paralelizar o cálculo na CPU.

- **GPU com OpenCL**  
  - **piopcl.c**  
    Implementação que utiliza OpenCL para executar o cálculo na GPU, em conjunto com o kernel **pikernel.cl**.  
    A função `atomic_add_f` usada no kernel foi baseada no artigo [Atomic Float Addition in OpenCL](https://pipinspace.github.io/blog/atomic-float-addition-in-opencl.html).

### Outros Arquivos do Projeto

- **opcl.c**  
  Primeira tentativa de implementação com OpenCL, baseada em um exemplo cuja fonte original não é lembrada.

- **baseopcl.c**  
  Desenvolvido a partir dos exemplos do livro *Heterogeneous Computing with OpenCL 2.0* (Kaeli et al., 2015).

- **newpi.c**  
  Outra variação do cálculo de π para fins experimentais.

- **vector_add_kernel.cl** e **vector_add_kernel_f.cl**  
  Kernels para testes adicionais com OpenCL, voltados para operações vetoriais.

---

## Verificação de Compatibilidade com OpenCL

Antes de compilar e executar os programas, certifique-se de que seu sistema possui dispositivos compatíveis com OpenCL. Para isso, utilize o utilitário `clinfo`, que exibe informações sobre as plataformas e dispositivos disponíveis.

### Instalação do clinfo

- **Arch Linux**:
  ```bash
  sudo pacman -S clinfo
  ```

- **Ubuntu / Debian**:
  ```bash
  sudo apt install clinfo
  ```

### Uso do clinfo

```bash
clinfo -l
```

---

## Compilação

Para compilar os programas do projeto, utilize o GCC com a biblioteca OpenCL. Por exemplo, para compilar o programa **piopcl.c**, execute:

```bash
gcc piopcl.c -lOpenCL -o piopcl
```

Certifique-se de ter o OpenCL devidamente instalado e configurado no seu sistema.

---

## Rodando OpenCL no Android com Termux

Esta seção descreve os passos necessários para executar os programas deste projeto em dispositivos Android compatíveis com OpenCL, utilizando o Termux como ambiente de execução.

### Pré-requisitos e Configuração

1. **Verificação da Biblioteca OpenCL**  
   Em alguns dispositivos Android, a biblioteca OpenCL pode estar localizada em:
   ```
   /system/vendor/lib64/libOpenCL.so
   ```
   Observe que esta localização pode variar de acordo com o dispositivo.

2. **Instalação do Termux e Pacotes Necessários**  
   No Termux, atualize os pacotes e instale as dependências:
   ```bash
   pkg update && pkg upgrade
   pkg install git clang clinfo
   ```

3. **Clone dos Repositórios Relevantes**  
   Clone os headers e o repositório do projeto:
   ```bash
   git clone https://github.com/KhronosGroup/OpenCL-Headers.git
   git clone https://github.com/gustavoaureliano/test-opencl.git
   ```

4. **Verificação do Suporte ao OpenCL**  
   Defina a variável de ambiente `LD_LIBRARY_PATH` para apontar para o diretório da biblioteca e execute:
   ```bash
   LD_LIBRARY_PATH=/system/vendor/lib64 clinfo
   ```

### Compilando e Executando

1. **Compilação**  
   Acesse o diretório do projeto e compile utilizando o `clang`:
   ```bash
   cd test-opencl
   clang -o piopcl piopcl.c -I../OpenCL-Headers -lOpenCL
   ```

2. **Execução**  
   Execute o programa definindo a variável `LD_LIBRARY_PATH`:
   ```bash
   LD_LIBRARY_PATH=/system/vendor/lib64 ./piopcl 1.0e-9
   ```
   *Observação*: O parâmetro `1.0e-9` especifica a precisão desejada para o cálculo de π. Valores menores aumentam a precisão (ainda que a precisão do `float` na GPU limite os ganhos após certo ponto).

---

## Observações

- **Precisão**:  
  Mesmo com a execução na GPU, a precisão do tipo `float` pode limitar os resultados. Trabalhar com `double` em GPUs envolve desafios adicionais e não foi o enfoque deste estudo.

- **Objetivo**:  
  O principal objetivo deste projeto é experimentar e explorar técnicas de paralelismo, não alcançar a máxima precisão no cálculo de π.

---

## Fontes e Referências

### OpenCL e Implementações

- **Atomic Float Addition in OpenCL**  
  [Atomic Float Addition in OpenCL](https://pipinspace.github.io/blog/atomic-float-addition-in-opencl.html)

- **OpenCL no Android e Termux**:
  - [Oblomov/clinfo](https://github.com/Oblomov/clinfo)
  - [KhronosGroup/OpenCL-Headers](https://github.com/KhronosGroup/OpenCL-Headers)
  - [research-note/termux-opencl](https://github.com/research-note/termux-opencl)
  - Discussões no GitHub:  
    [Clinfo Issue #46](https://github.com/Oblomov/clinfo/issues/46) e [Termux Packages Issue #16852](https://github.com/termux/termux-packages/issues/16852)

### Literatura

- **Livro**:  
  *Heterogeneous Computing with OpenCL 2.0*, Morgan Kaufmann, 1ª edição, 2015.  
  Autores: David R. Kaeli, Perhaad Mistry, Dana Schaa, Dong Zhang.
