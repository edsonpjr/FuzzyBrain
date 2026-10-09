/**
 * @file engine.hpp
 * @brief Ponto de entrada do motor (Engine) e orquestração do fluxo da aplicação.
 */

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <vector>
#include "dataset_reader.hpp"
#include "probability_engine.hpp"
#include "ui_console.hpp"

/**
 * @class Engine
 * @brief Coordena a interface de usuário, o carregamento do conjunto de dados e a execução da
 * aplicação.
 */
class Engine {
  public:
    /**
     * @brief Constrói uma nova instância da classe Engine.
     *
     * Carrega o conjunto de dados e inicializa o motor de probabilidade com a matriz carregada.
     */
    Engine();

    /**
     * @brief Constrói uma nova instância da classe Engine.
     *
     * Carrega o conjunto de dados e inicializa o motor de probabilidade com a matriz carregada.
     * 
     * @param debug_mode Indica se o modo debug está ativo.
     */
    Engine(bool debug_mode);

    /**
     * @brief Inicia o motor e executa o laço (loop) principal da aplicação.
     * 
     * Iniciará a sessão de perguntas e respostas, interagindo com o usuário via console.
     */
    void start();

  private:
    inline static const std::string_view dataset_file_name =
        "knowledge_base.txt";  ///< Nome padrão do arquivo do conjunto de dados (dataset).
    
    ConsoleUI ui_console_;  ///< Auxiliar de interface de usuário em modo console.
    DatasetReader
        reader_;  ///< Leitor de conjunto de dados utilizado para carregar a base de conhecimento.
    std::vector<QuestionRow> matrix_;        ///< Matriz em memória contendo as linhas de perguntas.
    ProbabilityEngine probability_engine_;  ///< Motor de probabilidade e seleção de perguntas.
    bool debug_mode_ = false;               ///< Indica se o modo debug está ativo.
};

#endif
