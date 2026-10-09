/**
 * @file session_controller.hpp
 * @brief Controlador responsável por orquestrar uma sessão de perguntas.
 */

#ifndef SESSION_CONTROLLER_HPP
#define SESSION_CONTROLLER_HPP

#include <vector>

#include "dataset_reader.hpp"
#include "probability_engine.hpp"

/**
 * @class SessionController
 * @brief Coordena o fluxo de perguntas, respostas e atualização das probabilidades.
 */
class SessionController {
  public:
    /**
     * @brief Constrói um controlador para uma sessão de perguntas.
     *
     * @param probability_engine Motor responsável pela seleção das perguntas
     * e atualização das probabilidades.
     * @param matrix Matriz contendo as perguntas e suas relações com os candidatos.
     * @param debug_mode Indica se o modo debug está ativo.
     */
    SessionController(ProbabilityEngine& probability_engine, std::vector<QuestionRow>& matrix, bool debug_mode);

    /**
     * @brief Executa o fluxo principal da sessão de perguntas.
     */
    void run();

    /**
     * @brief Imprime informações de depuração sobre os candidatos e suas probabilidades.
     *
     * @param probability_engine Motor de probabilidades utilizado durante a sessão.
     */
    static void print_debug_candidates_info(const ProbabilityEngine& probability_engine);

  private:
    ProbabilityEngine* probability_engine_; ///< Motor de probabilidades utilizado durante a sessão.
    std::vector<QuestionRow>* matrix_;  ///< Matriz de perguntas utilizada durante a sessão.
    bool debug_mode_;  ///< Indica se o modo debug está ativo.
};

#endif