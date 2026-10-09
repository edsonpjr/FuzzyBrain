/**
 * @file ui_console.hpp
 * @brief Auxiliar de interface de usuário em modo console para exibição de mensagens e
 * configurações.
 */

#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <string>
#include <vector>

/**
 * @class ConsoleUI
 * @brief Fornece operações simples de interface de usuário baseadas em console para a aplicação.
 */
class ConsoleUI {
  public:
    /**
     * @brief Constrói uma nova instância da classe ConsoleUI.
     */
    ConsoleUI();

    /**
     * @brief Exibe uma mensagem no console.
     *
     * @param message A string contendo a mensagem a ser impressa.
     */
    static void print_message(const std::string_view& message);

    /**
     * @brief Exibe uma mensagem no console em nova linha.
     *
     * @param message A string contendo a mensagem a ser impressa.
     */
    static void print_message_new_line(const std::string_view& message);

    /**
     * @brief Exibe uma linha vazia no console.
     */
    static void print_empty_line();

    /**
     * @brief Lê uma resposta numérica fornecida pelo usuário.
     *
     * @return O valor informado pelo usuário.
     */
    static auto read_answer() -> double;

    /**
     * @brief Exibe a mensagem de boas-vindas no console.
     */
    static void print_wellcome_message();

    /**
     * @brief Exibe o menu de controle da sessão no console e lê a escolha do usuário.
     *
     * @return O valor inteiro correspondente à escolha do usuário.
     */
    static auto print_session_control() -> int;

    /**
     * @brief Estrutura para armazenar os parâmetros de carregamento de perguntas e candidatos.
     */
    struct LoadedQuestionsParams {
        size_t questions_count;
        size_t candidates_count;
    };

    /**
     * @brief Exibe a mensagem de carregamento de perguntas e candidatos no console.
     *
     * @param params Estrutura contendo o número de perguntas e candidatos carregados.
     */
    static void print_load_questions_message(const LoadedQuestionsParams& params);

    /**
     * @brief Exibe a mensagem de sugestão de candidato para confirmação do usuário.
     *
     * @param candidate_name Nome do candidato sugerido.
     */
    static void print_suggestion_message(const std::string_view& candidate_name);

    /**
     * @brief Exibe a probabilidade de cada candidato durante o modo de depuração.
     *
     * @param candidate_name Nome do candidato.
     * @param probability Probabilidade associada ao candidato.
     */
    static void print_candidate_probalility_debug(const std::string_view& candidate_name, double probability);

    /**
     * @brief Exibe a mensagem de candidato forte encontrado.
     *
     * @param pluralize Se true, exibe a mensagem no plural; caso contrário, no singular.
     */
    static void print_strong_candidate_found(bool pluralize);

    /**
     * @brief Exibe a probabilidade de cada candidato forte durante o modo de depuração.
     *
     * @param candidate_name Nome do candidato.
     * @param probability Probabilidade associada ao candidato.
     */
    static void print_strong_candidate_debug(const std::string_view& candidate_name, double probability);

    /**
     * @brief Exibe a mensagem de sucesso ao identificar corretamente o candidato.
     *
     * @param candidate_name Nome do candidato identificado.
     */
    static void print_success_message(std::string_view candidate_name);

    /**
     * @brief Exibe a mensagem de encerramento quando o sistema não consegue identificar.
     */
    static void print_give_up_message();

    /**
     * @brief Lê uma resposta de sim/não fornecida pelo usuário.
     *
     * @return true para "sim"; false para "não".
     */
    static auto read_yes_no() -> bool;

  private:
    static constexpr std::string_view welcome_message =
        "Bem-vindo ao FuzzyBrain!";  ///< Mensagem de boas-vindas exibida ao iniciar o programa.
    static constexpr std::string_view session_control_message =
        "\n 1. Iniciar nova sessão\n 2. Iniciar nova sessão [modo debug]\n 3. Sair\n > Escolha uma opção: ";  ///< Mensagem de controle da sessão.
    static constexpr std::string_view starting_new_session_message =
        "Iniciando nova sessão...";  ///< Mensagem exibida ao iniciar uma nova sessão.
    static constexpr std::string_view exiting_program_message =
        "Encerrando o programa. Até mais!";  ///< Mensagem exibida ao encerrar o programa.
    static constexpr std::string_view invalid_session_input_message =
        "> Entrada inválida! Por favor, digite apenas 1, 2 ou 3: ";  ///< Mensagem de erro para entrada inválida para sessão.
    static constexpr std::string_view load_questions_message =
        "Foram carregadas do conjunto de dados {} perguntas para {} candidatos."; ///< Mensagem exibida após o carregamento das perguntas e candidatos.
    static constexpr std::string_view input_request_message =
        "> Digite um valor entre 0 e 1: ";  ///< Mensagem de prompt para o usuário.
    static constexpr std::string_view invalid_input_message =
        "> Entrada inválida! Por favor, digite um valor entre 0 e 1: ";  ///< Mensagem de erro para entrada inválida.
    static constexpr std::string_view suggestion_message =
        "Eu acho que é: {}. Estou certo? (s/n): ";  ///< Mensagem de sugestão de candidato.
    static constexpr std::string_view success_message =
        "Acertei! Era {} mesmo!";  ///< Mensagem de acerto.
    static constexpr std::string_view give_up_message =
        "Desculpe, infelizmente não consegui identificar desta vez.";  ///< Mensagem de desistência.
    static constexpr std::string_view invalid_yes_no_message =
        "> Resposta inválida! Por favor, digite 's' para sim ou 'n' para não: ";  ///< Mensagem de erro para entrada sim/não inválida.
    static constexpr std::string_view candidate_probability_debug_message =
        " - {}: {}%";  ///< Linha para exibir a probabilidade do candidato durante debug.
    static constexpr std::string_view strong_candidate_found_debug =
        " # Candidato{} forte{} #";  ///< Linha para exibir se há candidatos fortes durante debug.
    static constexpr std::string_view strong_candidate_probability_debug_format =
        " * Candidato: {} - {}%";  ///< Formato para exibir o candidato forte durante debug.

    static constexpr double percent_mulitplier =
        100.0;  ///< Multiplicador para converter probabilidade em porcentagem.
    /**
     * @brief Configura o console para utilizar a codificação de caracteres UTF-8, se necessário.
     *
     * Garante a exibição correta de caracteres especiais e acentuações no terminal.
     */
    static void utf8_language_settings();
};

#endif
