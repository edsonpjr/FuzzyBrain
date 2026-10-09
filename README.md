edsonpjr/FuzzyBrain# 🧠 FuzzyBrain

Bem-vindo ao repositório do projeto **FuzzyBrain**. Este é um sistema inteligente de análise de candidatos baseado em lógica fuzzy, desenvolvido pela Equipe FuzzyBrain (CIn-UFPE).

Este documento define os padrões técnicos, de nomenclatura, estrutura de pastas, guias de inicialização e instruções de uso para garantir a consistência entre desenvolvedores que utilizam **Visual Studio**, **VS Code Community** e outras IDEs modernas.

## 👥 Equipe FuzzyBrain

Desenvolvido por **Equipe FuzzyBrain** (projeto acadêmico - CIn UFPE)
- **André Santos** (agds@cin.ufpe.br)
- **Edson Júnio** (ejapj@cin.ufpe.br)
- **Felipe Farias** (fjbf@cin.ufpe.br)
- **Rogério Henrique** (rhmt@cin.ufpe.br)

---

## 📁 Estrutura de Pastas do Projeto

O projeto adota uma divisão estrita entre arquivos de declaração (`.hpp`) e implementação (`.cpp`) dentro do diretório principal de código:

```text
fuzzybrain/
├── .gitignore                          # Arquivo de exclusão do Git
├── .clang-format                       # Configuração de formatação Clang
├── .clang-tidy                         # Configuração Clang-Tidy
├── .clang-tidy-config                  # Controle ON/OFF do Clang-Tidy
├── CMakeLists.txt                      # Configuração CMake
├── Doxyfile                            # Configuração Doxygen
├── README.md                           # Este arquivo
├── docs/                               # Documentação gerada pelo Doxygen
├── scripts/                            # Scripts utilitários
│   └── convert-clang-tidy-to-json.sh   # Converter saída Clang-Tidy para JSON
├── src/
│   ├── headers/                        # Apenas arquivos de cabeçalho (.hpp)
│   │   ├── candidate.hpp               # Classe de candidato
│   │   ├── dataset_reader.hpp          # Leitor de datasets
│   │   ├── engine.hpp                  # Núcleo da aplicação
│   │   ├── probability_engine.hpp      # Motor de probabilidades fuzzy
│   │   ├── question.hpp                # Classe de questão
│   │   ├── relation.hpp                # Classe de relação
│   │   ├── session_controller.hpp      # Controlador de sessão
│   │   └── ui_console.hpp              # Interface de console
│   ├── resources/
│   │   └── knowledge_base.txt          # Base de conhecimento
│   ├── main.cpp                        # Ponto de entrada do programa
│   ├── mainpage.dox                    # Página principal da documentação Doxygen
│   ├── candidate.cpp                   # Implementação de candidato
│   ├── dataset_reader.cpp              # Implementação do leitor de datasets
│   ├── engine.cpp                      # Implementação do núcleo
│   ├── probability_engine.cpp          # Implementação do motor fuzzy
│   ├── question.cpp                    # Implementação de questão
│   ├── relation.cpp                    # Implementação de relação
│   ├── session_controller.cpp          # Implementação do controlador
│   └── ui_console.cpp                  # Implementação da interface console
└── out/                                # Pasta de saída (não rastreada no Git)
    └── build/                          # Artefatos de build
```

### 📝 Componentes Principais

| Componente | Descrição |
| :--- | :--- |
| **`candidate.hpp/cpp`** | Definição e implementação da classe de candidato |
| **`dataset_reader.hpp/cpp`** | Leitor de arquivos de dados em formato texto |
| **`engine.hpp/cpp`** | Núcleo da aplicação, orquestra todo o fluxo |
| **`probability_engine.hpp/cpp`** | Motor de cálculo de probabilidades com lógica fuzzy |
| **`question.hpp/cpp`** | Representação de perguntas/atributos |
| **`relation.hpp/cpp`** | Gestão de relações entre candidatos |
| **`session_controller.hpp/cpp`** | Controle de sessões e estado da aplicação |
| **`ui_console.hpp/cpp`** | Interface de usuário em modo console |
| **`knowledge_base.txt`** | Dados estáticos de conhecimento |
| **`main.cpp`** | Ponto de entrada, gerencia menu e loop principal |

### 📂 Organização de Arquivos

*   **`src/headers/`**: Contém exclusivamente arquivos `.hpp` - apenas declarações
*   **`src/`**: Contém arquivos `.cpp` - implementações correspondentes aos headers
*   **`src/resources/`**: Dados estáticos e arquivos de configuração
*   **`docs/`**: Documentação HTML gerada pelo Doxygen
*   **`scripts/`**: Utilitários de build e processamento
*   **`out/`**: Artefatos de build (não rastreado no Git)

---

## 🔤 Padrões de Nomenclatura e Escrita

Para evitar conflitos de compilação entre sistemas operacionais (Windows vs. Linux/Mac), adotamos as seguintes convenções baseadas nas **C++ Core Guidelines**:

### 📖 Referência: C++ Core Guidelines

Este projeto segue as diretrizes de codificação C++ definidas pelos criadores da linguagem:
- **Bjarne Stroustrup** (criador do C++)
- **Herb Sutter** (especialista em C++)

Referência completa: [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

**Princípios principais seguidos:**
- Preferir segurança de tipos (`type safety`)
- Usar RAII (Resource Acquisition Is Initialization)
- Evitar ponteiros brutos quando possível (preferir smart pointers)
- Escrever código legível e mantível
- Usar features modernas do C++20
- Const correctness (use `const` amplamente)
- Memory safety (gestão segura de memória)
- Documentação clara e concisa

### 1. Arquivos e Pastas (`snake_case` minúsculo)
*   **Regra:** Todos os nomes de pastas e arquivos devem ser escritos em letras minúsculas e separados por sublinhado (`_`).
*   **Extensões:** Use obrigatoriamente `.hpp` para cabeçalhos e `.cpp` para implementações.
*   **Consistência:** O arquivo de implementação deve ter **exatamente o mesmo nome** do seu arquivo de cabeçalho.
    *   *Certo:* `src/headers/processador_dados.hpp` e `src/resources/processador_dados.cpp`
    *   *Errado:* `src/headers/ProcessadorDados.H` e `src/resources/processadorDados.cpp`

### 2. Elementos de Código

| Elemento | Padrão | Exemplo |
| :--- | :--- | :--- |
| **Classes e Structs** | `PascalCase` | `class GerenciadorJanela { ... };` |
| **Métodos e Funções** | `camelCase` | `void salvarDadosUsuario();` |
| **Variáveis e Parâmetros** | `snake_case` | `int pontuacao_atual = 0;` |
| **Constantes e Macros** | `snake_case` | `const int limite_maximo = 100;` |

---

## 🤖 Formatação Automática de Código

Utilizamos o utilitário **Clang-Format** (baseado no estilo do Google) com espaçamento de **4 espaços por tabulação** para manter o código consistente. O arquivo `.clang-format` já está configurado na raiz do projeto.

### Formatação Automática

*   **No Visual Studio:** Detecta o arquivo automaticamente e formata o código ao digitar `;` ou fechar chaves `}`.
*   **No VS Code:** Certifique-se de ter a extensão **C/C++ de Microsoft** instalada e ative a formatação ao salvar no `settings.json`:
    ```json
    {
      "editor.formatOnSave": true,
      "editor.defaultFormatter": "ms-vscode.cpptools"
    }
    ```

### Formatação Manual

Se precisar formatar um arquivo manualmente:
```powershell
clang-format -i src/seu_arquivo.cpp
```

Formatar recursivamente todos os arquivos:
```powershell
clang-format -i src/**/*.cpp src/**/*.hpp
```

**Espaçamento:** 4 espaços (não use tabs)
**Largura da linha:** 100 caracteres (máximo recomendado)
**Quebra de linhas:** Automática em expressões longas

---

## 🔍 Validação de Código com Clang-Tidy

O projeto utiliza **Clang-Tidy** (versão 14+) para validação contínua de conformidade com as **C++ Core Guidelines**. A validação está integrada ao processo de build e também é executada automaticamente em Merge Requests.

**Características:**
- Validação baseada em regras da C++ Core Guidelines
- Integração com CI/CD (GitLab Pipelines) para MRs
- Conversão de resultados para formato JSON e relatórios de qualidade

### ⚡ Liga/Desliga Rápido

O controle é feito pelo arquivo `.clang-tidy-config` na raiz do projeto:

**Para ATIVAR clang-tidy localmente:**
Edite `.clang-tidy-config` e mude o conteúdo para:
```
ON
```

Depois rode ou reconfi gure:
```powershell
cmake.exe --build build
```

**Para DESATIVAR:**
Edite `.clang-tidy-config` e mude para:
```
OFF
```

Depois rode:
```powershell
cmake.exe --build build
```

### 💡 Como funciona:

- O CMake **monitora automaticamente** o arquivo `.clang-tidy-config`
- Quando você o modifica, o próximo `cmake --build build` reconfigura e aplica a mudança
- Se `ON`: clang-tidy roda como **post-build step** (após a compilação)
- Se `OFF`: build normal, sem análise
- **CI/CD:** Clang-tidy sempre roda em Merge Requests (MRs) independente da configuração local
- **Resultados:** Aparecem na aba "Code Quality" do MR no GitLab

### 🔧 Rodar clang-tidy manualmente:

```powershell
cmake --build build --target clang-tidy-check
```

Ou em um arquivo específico:
```bash
clang-tidy src/candidate.cpp -- -std=c++20 -Isrc/headers
```

### 📋 Verificações Incluídas

Clang-tidy valida:
- **Performance**: Otimizações perdidas, cópias desnecessárias
- **Readability**: Clareza e legibilidade do código
- **Modernize**: Uso de features C++20 apropriadas
- **Safety**: Segurança de memória, ponteiros, concorrência

### 🎯 Rodar clang-tidy manualmente (sem quickbuild):
```powershell
cmake --build build --target clang-tidy-check
```

---

## 📚 Documentação com Doxygen

O projeto utiliza **Doxygen** para gerar documentação técnica automática a partir dos comentários no código-fonte.

### 📝 Melhores Práticas de Documentação

**Regra Principal:** 
- **Documente APENAS nos arquivos `.hpp` (headers)**
- O Doxygen extrairá automaticamente as documentações
- Não adicione blocos de documentação nos arquivos `.cpp`

### ✍️ Formato de Comentários Doxygen

Use o formato `/**` para iniciar blocos de documentação:

```cpp
/**
 * @file exemplo.hpp
 * @brief Descrição breve do arquivo
 */

/**
 * @class MinhaClasse
 * @brief Descrição breve da classe
 * 
 * Descrição detalhada da classe explicando seu propósito
 * e como usar.
 */
class MinhaClasse {
public:
    /**
     * @brief Descrição breve do método
     * 
     * @param parametro1 Descrição do parâmetro 1
     * @param parametro2 Descrição do parâmetro 2
     * @return Descrição do retorno
     */
    auto meu_metodo(int parametro1, std::string parametro2) -> bool;
};
```

### 📊 Tags Doxygen Frequentes

| Tag | Uso | Exemplo |
| :--- | :--- | :--- |
| `@file` | Documentar arquivo | `@file candidate.hpp` |
| `@brief` | Descrição breve (uma linha) | `@brief Representa um candidato` |
| `@details` | Descrição detalhada | `@details Explanação completa...` |
| `@param` | Documentar parâmetro | `@param candidates Lista de candidatos` |
| `@return` | Documentar retorno | `@return Resultado do processamento` |
| `@throw` | Exceção lançada | `@throw std::invalid_argument Se dados inválidos` |
| `@see` | Referência cruzada | `@see Engine, ProbabilityEngine` |
| `@deprecated` | Marcar como obsoleto | `@deprecated Use novaFuncao() em t.1` |
| `@note` | Nota importante | `@note Este método não é thread-safe` |
| `@warning` | Aviso | `@warning Não modifique diretamente` |
| `@example` | Exemplo de uso | `@example demo.cpp` |

### 🎯 Geração de Documentação

A documentação é gerada automaticamente através de:

1. **Localmente:** Execute no diretório raiz:
```bash
doxygen Doxyfile
```

2. **CI/CD:** Documentação é gerada e disponibilizada em cada merge para `main`

3. **Saída:** Documentação HTML é gerada em `docs/html/index.html`. Abra em seu navegador:
```bash
start docs/html/index.html  # Windows
# ou
open docs/html/index.html   # Mac
# ou
firefox docs/html/index.html # Linux
```

**Incluições úteis na documentação:**
- Diagrama de classes UML
- Dependências entre arquivos
- Graphs de chamadas de funções
- Índices de símbolos e referências cruzadas
- Busca em toda a documentação

Para mais detalhes: [Wiki - Documentação com Doxygen](https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain/-/wikis/Documenta%C3%A7%C3%A3o-com-Doxygen)

---

### 🔄 CI/CD no GitLab (Automático em Merge Requests)

O projeto está configurado com **GitLab CI/CD** com dois jobs separados:

#### 🔍 **1. Validação de Código (Clang-Tidy)**

**Quando roda:** Quando você abre um **Merge Request para `main`**

**O que acontece:**
1. GitLab dispara o pipeline automaticamente
2. Clang-tidy roda em ambiente Linux (Debian bookworm)
3. Analisa todo o código C++ do projeto
4. Gera um relatório em formato GitLab Code Quality
5. **Resultados aparecem na aba "Code Quality" do MR**

**Você verá:**
- Lista de todos os problemas encontrados
- Arquivo, linha e coluna exata
- Descrição do problema
- Pode corrigir localmente e committar novamente - pipeline roda automático

#### 📄 **2. Publicação de Documentação (Doxygen - Artefatos)**

**Quando roda:** Quando o MR é **mergeado para `main`**

**O que acontece:**
1. Doxygen gera a documentação técnica do código
2. Cria diagrama UML de classes
3. Gera graphs de chamadas de funções
4. Documenta todas as classes, métodos e funções
5. Documentação é empacotada como **artefato do pipeline**

⚠️ **Nota Importante:** GitLab Pages está **desativado** para este projeto. A documentação é disponibilizada exclusivamente como artefato.

### 📥 Como Acessar a Documentação

**Via Artefatos do Pipeline:**

1. Acesse: [Pipelines GitLab - Branch Main](https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain/-/pipelines?scope=branches&ref=main&status=success)

2. Selecione um build bem-sucedido (status ✅ **Success**) na branch `main`

3. Clique na aba **"Artifacts"** (ou "Artefatos")

4. Procure pelo arquivo de documentação (`doxygen_docs` ou similar)

5. Faça download e descompacte em seu computador

6. Abra `index.html` em seu navegador

**Conteúdo da Documentação:**
- ✅ Referência completa de APIs
- ✅ Diagrama UML de arquitetura
- ✅ Exemplo de uso de cada classe
- ✅ Relacionamentos entre componentes
- ✅ Busca textual completa

#### 📊 **Fluxo Visual:**

```
Seu Branch (ex: feature/xyz)
        ↓
   Abrir MR para main
        ↓
   ✅ Clang-Tidy valida código
   ✅ Revisa problemas no MR
   ✅ Corrige e faz commits
        ↓
   Faz Merge para main
        ↓
   📄 Documentação é gerada como artefato
```
   Abrir MR para main
        ↓
   ✅ Clang-Tidy valida código
   ✅ Revisa problemas no MR
   ✅ Corrige e faz commits
        ↓
   Faz Merge para main
        ↓
   📄 Documentação é publicada
```

#### 🔗 **Acessar os Relatórios:**

- **Code Quality (no MR):** Clique na aba "Code Quality" dentro do MR
- **Build Logs:** Clique em "Pipelines" no repositório
- **Documentação (Pages):** Em `Settings > Pages` você vê a URL do site

Nenhuma configuração adicional necessária - está pronto! 🚀

---

## ⚡ Quick Start - Guia Rápido de Uso

### Compilação e Execução

**1. Clone o repositório:**
```bash
git clone https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain.git
cd fuzzybrain
```

**2. Crie pasta de build e configure CMake:**
```powershell
mkdir build
cd build
cmake -G Ninja ..
```

**3. Compile:**
```powershell
cmake --build .
```

**4. Execute a aplicação:**
```powershell
# Windows
.\fuzzybrain.exe

# Linux/Mac
./fuzzybrain
```

### Menu Interativo da Aplicação

Após executar, você verá o menu:

```
Bem-vindo ao FuzzyBrain!

 1. Iniciar nova sessão
 2. Iniciar nova sessão [modo debug]
 3. Sair
 > Escolha uma opção:
```

### Opção 1: Iniciar nova sessão

- Processa todos os candidatos na base de conhecimento
- Aplica o motor de probabilidades fuzzy
- Exibe resultado da análise
- Tempo de execução típico: < 1 segundo

### Opção 2: Iniciar nova sessão [modo debug]

- **Ativa saída detalhada** da análise
- Mostra **percentual de candidatos processados** (ex: 95.24%)
- Exibe **detecção de outliers** (candidatos anômalos)
- Útil para validação e troubleshooting

---

## 🚀 Como Sincronizar e Executar o Projeto

### Sincronização Inicial (Terminal Git)
Se você já tem uma pasta local e precisa vinculá-la a este repositório do CIn-UFPE:
```bash
cd caminho_da_sua_pasta_local
git remote add origin https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain.git
git branch -M main
git push -uf origin main
```


### Fluxo de Desenvolvimento pelas IDEs (Via CMake)
**Nunca tente compilar arquivos `.cpp` individualmente.** O projeto é gerenciado universalmente pelo CMake.

*   **💻 No Visual Studio:** Vá em *Arquivo > Abrir > Pasta Local* e selecione a raiz do projeto. O VS detectará o `CMakeLists.txt` e configurará os botões de execução automaticamente.
*   **📝 No VS Code:** Abra a pasta raiz. Certifique-se de ter as extensões **C/C++** e **CMake Tools** instaladas. Selecione o seu compilador (Kit) e utilize os botões **`Build`** e **`Play/Debug`** localizados estritamente na **barra inferior** do VS Code.

---
💡 *Dica de equipe: Arquivos `.exe`, `.out` ou pastas temporárias de build (`/build/`, `.vs/`) estão bloqueados no `.gitignore` e não devem ser enviados ao GitLab.*

### ⚙️ Automação de Novos Arquivos (CMake)

O projeto está configurado para **detectar automaticamente** qualquer arquivo `.cpp` ou `.hpp` adicionado às pastas, sem a necessidade de editar o arquivo `CMakeLists.txt` manualmente.

⚠️ **Regra importante ao criar novos arquivos:** 
Sempre que você criar um novo arquivo de código, as IDEs precisam atualizar o cache para reconhecê-lo. Siga o comando abaixo dependendo do seu editor:

*   **No VS Code:** Abra a paleta de comandos (`Ctrl + Shift + P` ou `Cmd + Shift + P`) e execute:  
    `CMake: Delete Cache and Reconfigure`
*   **No Visual Studio:** Clique com o botão direito sobre o arquivo `CMakeLists.txt` e selecione:  
    `Gerar Cache para fuzzybrain` (Generate Cache)

---

## 🚀 Primeiros Passos

Para facilitar seu início com este projeto, aqui está uma lista de próximos passos recomendados.

Já é experiente? Basta editar este README.md e personalizá-lo! Quer facilitar? [Use o template na parte inferior](#editing-this-readme)!

## 📤 Adicionando seus arquivos

* [Criar](https://docs.gitlab.com/user/project/repository/web_editor/#create-a-file) ou [fazer upload](https://docs.gitlab.com/user/project/repository/web_editor/#upload-a-file) de arquivos
* [Adicionar arquivos usando a linha de comando](https://docs.gitlab.com/topics/git/add_files/#add-files-to-a-git-repository) ou fazer push de um repositório Git existente com o seguinte comando:

```bash
cd seu_repositorio_existente
git remote add origin https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain.git
git branch -M main
git push -uf origin main
```



## 📋 Licença

A licença para este projeto está **pendente de definição**. Até o momento, não há uma licença específica atribuída.

### Informações sobre a Licença

- **Status Atual:** Em desenvolvimento - licença não definida
- **Tipo de Projeto:** Acadêmico (CIn-UFPE)
- **Uso Permitido:** Apenas para fins educacionais e de pesquisa
- **Distribuição:** Não autorizada sem consentimento dos autores

Para questões relacionadas à licença e permissões de uso, entre em contato com os autores:
- **André Santos** (agds@cin.ufpe.br)
- **Edson Júnio** (ejapj@cin.ufpe.br)
- **Felipe Farias** (fjbf@cin.ufpe.br)
- **Rogério Henrique** (rhmt@cin.ufpe.br)

---

## 🛠️ Ambientes de Desenvolvimento Suportados

### IDEs Suportadas

- **Visual Studio 2022+** (Professional/Community)
  - Detecta CMakeLists.txt automaticamente
  - Integração nativa com Clang-format e Clang-tidy
  - IntelliSense completo para C++20

- **Visual Studio Code Community**
  - Extensão: C/C++ Extension Pack
  - Extensão: CMake Tools
  - Extensão: Doxygen Documentation Generator
  - Integração com compiladores MSVC/GCC/Clang

### Plataformas Suportadas

| Plataforma | Status | IDE Recomendada |
| :--- | :--- | :--- |
| **Windows 10/11** | ✅ Principal | Visual Studio 2022 |
| **Linux** (Ubuntu 20.04+) | ✅ Completo | VS Code |
| **macOS** (Intel/Apple Silicon) | ✅ Completo | VS Code / Xcode |

### Requisitos Mínimos

- **Compilador:** MSVC 14.3+ (Visual Studio 2022), GCC 11+, ou Clang 14+
- **CMake:** 3.15 ou posterior
- **C++ Standard:** C++20
- **Memória:** 512 MB (mínimo), 2 GB (recomendado)
- **Espaço em Disco:** ~500 MB (incluindo build)

### Configuração Inicial por Plataforma

#### Windows (Visual Studio)
1. Abra Visual Studio 2022
2. Clique em "Abrir uma pasta local"
3. Selecione a pasta `fuzzybrain`
4. VS detectará CMakeLists.txt automaticamente
5. Pressione Ctrl+Shift+B para compilar

#### Windows/Linux (VS Code)
1. Instale C/C++ Extension Pack
2. Instale CMake Tools
3. Abra a pasta `fuzzybrain`
4. Selecione Kit: MSVC 14.3 (Windows) ou GCC/Clang (Linux)
5. Clique em "Build" na barra inferior

#### Linux/macOS (Terminal)
```bash
cd fuzzybrain
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

---

## 📞 Suporte e Contacto

### Reportar Problemas

Use o [Issue Tracker do GitLab](https://gitlab.cin.ufpe.br/edoo20262/fuzzybrain/-/issues) para:
- Reportar bugs
- Sugerir funcionalidades
- Pedir esclarecimentos

### Contribuições

Contribuições são bem-vindas! Siga o processo:
1. Crie uma branch: `git checkout -b feature/sua-feature`
2. Commit suas mudanças: `git commit -m "Add feature"`
3. Push para a branch: `git push origin feature/sua-feature`
4. Abra um Merge Request

**Obrigado por usar FuzzyBrain! 🚀**
